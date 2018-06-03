#include <avr/io.h>
#include <string.h>
#include "board.h"
#include <avr/interrupt.h>
#include "usart.h"
#include "slip.h"
#include "slip_payload.h"
#include "gpio.h"
#include "tim.h"
#include "sched.h"
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

void send_slip_msg(slip_payload_t * msg)
{
    /* Byte to send, may be escaped */
    uint8_t encoded_byte[2];
    /* Size of encoded byte (1 or 2) */
    uint8_t sz;
    /* Update CRC in msg */
    msg->crc = compute_crc(msg);
    /* Start */
    encoded_byte[0] = SLIP_END;
    usart_write(encoded_byte, 1);
    /* PID */
    sz = slip_encode_byte(msg->pid, encoded_byte);
    usart_write(encoded_byte, sz);
    /* SEQ */
    sz = slip_encode_byte(msg->seq, encoded_byte);
    usart_write(encoded_byte, sz);
    /* LEN */
    sz = slip_encode_byte(msg->len, encoded_byte);
    usart_write(encoded_byte, sz);

    /* Datas */
    for (int i = 0; i < msg->len; ++i)
    {
        sz = slip_encode_byte(msg->data[i], encoded_byte);
        usart_write(encoded_byte, sz);
    }
    /* CRC LSB then MSB */
    sz = slip_encode_byte((uint8_t)(msg->crc & 0xFF), encoded_byte);
    usart_write(encoded_byte, sz);
    sz = slip_encode_byte((uint8_t)((msg->crc & 0xFF00) >> 8), encoded_byte);
    usart_write(encoded_byte, sz);

    /* End */
    encoded_byte[0] = SLIP_END;
    usart_write(encoded_byte, 1);
}

int main(void)
{
    uint8_t freq_idx = 0;
    sei();
    uint32_t tick_1hz;
    sched_init();
    board_pin_set_output(13);
    int16_t slip_size;

    /* setup timer */
    enable_tim2();
    enable_tim2_irq((1 << OCIE2A));
    set_tim2_mode(TIM0_WGM_CTC);
    set_tim2_prescaler(TIM2_PRESCALING_DIV1024);
    set_tim2_cfg(comput_tim2_freq_cfg(1000));

    enable_tim0();
    enable_tim0_irq((1 << TOIE0));
    set_tim0_mode(TIM0_WGM_CTC);
    set_tim0_com_cha(TIM0_NON_PWM_TOGGLE_OC_MATCH);
    set_tim0_cfg(comput_tim0_freq_cfg(ode_a_la_joie[freq_idx]));



    setup_usart(BAUD_9600);
    init_slip_decoder(&slip, slip_buffer, 128);

    set_tim0_ov_cnt(0);
    tick_1hz = get_tim2_freq();
    sched_register_cnt(&tick_1hz);
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
        }
        if (!tick_1hz)
        {
            tick_1hz = get_tim2_freq();
            sched_register_cnt(&tick_1hz);
            board_pin_toggle(13);

        }


        size_t sz = usart_read(&c, 1);
        if (sz != 0) {
            slip_size = slip_decode(&slip, c);
            if (slip_size == 0) {
                /* Reset empty message */
                reset_slip_decoder(&slip);
            } else if (slip_size > 0) {
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
