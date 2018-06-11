#include "melody.h"
#include "tim.h"
#include "sched.h"
#include "messages.h"
static uint32_t get_note_freq(note_name_t note_name, uint8_t octave);

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

void play_note(note_t * note, uint32_t *cnt)
{
    uint32_t freq = get_note_freq(note->name, note->octave);
    *cnt = get_note_length(note->length, 60 /*tempo*/);
    sched_register_cnt(cnt);
    set_tim0_cfg(comput_tim0_freq_cfg(freq));
}
