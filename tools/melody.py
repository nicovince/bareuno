#!/usr/bin/env python3

from genmsg.slip import *
from genmsg.messages import *
import argparse
import serial
import time

def msg_to_slip_payload(msg):
    return SlipPayload(msg.msg_id, 0, msg.pack())

def got(fd):
    conf = ReqMelodyConf(MelodySrc.FIFO, 168)
    msg = slip_transaction(fd, SlipPayload(conf.msg_id, 0, conf.pack()), False)
    got_notes = list()
    octave = 3
    for i in range(4):
        got_notes.append(Note(NoteName.SOL, octave, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.DO, octave, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.MI_B, octave, NoteLength.CROCHE))
        got_notes.append(Note(NoteName.FA, octave, NoteLength.CROCHE))

    for i in range(4):
        got_notes.append(Note(NoteName.SOL, octave, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.DO, octave, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.MI, octave, NoteLength.CROCHE))
        got_notes.append(Note(NoteName.FA, octave, NoteLength.CROCHE))

    got_notes.append(Note(NoteName.SOL, octave, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.SOL, octave, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.DO, octave, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.DO, octave, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.MI_B, octave, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.FA, octave, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.SOL, octave, NoteLength.BLANCHE))

    got_notes.append(Note(NoteName.DO, octave, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.MI_B, octave, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.FA, octave, NoteLength.CROCHE))

    for i in range(3):
        got_notes.append(Note(NoteName.RE, octave, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.SOL, octave-1, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.SI_B, octave-1, NoteLength.CROCHE))
        got_notes.append(Note(NoteName.DO, octave, NoteLength.CROCHE))

    got_notes.append(Note(NoteName.RE, octave, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.SOL, octave-1, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.SI_B, octave-1, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.FA, octave, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.FA, octave, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.DO, octave, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.DO, octave, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.MI_B, octave, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.RE, octave, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.FA, octave, NoteLength.BLANCHE))

    got_notes.append(Note(NoteName.SI_B, octave-1, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.SI_B, octave-1, NoteLength.NOIRE))

    for i in range(3):
        got_notes.append(Note(NoteName.MI_B, octave, NoteLength.CROCHE))
        got_notes.append(Note(NoteName.RE, octave, NoteLength.CROCHE))
        got_notes.append(Note(NoteName.DO, octave, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.LA_B, octave-1, NoteLength.CROCHE))
        got_notes.append(Note(NoteName.SI_B, octave-1, NoteLength.CROCHE))

    got_notes.append(Note(NoteName.DO, octave, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.FA, octave-1, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.DO, octave, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.SOL, octave, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.SOL, octave, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.DO, octave, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.DO, octave, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.MI_B, octave, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.FA, octave, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.SOL, octave, NoteLength.BLANCHE))

    got_notes.append(Note(NoteName.DO, octave, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.MI_B, octave, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.FA, octave, NoteLength.CROCHE))

    for i in range(3):
        got_notes.append(Note(NoteName.RE, octave, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.SOL, octave-1, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.SI_B, octave-1, NoteLength.CROCHE))
        got_notes.append(Note(NoteName.DO, octave, NoteLength.CROCHE))

    got_notes.append(Note(NoteName.RE, octave, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.SOL, octave-1, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.SI_B, octave-1, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.FA, octave, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.FA, octave, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.SI, octave-1, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.SI, octave-1, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.MI_B, octave, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.RE, octave, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.FA, octave, NoteLength.BLANCHE))

    got_notes.append(Note(NoteName.SI, octave-1, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.SI, octave-1, NoteLength.NOIRE))


    got_notes.append(Note(NoteName.MI_B, octave, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.RE, octave, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.DO, octave, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.LA_B, octave-1, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.SI_B, octave-1, NoteLength.CROCHE))


    for i in range(2):
        got_notes.append(Note(NoteName.DO, octave, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.SOL, octave-1, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.LA_B, octave-1, NoteLength.CROCHE))
        got_notes.append(Note(NoteName.SI_B, octave-1, NoteLength.CROCHE))

    got_notes.append(Note(NoteName.DO, octave, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.SOL, octave-1, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.FA, octave+1, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.FA, octave+1, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.DO, octave+1, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.DO, octave+1, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.MI_B, octave+1, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.FA, octave+1, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.SOL, octave+1, NoteLength.BLANCHE))

    got_notes.append(Note(NoteName.DO, octave+1, NoteLength.BLANCHE))
    got_notes.append(Note(NoteName.MI_B, octave+1, NoteLength.CROCHE))
    got_notes.append(Note(NoteName.FA, octave+1, NoteLength.CROCHE))

    for i in range(3):
        got_notes.append(Note(NoteName.RE, octave+1, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.SOL, octave, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.SI, octave, NoteLength.CROCHE))
        got_notes.append(Note(NoteName.DO, octave+1, NoteLength.CROCHE))

    got_notes.append(Note(NoteName.RE, octave+1, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.SOL, octave, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.SI, octave, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.FA, octave+1, NoteLength.BLANCHE))

    got_notes.append(Note(NoteName.SI_B, octave, NoteLength.BLANCHE))

    got_notes.append(Note(NoteName.RE, octave+1, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.MI_B, octave+1, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.MI_B, octave+1, NoteLength.NOIRE))

    got_notes.append(Note(NoteName.RE, octave+1, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.SI_B, octave, NoteLength.NOIRE))
    got_notes.append(Note(NoteName.SI_B, octave, NoteLength.NOIRE))

    for i in range(2):
        got_notes.append(Note(NoteName.DO, octave+1, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.SOL, octave, NoteLength.NOIRE))
        got_notes.append(Note(NoteName.LA, octave, NoteLength.CROCHE))
        got_notes.append(Note(NoteName.SI_B, octave, NoteLength.CROCHE))

    for n in got_notes:
        msg = slip_transaction(fd, msg_to_slip_payload(n), False)
        while msg[0].data.status == 0:
            time.sleep(0.5)
            msg = slip_transaction(fd, msg_to_slip_payload(n), False)

def do_si(fd):
    conf = ReqMelodyConf(MelodySrc.FIFO, 60)
    msg = slip_transaction(fd, SlipPayload(conf.msg_id, 0, conf.pack()), True)
    print(msg)

    for n in NoteName:
        note = Note(n, 2, NoteLength.NOIRE)
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

    got(serial_fd)

if __name__ == "__main__":
    main()
