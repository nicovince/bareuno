#!/usr/bin/env python3

from genmsg.slip import *
from genmsg.messages import *
import argparse
import serial
import time

def do_si(fd):
    conf = ReqMelodyConf(MelodySrc.FIFO.value)
    msg = slip_transaction(fd, SlipPayload(conf.msg_id, 0, conf.pack()), True)
    print(msg)

    for n in NoteName:
        note = Note(n.value, 2, NoteLength.NOIRE.value)
        msg = slip_transaction(fd, SlipPayload(note.msg_id, 0, note.pack()), True)
        while msg[0].data.status == 0:
            msg = slip_transaction(fd, SlipPayload(note.msg_id, 0, note.pack()), True)
            time.sleep(0.5)

    print(msg)


def main():
    parser = argparse.ArgumentParser(description="Send Slip message",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("slip_interface", type=str, help="Slip interface")
    # Parse args
    args = parser.parse_args()

    serial_fd = open(args.slip_interface, "rb")
    serial_fd = serial.Serial(port=args.slip_interface, baudrate=9600)

    do_si(serial_fd)

if __name__ == "__main__":
    main()
