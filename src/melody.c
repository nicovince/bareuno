#include "melody.h"
#include "tim.h"
#include "sched.h"
#include "messages.h"
#include "fifo.h"
#include <string.h>

#define NUM_NOTES_MAX 4

static fifo_t notes_fifo;
static note_t notes_array[NUM_NOTES_MAX];
static melody_src_t melody_src = NONE;
static uint8_t tempo = 60;

static uint32_t get_note_freq(note_name_t note_name, uint8_t octave);
static uint32_t get_note_length(note_length_t note_length, uint16_t tempo);

static uint32_t get_note_freq(note_name_t note_name, uint8_t octave)
{
    uint32_t freq0;
    switch(note_name)
    {
    case DO:
        freq0 = FREQ_DO;
        break;
    case RE_B:
        freq0 = FREQ_RE_B;
        break;
    case RE:
        freq0 = FREQ_RE;
        break;
    case MI_B:
        freq0 = FREQ_MI_B;
        break;
    case MI:
        freq0 = FREQ_MI;
        break;
    case FA:
        freq0 = FREQ_FA;
        break;
    case SOL_B:
        freq0 = FREQ_SOL_B;
        break;
    case SOL:
        freq0 = FREQ_SOL;
        break;
    case LA_B:
        freq0 = FREQ_LA_B;
        break;
    case LA:
        freq0 = FREQ_LA;
        break;
    case SI_B:
        freq0 = FREQ_SI_B;
        break;
    case SI:
        freq0 = FREQ_SI;
        break;
    default:
        freq0 = 0;
    }

    return freq0 * (1 << octave)/100;
}

/* Return note length in ms */
static uint32_t get_note_length(note_length_t note_length, uint16_t tempo)
{
    /*
     * tempo gives the number of noire per minute
     * Compute the length of the shortest note
     * */
    const uint32_t black_len_ms = 1000*tempo/60;
    const uint32_t shortest_len= black_len_ms/(1 << (note_length_END -1 - NOIRE));
    return shortest_len * (1 << (note_length_END -1 - note_length));
}

void melody_init(void)
{
    init_fifo(&notes_fifo, sizeof(note_t), NUM_NOTES_MAX, notes_array);
}

melody_src_t melody_get_src(void)
{
    return melody_src;
}

void play_note(note_t * note, uint32_t *cnt)
{
    uint32_t freq = get_note_freq(note->name, note->octave);
    *cnt = get_note_length(note->length, tempo);
    sched_register_cnt(cnt);
    set_tim0_cfg(comput_tim0_freq_cfg(freq));
}

uint8_t melody_fifo_empty(void)
{
    return is_empty(&notes_fifo);
}

note_t melody_fifo_get_note(void)
{
    note_t note;
    pop(&notes_fifo, &note);
    return note;
}

void pid_note(slip_payload_t *msg)
{
    ack_note_t ack;
    if (!is_full(&notes_fifo)) {
        push(&notes_fifo, msg->data);
        ack.status = 1;
    } else {
        ack.status = 0;
    }
    msg->pid |= 0x80;
    msg->len = sizeof(ack_note_t);
    memcpy(&msg->data, &ack, sizeof(ack_note_t));
}

void pid_melody_conf(slip_payload_t *msg)
{
    req_melody_conf_t *req = (req_melody_conf_t *) msg->data;
    melody_src = req->source;
    tempo = req->tempo;
    msg->pid |= 0x80;
    msg->len = 0;
}
