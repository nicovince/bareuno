#ifndef __SCHED_H__
#define __SCHED_H__
#include <stdint.h>
#define SCHED_MAX_CNT 16
extern uint32_t *sched_cnts[SCHED_MAX_CNT];
/**
 * @brief Initialize sched_cnts
 */
void sched_init(void);
/**
 * @brief Register a counter which will be decreased each timer tick
 *
 * @param cnt Pointer to counter
 *
 * @return 0 on success, -1 if counter could not be registered
 */
int8_t sched_register_cnt(uint32_t *cnt);
#endif // __SCHED_H__
