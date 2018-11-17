#include "messages.h"
#include "servo.h"
#include "gpio.h"

void servo_init(void)
{
    set_tim0_prescaler(TIM_PRESCALING_DIV256);
    /* OC0A */
    board_pin_set_output(6);
    /* OC0B */
    board_pin_set_output(5);
    enable_tim0();
    disable_tim0_irq(0x7);
    set_tim0_mode(TIM0_WGM_PWM);
    set_tim0_com_cha(TIM0_PWM_CLEAR_UP_OC_MATCH);
    set_tim0_com_chb(TIM0_PWM_CLEAR_UP_OC_MATCH);
    set_tim0_ocra(0x7F);
    set_tim0_ocrb(0x1);
}


void pid_req_servo_conf(slip_payload_t * msg)
{
    req_servo_conf_t *req = (req_servo_conf_t *) msg->data;
    msg->pid |= 0x80;
    set_tim0_ocra(req->servo0);
    set_tim0_ocrb(req->servo1);
    msg->len = 0;
}

