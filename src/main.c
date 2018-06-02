#include <avr/io.h>
#include <string.h>
#include "board.h"
#include <avr/interrupt.h>
#include "usart.h"
#include "slip.h"
#include "slip_payload.h"
#include "gpio.h"
#include "tim.h"
#include "messages.h"

#define DO     0
#define DO_B   1
#define RE     2
#define RE_B   3
#define MI     4
#define FA     5
#define FA_B   6
#define SOL    7
#define SOL_B  8
#define LA     9
#define LA_B  10
#define SI    11

uint32_t freq[] = {
    262, /* do */
    277, /* do# */
    294, /* re */
    311, /* re# */
    330, /* mi */
    349, /* fa */
    370, /* fa# */
    392, /* sol */
    415, /* sol# */
    440, /* la */
    466, /* la# */
    494 /* si */
};
uint32_t ode_a_la_joie[] = {
    MI, MI, FA, SOL,
    SOL, FA, MI, RE,
    DO, DO, RE, MI,
    MI, RE, RE,
    //
    MI, MI, FA, SOL,
    SOL, FA, MI, RE,
    DO, DO, RE, MI,
    RE, DO, DO,
    //
    RE, RE, MI, DO,
    RE, MI, FA, MI, DO,
    RE, MI, FA, MI, RE,
    DO, RE, DO, MI,
    //
    MI, MI, FA, SOL,
    SOL, FA, MI, RE,
    DO, DO, RE, MI,
    RE, DO, DO
};


slip_decoder_t slip;
uint8_t slip_buffer[128];
uint8_t slip_encoded[256];
uint8_t raw_slip_payload[MAX_SLIP_PAYLOAD];
slip_payload_t slip_payload;

void default_slip_callback(slip_payload_t *msg)
{
    msg->pid = SLIP_ERROR_ID;
    msg->len = sizeof(slip_error_t);
    slip_error_t ack;
    ack.pid = msg->pid;
    ack.error = SLIP_UNKNOWN_ID;
    memcpy(msg->data, &ack, sizeof(slip_error_t));
}

void send_pid(uint8_t pid)
{
    static uint8_t cnt = 0;
    slip_payload.pid = pid;
    slip_payload.seq = cnt++;
    slip_payload.len = 0;
    uint16_t slip_payload_size = pack_slip_payload(&slip_payload, raw_slip_payload);
    uint8_t tx_slip_size = slip_encode(raw_slip_payload, slip_encoded, slip_payload_size);
    usart_write(slip_encoded, tx_slip_size);
}

void send_slip_msg(slip_payload_t * msg)
{
    uint16_t slip_payload_size = pack_slip_payload(msg, raw_slip_payload);
    uint8_t tx_slip_size = slip_encode(raw_slip_payload, slip_encoded, slip_payload_size);
    usart_write(slip_encoded, tx_slip_size);
}

int main(void)
{
    uint8_t freq_idx = 0;
    sei();
    board_pin_set_output(13);
    board_pin_set_output(YELLOW_LED);
    board_pin_clear(YELLOW_LED);
    board_pin_set_output(GREEN_LED);
    board_pin_set(GREEN_LED);
    int16_t slip_size;

    /* setup timer */
    enable_tim2();
    enable_tim2_irq((1 << TOIE2));
    set_tim2_mode(TIM0_WGM_NORMAL);
    set_tim2_prescaler(TIM2_PRESCALING_DIV1024);

    enable_tim0();
    enable_tim0_irq((1 << TOIE0));
    set_tim0_mode(TIM0_WGM_CTC);
    set_tim0_com_cha(TIM0_NON_PWM_TOGGLE_OC_MATCH);
    set_tim0_cfg(comput_tim0_freq_cfg(ode_a_la_joie[freq_idx]));

    board_pin_clear(13);


    setup_usart(BAUD_9600);
    board_pin_clear(13);
    init_slip_decoder(&slip, slip_buffer, 128);

    set_tim0_ov_cnt(0);
    set_tim2_ov_cnt(get_tim2_freq());
    uint8_t c;
    while(1)
    {

        if (get_tim2_ov_cnt() == 0)
        {
            freq_idx = (freq_idx + 1) % (sizeof(ode_a_la_joie)/sizeof(ode_a_la_joie[0]));
            set_tim0_cfg(comput_tim0_freq_cfg(freq[ode_a_la_joie[freq_idx]]));
            /* Rearm 1Hz event */
            set_tim2_ov_cnt(get_tim2_freq());
            /* Toggle led */
            board_pin_toggle(13);
        }
        /* Update led based on slip state */
        board_pin_clear(GREEN_LED);
        board_pin_clear(GREEN_LED);
        switch(slip.state)
        {
        case SLIP_WAIT_END:
            board_pin_set(YELLOW_LED);
            board_pin_clear(GREEN_LED);
            break;
        case SLIP_STORE_INCOMING:
            board_pin_set(GREEN_LED);
            board_pin_clear(YELLOW_LED);
            break;
        default:
            board_pin_clear(GREEN_LED);
            board_pin_clear(YELLOW_LED);
        }


        size_t sz = usart_read(&c, 1);
        if (sz != 0) {
            slip_size = slip_decode(&slip, c);
            if (slip_size == 0) {
                /* Reset empty message */
                reset_slip_decoder(&slip);
            } else if (slip_size > 0) {
                board_pin_set(GREEN_LED);
                int8_t crc_ok = unpack_slip_payload(slip.buf, &slip_payload);
                /* TODO Check length of payload data is lesser than slip message length */
                if (crc_ok)
                {
                    /* Send request reply */
                    switch(slip_payload.pid)
                    {
                    case REQ_TIM_STATUS_ID:
                        pid_req_tim_status(&slip_payload);
                        break;
                    default:
                        default_slip_callback(&slip_payload);
                        break;
                    }
                    send_slip_msg(&slip_payload);
                }

                reset_slip_decoder(&slip);
            }
        }
    }
    return 0;
}
