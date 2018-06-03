#ifndef __TIM_H__
#define __TIM_H__
#include <stdint.h>
#include "slip_payload.h"

/*
 * COMnx (Compare Output Mode)
 */
/* Non Pwm */
#define TIM0_NON_PWM_OC_DISCONNECTED 0x0
#define TIM0_NON_PWM_TOGGLE_OC_MATCH 0x1
#define TIM0_NON_PWM_CLEAR_OC_MATCH  0x2
#define TIM0_NON_PWM_SET_OC_MATCH    0x3

/* Fast Pwm (single slope) */
#define TIM0_FAST_PWM_OC_DISCONNECTED            0x0
#define TIM0_FAST_PWM_TOGGLE_OC_MATCH            0x1
#define TIM0_FAST_PWM_CLEAR_OC_MATCH_SET_BOTTOM  0x2
#define TIM0_FAST_PWM_SET_OC_MATCH_CLEAR_BOTTOM  0x3

/* Phase correct Pwm (dual slope) */
#define TIM0_PWM_OC_DISCONNECTED            0x0
#define TIM0_PWM_TOGGLE_OC_MATCH            0x1 /* Toggle OCnx on match */
#define TIM0_PWM_CLEAR_UP_OC_MATCH          0x2 /* Clear OCnx on match when up-counting,
                                              set when downcouting */
#define TIM0_PWM_SET_UP_OC_MATCH            0x3 /* Set OCnx on match when up-counting,
                                              clear when down-counting */
/*
 * WGM (Waveform Generation Mode)
 * bit 2 is in TCCRnB register
 * bit 1:0 are in TCCRnA register
 */
#define TIM0_WGM_NORMAL            0x0 /* TOP = 0xFF */
#define TIM0_WGM_PWM               0x1 /* TOP = 0xFF */
#define TIM0_WGM_CTC               0x2 /* TOP = 0CRnx */
#define TIM0_WGM_FAST_PWM          0x3 /* TOP = 0xFF */
#define TIM0_WGM_PWM_TOP_OCR       0x5 /* TOP = 0CRnx */
#define TIM0_WGM_FAST_PWM_TOP_OCR  0x7 /* TOP = OCRnx */
#define TIM_WGM_POS  WGM00
#define TIM_WGM_MASK ((1 << WGM00) | (1 << WGM01))

/* CSn Clock Source */
#define TIM_NO_CLK               0x0
#define TIM_NO_PRESCALING        0x1
#define TIM_PRESCALING_DIV8      0x2
#define TIM_PRESCALING_DIV64     0x3
#define TIM_PRESCALING_DIV256    0x4
#define TIM_PRESCALING_DIV1024   0x5
#define TIM_EXT_CLK_FALLING_EDGE 0x6
#define TIM_EXT_CLK_RISING_EDGE  0x7
#define TIM_CS_POS  CS00
#define TIM_CS_MASK ((1 << CS00) | (1 << CS01) | (1 << CS02))

/* Clock source for timer 2 */
#define TIM2_NO_CLK             0x0
#define TIM2_NO_PRESCALING      0x1
#define TIM2_PRESCALING_DIV8    0x2
#define TIM2_PRESCALING_DIV32   0x3
#define TIM2_PRESCALING_DIV64   0x4
#define TIM2_PRESCALING_DIV128  0x5
#define TIM2_PRESCALING_DIV256  0x6
#define TIM2_PRESCALING_DIV1024 0x7
#define TIM2_CS_POS CS20
#define TIM2_CS_MASK ((1 << CS20) | (1 << CS21) | (1 << CS22))

#define TIM_CTC_FREQ(n, top) (F_CPU/((uint32_t)n*(1+top)))
#define TIM_CTC_TOP(n, freq) (F_CPU/(n*freq) -1)

typedef struct {
    int8_t shift_prescaler;
    uint8_t top;
} tim_cfg_t;

uint32_t get_tim0_ov_cnt(void);
uint32_t get_tim2_ov_cnt(void);
void set_tim0_ov_cnt(uint32_t val);
void set_tim2_ov_cnt(uint32_t val);

void enable_tim0(void);
void enable_tim2(void);
/* Return frequency at which timer counter is incremented */
uint32_t get_tim0_cnt_freq(void);
uint32_t get_tim2_cnt_freq(void);
/* Return frequency at which timer overflows */
uint32_t get_tim0_freq(void);
uint32_t get_tim2_freq(void);

void set_tim0_prescaler(uint8_t prescaler);
void set_tim2_prescaler(uint8_t prescaler);

void enable_tim0_irq(uint8_t irq_mask);
void enable_tim2_irq(uint8_t irq_mask);
void disable_tim0_irq(uint8_t irq_mask);
void disable_tim2_irq(uint8_t irq_mask);

void set_tim0_com_cha(uint8_t com);
void set_tim0_ocra(uint8_t val);

tim_cfg_t comput_tim0_freq_cfg(uint32_t freq);
void set_tim0_cfg(tim_cfg_t tim_cfg);
tim_cfg_t comput_tim2_freq_cfg(uint32_t freq);
void set_tim2_cfg(tim_cfg_t tim_cfg);

void set_tim0_mode(uint8_t mode);
void set_tim2_mode(uint8_t mode);
uint8_t get_tim0_mode(void);
uint8_t get_tim2_mode(void);

void pid_req_tim_status(slip_payload_t * msg);
#endif // __TIM_H__
