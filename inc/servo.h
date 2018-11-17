#ifndef __SERVO_H__
#define __SERVO_H__
#include "tim.h"
void servo_init(void);
void pid_req_servo_conf(slip_payload_t * msg);
#endif // __SERVO_H__
