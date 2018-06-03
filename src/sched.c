#include "sched.h"

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
