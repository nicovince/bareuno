#include "sched.h"
#include "slip_payload.h"
#include "messages.h"
#include <string.h>
#include <avr/interrupt.h>

volatile uint32_t sched_tick_cnt;
uint32_t *sched_cnts[SCHED_MAX_CNT];

void sched_init(void)
{
    for (uint8_t i = 0; i < SCHED_MAX_CNT; ++i)
    {
        sched_cnts[i] = 0;
    }
}

int8_t sched_register_cnt(uint32_t *cnt)
{
    for (uint8_t i = 0; i < SCHED_MAX_CNT; ++i)
    {
        if (sched_cnts[i] == 0) {
            sched_cnts[i] = cnt;
            return 0;
        }
    }
    return -1;
}

void sched_update(void)
{
    for (uint8_t i = 0; i < SCHED_MAX_CNT; ++i)
    {
        if (sched_cnts[i])
        {
            /* Decrease registered counter */
            if (*sched_cnts[i] > sched_tick_cnt)
            {
                (*sched_cnts[i]) -= sched_tick_cnt;
            }
            else
            {
                (*sched_cnts[i]) = 0;
                sched_cnts[i] = 0;
            }
        }
    }
    sched_tick_cnt = 0;
}

void pid_req_sched(slip_payload_t *msg)
{
    ack_sched_t ack;
    for (int i = 0; i < SCHED_MAX_CNT; ++i)
    {
        ack.counters[i] = *sched_cnts[i];
    }
    msg->len = sizeof(ack_sched_t);
    msg->pid |= 0x80;
    memcpy(&msg->data, &ack, sizeof(ack_sched_t));
}
