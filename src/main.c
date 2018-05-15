#include <avr/io.h>
#include "board.h"
#include <avr/interrupt.h>
#include "usart.h"
#include "slip.h"
#include "slip_payload.h"
#include "gpio.h"
#include "tim.h"

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
    board_pin_set_output(6);



    setup_usart(BAUD_9600);
    init_slip_decoder(&slip, slip_buffer, 128);

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
        size_t sz = usart_read(&c, 1);
        if (sz != 0) {
            slip_size = slip_decode(&slip, c);
            if (slip_size == 0) {
                /* Reset empty message */
                reset_slip_decoder(&slip);
                send_pid(0);
            } else if (slip_size > 0) {
                int8_t crc_ok = unpack_slip_payload(slip.buf, &slip_payload);
                /* TODO Check length of payload data is lesser than slip message length */
                if (!crc_ok) {
                    send_pid(slip_size);
                } else if (slip_payload.pid == 6) {
                    send_pid(0xFF);
                    board_pin_toggle(13);
                } else if (slip_payload.pid == 1) {
                    get_tim0_status(&slip_payload);
                    send_slip_msg(&slip_payload);
                } else if (slip_payload.pid == 2) {
                    slip_payload.pid |= 0x80;
                    slip_payload.len = 0;
                    send_slip_msg(&slip_payload);
                }

                reset_slip_decoder(&slip);
            }
        }
    }
    return 0;
}
