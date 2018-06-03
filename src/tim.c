#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "tim.h"
#include "gpio.h"
#include "messages.h"
#include "sched.h"

static uint32_t tim0_ov_cnt;
static uint32_t tim2_ov_cnt;

uint32_t get_tim0_ov_cnt(void)
{
    return tim0_ov_cnt;
}
void set_tim0_ov_cnt(uint32_t val)
{
    tim0_ov_cnt = val;
    enable_tim0_irq(TOIE0);
}

uint32_t get_tim2_ov_cnt(void)
{
    return tim2_ov_cnt;
}

void set_tim2_ov_cnt(uint32_t val)
{
    tim2_ov_cnt = val;
    enable_tim2_irq(TOIE2);
}


void enable_tim0(void)
{
    PRR &= ~_BV(PRTIM0);
}

void enable_tim2(void)
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

uint32_t get_tim2_shift_prescaler(void)
{
    uint8_t cs = (TCCR2B & TIM2_CS_MASK) >> TIM2_CS_POS;
    uint32_t prescaler;
    switch(cs)
    {
    default:
    case TIM2_NO_CLK:
        prescaler = 0;
        break;
    case TIM2_NO_PRESCALING:
        prescaler = 1;
        break;
    case TIM2_PRESCALING_DIV8:
        prescaler = 3;
        break;
    case TIM2_PRESCALING_DIV32:
        prescaler = 5;
        break;
    case TIM2_PRESCALING_DIV64:
        prescaler = 6;
        break;
    case TIM2_PRESCALING_DIV128:
        prescaler = 7;
        break;
    case TIM2_PRESCALING_DIV256:
        prescaler = 8;
        break;
    case TIM2_PRESCALING_DIV1024:
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

uint32_t get_tim2_cnt_freq(void)
{
    uint8_t cs = (TCCR2B & TIM_CS_MASK) >> TIM_CS_POS;
    if (cs != TIM_NO_CLK)
    {
        return F_CPU >> get_tim2_shift_prescaler();
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

void set_tim0_com_cha(uint8_t com)
{
    TCCR0A &= ~((1 << COM0A0) | (1 << COM0A1));
    TCCR0A |= ((com & 0x3) << COM0A0);
}

void set_tim0_ocra(uint8_t val)
{
    OCR0A = val;
}

void set_tim0_mode(uint8_t mode)
{
    TCCR0A &= ~TIM_WGM_MASK;
    TCCR0A |= ((mode & 0x3) << WGM00);
    TCCR0B &= ~(1 << WGM02);
    TCCR0B |= ((mode & 0x4) >> 2) << WGM02;
}

void set_tim2_mode(uint8_t mode)
{
    TCCR2A &= ~TIM_WGM_MASK;
    TCCR2A |= ((mode & 0x3) << WGM20);
    TCCR2B &= ~(1 << WGM22);
    TCCR2B |= ((mode & 0x4) >> 2) << WGM22;
}

uint8_t get_tim0_mode(void)
{
    return (((TCCR0A & TIM_WGM_MASK) >> TIM_WGM_POS)
            | (((TCCR0B & (1 << WGM02)) >> WGM02 ) << 2));
}

uint8_t get_tim2_mode(void)
{
    return (((TCCR2A & TIM_WGM_MASK) >> TIM_WGM_POS)
            | (((TCCR2B & (1 << WGM22)) >> WGM22 ) << 2));
}

uint32_t get_tim0_freq(void)
{
    uint8_t mode = get_tim0_mode();
    uint32_t cnt_freq = get_tim0_cnt_freq();
    return get_8bit_tim_freq(cnt_freq, mode, OCR0A);
}

uint32_t get_tim2_freq(void)
{
    uint8_t mode = get_tim2_mode();
    uint32_t cnt_freq = get_tim2_cnt_freq();
    return get_8bit_tim_freq(cnt_freq, mode, OCR2A);
}

void set_tim0_prescaler(uint8_t prescaler)
{
    prescaler &= TIM_CS_MASK;
    TCCR0B &= ~TIM_CS_MASK;
    TCCR0B |= (prescaler << CS00);
    GTCCR |= _BV(PSRSYNC);
}

void set_tim2_prescaler(uint8_t prescaler)
{
    prescaler &= TIM_CS_MASK;
    TCCR2B &= ~TIM_CS_MASK;
    TCCR2B |= (prescaler << CS20);
    GTCCR |= _BV(PSRSYNC);
}

void enable_tim0_irq(uint8_t irq_mask)
{
    TIMSK0 |= (irq_mask);
}

void enable_tim2_irq(uint8_t irq_mask)
{
    TIMSK2 |= (irq_mask);
}
void disable_tim0_irq(uint8_t irq_mask)
{
    TIMSK0 &= ~(irq_mask);
}

void disable_tim2_irq(uint8_t irq_mask)
{
    TIMSK2 &= ~(irq_mask);
}

ISR(TIMER0_OVF_vect)
{
    if (tim0_ov_cnt-- == 0)
    {
        /* Disable irq */
        disable_tim0_irq(TOIE0);
    }
}

ISR(TIMER2_OVF_vect)
{
    if (tim2_ov_cnt) {
        tim2_ov_cnt--;
    }
}

ISR(TIMER2_COMPA_vect)
{
    for (uint8_t i = 0; i < SCHED_MAX_CNT; ++i)
    {
        if (sched_cnts[i])
        {
            /* Decrease registered counter */
            (*sched_cnts[i])--;
            if (!*sched_cnts[i])
            {
                /* Deregister counter which have readched 0 */
                sched_cnts[i] = 0;
            }
        }
    }
}

tim_cfg_t comput_tim0_freq_cfg(uint32_t freq)
{
    tim_cfg_t tim_cfg;
    tim_cfg.shift_prescaler = 0;
    if (freq <= TIM_CTC_FREQ(256, 255))
    {
        tim_cfg.shift_prescaler = 10;
    } else if (freq <= TIM_CTC_FREQ(64, 255))
    {
        tim_cfg.shift_prescaler = 8;
    } else if (freq <= TIM_CTC_FREQ(8, 255))
    {
        tim_cfg.shift_prescaler = 6;
    } else if (freq <= TIM_CTC_FREQ(1, 255))
    {
        tim_cfg.shift_prescaler = 3;
    } else if (freq <= TIM_CTC_FREQ(1, 0))
    {
        tim_cfg.shift_prescaler = 0;
    } else
    {
        tim_cfg.shift_prescaler = -1;
    }

    tim_cfg.top = TIM_CTC_TOP((1 << tim_cfg.shift_prescaler), freq);
    return tim_cfg;
}

void set_tim0_cfg(tim_cfg_t tim_cfg)
{
    uint8_t cs = 0;
    switch(tim_cfg.shift_prescaler)
    {
    case 1:
        cs = TIM_NO_PRESCALING;
        break;
    case 3:
        cs = TIM_PRESCALING_DIV8;
        break;
    case 6:
        cs = TIM_PRESCALING_DIV64;
        break;
    case 8:
        cs = TIM_PRESCALING_DIV256;
        break;
    case 10:
        cs = TIM_PRESCALING_DIV1024;
        break;
    default:
        cs = TIM_NO_CLK;
        break;
    }
    set_tim0_prescaler(cs);
    OCR0A = tim_cfg.top;
}

tim_cfg_t comput_tim2_freq_cfg(uint32_t freq)
{
    tim_cfg_t tim_cfg;
    tim_cfg.shift_prescaler = 0;
    if (freq <= TIM_CTC_FREQ(256, 255))
    {
        tim_cfg.shift_prescaler = 10;
    } else if (freq <= TIM_CTC_FREQ(128, 255))
    {
        tim_cfg.shift_prescaler = 8;
    } else if (freq <= TIM_CTC_FREQ(64, 255))
    {
        tim_cfg.shift_prescaler = 7;
    } else if (freq <= TIM_CTC_FREQ(32, 255))
    {
        tim_cfg.shift_prescaler = 6;
    } else if (freq <= TIM_CTC_FREQ(8, 255))
    {
        tim_cfg.shift_prescaler = 5;
    } else if (freq <= TIM_CTC_FREQ(1, 255))
    {
        tim_cfg.shift_prescaler = 3;
    } else if (freq <= TIM_CTC_FREQ(1, 0))
    {
        tim_cfg.shift_prescaler = 0;
    } else
    {
        tim_cfg.shift_prescaler = -1;
    }

    tim_cfg.top = TIM_CTC_TOP((1 << tim_cfg.shift_prescaler), freq);
    return tim_cfg;
}

void set_tim2_cfg(tim_cfg_t tim_cfg)
{
    uint8_t cs = 0;
    switch(tim_cfg.shift_prescaler)
    {
    case 1:
        cs = TIM2_NO_PRESCALING;
        break;
    case 3:
        cs = TIM2_PRESCALING_DIV8;
        break;
    case 5:
        cs = TIM2_PRESCALING_DIV32;
        break;
    case 6:
        cs = TIM2_PRESCALING_DIV64;
        break;
    case 7:
        cs = TIM2_PRESCALING_DIV128;
        break;
    case 8:
        cs = TIM2_PRESCALING_DIV256;
        break;
    case 10:
        cs = TIM2_PRESCALING_DIV1024;
        break;
    default:
        cs = TIM2_NO_CLK;
        break;
    }
    set_tim2_prescaler(cs);
    OCR2A = tim_cfg.top;
}

void pid_req_tim_status(slip_payload_t * msg)
{
    msg->pid |= 0x80;
    req_tim_status_t * req = (req_tim_status_t *) msg->data;
    tim_status_t status;
    switch(req->timer_id)
    {
    case 0:
        status.tim_freq = get_tim0_freq();
        status.cnt_freq = get_tim0_cnt_freq();
        status.cnt = tim0_ov_cnt;
        status.mode = get_tim0_mode();
        status.tccra = TCCR0A;
        status.tccrb = TCCR0B;
        status.timsk = TIMSK0;
        status.gtccr = GTCCR;
        status.tcnt = TCNT0;
        break;
    case 2:
        status.tim_freq = get_tim2_freq();
        status.cnt_freq = get_tim2_cnt_freq();
        status.cnt = tim2_ov_cnt;
        status.mode = get_tim2_mode();
        status.tccra = TCCR2A;
        status.tccrb = TCCR2B;
        status.timsk = TIMSK2;
        status.gtccr = GTCCR;
        status.tcnt = TCNT2;
        break;
    }
    memcpy(msg->data, &status, sizeof(status));
    msg->len = sizeof(status);
}
