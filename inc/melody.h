#ifndef __MELOLDY_H
#define __MELOLDY_H
#include "messages.h"
#include "slip_payload.h"

void play_note(note_t * note, uint32_t *cnt);
void pid_note(slip_payload_t *msg);
void melody_init(void);

#endif // __MELOLDY_H
