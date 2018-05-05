#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "tim.h"
#include "gpio.h"

static uint32_t tim0_ov_max_cnt;

void set_tim0_ov_max_cnt(uint32_t val)
{
    tim0_ov_max_cnt = val;
}

uint32_t get_tim0_ov_max_cnt(void)
{
    return tim0_ov_max_cnt;
}

void enable_tim0(void)
{
    PRR &= ~_BV(PRTIM0);
}
uint32_t get_shift_prescaler(void)
{
    uint8_t cs = (TCCR0B & TIM_CS_MASK) >> TIM_CS_POS;
    uint32_t prescaler;
    switch(cs)
    {
    default:
    case TIM_NO_CLK:
        prescaler = 0;
        break;
    case TIM_NO_PRESCALING:
        prescaler = 1;
        break;
    case TIM_PRESCALING_DIV8:
        prescaler = 3;
        break;
    case TIM_PRESCALING_DIV64:
        prescaler = 6;
        break;
    case TIM_PRESCALING_DIV256:
        prescaler = 8;
        break;
    case TIM_PRESCALING_DIV1024:
        prescaler = 10;
        break;
    }
    return prescaler;
}

uint32_t get_tim0_cnt_freq(void)
{
    uint8_t cs = (TCCR0B & TIM_CS_MASK) >> TIM_CS_POS;
    if (cs != TIM_NO_CLK)
    {
        return F_CPU >> get_shift_prescaler();
    }
    return 0;
}

uint32_t get_8bit_tim_freq(uint32_t cnt_freq, uint8_t mode, uint8_t ocr_ch_a)
{
    uint8_t top;
    uint32_t tim_freq = 0xDEADBEEF;
    switch(mode) {
    case TIM0_WGM_NORMAL:
        top = 0xFF;
        tim_freq = cnt_freq / (1 + top);
        break;
    case TIM0_WGM_PWM:
        top = 0xFF;
        tim_freq = cnt_freq / (2*(top-1));
        break;
    case TIM0_WGM_FAST_PWM:
        top = 0xFF;
        tim_freq = cnt_freq / (1 + top);
        break;
    case TIM0_WGM_PWM_TOP_OCR:
        top = ocr_ch_a;
        tim_freq = cnt_freq / (2*(top-1));
        break;
    case TIM0_WGM_CTC:
        top = ocr_ch_a;
        tim_freq = cnt_freq / (2*(1 + top));
        break;
    case TIM0_WGM_FAST_PWM_TOP_OCR:
        top = ocr_ch_a;
        tim_freq = cnt_freq / (1 + top);
        break;
    default:
        return -1;
    }
    return tim_freq;
}

uint8_t get_tim0_mode(void)
{
    return (((TCCR0A & TIM0_WGM10_MASK) >> TIM0_WGM10_POS)
            | (((TCCR0B & (1 << WGM02)) >> WGM02 ) << 2));
}

uint32_t get_tim0_freq(void)
{
    uint8_t mode = get_tim0_mode();
    uint32_t cnt_freq = get_tim0_cnt_freq();
    return get_8bit_tim_freq(cnt_freq, mode, OCR0A);
}

void set_tim0_prescaler(uint8_t prescaler)
{
    prescaler &= TIM_CS_MASK;
    TCCR0B |= (prescaler << CS00);
    GTCCR |= _BV(PSRSYNC);
}

void enable_tim0_irq(uint8_t irq_mask)
{
    TIMSK0 |= (irq_mask);
}

void disable_tim0_irq(uint8_t irq_mask)
{
    TIMSK0 &= ~(irq_mask);
}

ISR(TIMER0_OVF_vect)
{
    static uint32_t cnt = 0;
    if (cnt++ >= tim0_ov_max_cnt) {

        board_pin_toggle(13);
        cnt = 0;
    }
}

void get_tim0_status(slip_payload_t * msg)
{
    typedef struct {
        uint32_t tim_freq;
        uint32_t cnt_freq;
        uint32_t tim8_freq;
        uint8_t mode;
        uint8_t tccr0a;
        uint8_t tccr0b;
        uint8_t timsk0;
        uint8_t gtccr;
        uint8_t tcnt0;
    } tim_status_t;
    tim_status_t status;
    status.tim_freq = get_tim0_freq();
    status.cnt_freq = get_tim0_cnt_freq();
    status.tim8_freq = get_8bit_tim_freq(get_tim0_cnt_freq(), get_tim0_mode(), OCR0A);
    status.mode = get_tim0_mode();
    status.tccr0a = TCCR0A;
    status.tccr0b = TCCR0B;
    status.timsk0 = TIMSK0;
    status.gtccr = GTCCR;
    status.tcnt0 = TCNT0;
    memcpy(msg->data, &status, sizeof(status));
    msg->len = sizeof(status);
#if 0
    ((uint32_t *)msg->data)[msg->len] = get_tim0_freq();
    msg->len += sizeof(uint32_t);
    msg->data[msg->len] = get_tim0_cnt_freq();
    msg->len += sizeof(uint32_t);
    msg->data[msg->len++] = TCCR0A;
    msg->data[msg->len++] = TCCR0B;
    msg->data[msg->len++] = TIMSK0;
    msg->data[msg->len++] = GTCCR;
    msg->data[msg->len++] = TCNT0;
    ((uint32_t *)msg->data)[msg->len] = 0xCAFECAFE;
    msg->len += sizeof(uint32_t);
#endif
}
