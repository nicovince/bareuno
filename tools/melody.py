#!/usr/bin/env python3

from genmsg.slip import *
from genmsg.messages import *
import argparse
import serial
import time

def msg_to_slip_payload(msg):
    return SlipPayload(msg.msg_id, 0, msg.pack())

def got(fd):
    conf = ReqMelodyConf(MelodySrc.FIFO.value, 60)
    msg = slip_transaction(fd, SlipPayload(conf.msg_id, 0, conf.pack()), False)
    got_notes = list()
    octave = 4
    for i in range(4):
        got_notes.append(Note(NoteName.SOL.value, octave, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.DO.value, octave, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.MI_B.value, octave, NoteLength.CROCHE.value))
        got_notes.append(Note(NoteName.FA.value, octave, NoteLength.CROCHE.value))

    for i in range(4):
        got_notes.append(Note(NoteName.SOL.value, octave, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.DO.value, octave, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.MI.value, octave, NoteLength.CROCHE.value))
        got_notes.append(Note(NoteName.FA.value, octave, NoteLength.CROCHE.value))

    got_notes.append(Note(NoteName.SOL.value, octave, NoteLength.RONDE.value))

    got_notes.append(Note(NoteName.DO.value, octave, NoteLength.RONDE.value))

    got_notes.append(Note(NoteName.MI_B.value, octave, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.FA.value, octave, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.SOL.value, octave, NoteLength.BLANCHE.value))

    got_notes.append(Note(NoteName.DO.value, octave, NoteLength.BLANCHE.value))
    got_notes.append(Note(NoteName.MI_B.value, octave, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.FA.value, octave, NoteLength.CROCHE.value))

    for i in range(3):
        got_notes.append(Note(NoteName.RE.value, octave, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.SOL.value, octave-1, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.SI_B.value, octave-1, NoteLength.CROCHE.value))
        got_notes.append(Note(NoteName.DO.value, octave, NoteLength.CROCHE.value))

    got_notes.append(Note(NoteName.RE.value, octave, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.SOL.value, octave-1, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.SI_B.value, octave-1, NoteLength.NOIRE.value))

    got_notes.append(Note(NoteName.FA.value, octave, NoteLength.RONDE.value))

    got_notes.append(Note(NoteName.DO.value, octave, NoteLength.RONDE.value))

    got_notes.append(Note(NoteName.MI_B.value, octave, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.RE.value, octave, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.FA.value, octave, NoteLength.BLANCHE.value))

    got_notes.append(Note(NoteName.SI_B.value, octave-1, NoteLength.RONDE.value))

    for i in range(3):
        got_notes.append(Note(NoteName.MI_B.value, octave, NoteLength.CROCHE.value))
        got_notes.append(Note(NoteName.RE.value, octave, NoteLength.CROCHE.value))
        got_notes.append(Note(NoteName.DO.value, octave, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.LA_B.value, octave-1, NoteLength.CROCHE.value))
        got_notes.append(Note(NoteName.SI_B.value, octave-1, NoteLength.CROCHE.value))

    got_notes.append(Note(NoteName.DO.value, octave, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.FA.value, octave-1, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.DO.value, octave, NoteLength.NOIRE.value))

    got_notes.append(Note(NoteName.SOL.value, octave, NoteLength.RONDE.value))

    got_notes.append(Note(NoteName.DO.value, octave, NoteLength.RONDE.value))

    got_notes.append(Note(NoteName.MI_B.value, octave, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.FA.value, octave, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.SOL.value, octave, NoteLength.BLANCHE.value))

    got_notes.append(Note(NoteName.DO.value, octave, NoteLength.BLANCHE.value))
    got_notes.append(Note(NoteName.MI_B.value, octave, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.FA.value, octave, NoteLength.CROCHE.value))

    for i in range(3):
        got_notes.append(Note(NoteName.RE.value, octave, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.SOL.value, octave-1, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.SI_B.value, octave-1, NoteLength.CROCHE.value))
        got_notes.append(Note(NoteName.DO.value, octave, NoteLength.CROCHE.value))

    got_notes.append(Note(NoteName.RE.value, octave, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.SOL.value, octave-1, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.SI_B.value, octave-1, NoteLength.NOIRE.value))

    got_notes.append(Note(NoteName.FA.value, octave, NoteLength.RONDE.value))

    got_notes.append(Note(NoteName.SI.value, octave-1, NoteLength.RONDE.value))

    got_notes.append(Note(NoteName.MI_B.value, octave, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.RE.value, octave, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.FA.value, octave, NoteLength.BLANCHE.value))

    got_notes.append(Note(NoteName.SI.value, octave-1, NoteLength.RONDE.value))


    got_notes.append(Note(NoteName.MI_B.value, octave, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.RE.value, octave, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.DO.value, octave, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.LA_B.value, octave-1, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.SI_B.value, octave-1, NoteLength.CROCHE.value))


    for i in range(2):
        got_notes.append(Note(NoteName.DO.value, octave, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.SOL.value, octave-1, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.LA_B.value, octave-1, NoteLength.CROCHE.value))
        got_notes.append(Note(NoteName.SI_B.value, octave-1, NoteLength.CROCHE.value))

    got_notes.append(Note(NoteName.DO.value, octave, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.SOL.value, octave-1, NoteLength.NOIRE.value))

    got_notes.append(Note(NoteName.FA.value, octave+1, NoteLength.RONDE.value))
    got_notes.append(Note(NoteName.DO.value, octave+1, NoteLength.RONDE.value))

    got_notes.append(Note(NoteName.MI_B.value, octave+1, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.FA.value, octave+1, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.SOL.value, octave+1, NoteLength.BLANCHE.value))

    got_notes.append(Note(NoteName.DO.value, octave+1, NoteLength.BLANCHE.value))
    got_notes.append(Note(NoteName.MI_B.value, octave+1, NoteLength.CROCHE.value))
    got_notes.append(Note(NoteName.FA.value, octave+1, NoteLength.CROCHE.value))

    for i in range(3):
        got_notes.append(Note(NoteName.RE.value, octave+1, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.SOL.value, octave, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.SI.value, octave, NoteLength.CROCHE.value))
        got_notes.append(Note(NoteName.DO.value, octave+1, NoteLength.CROCHE.value))

    got_notes.append(Note(NoteName.RE.value, octave+1, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.SOL.value, octave, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.SI.value, octave, NoteLength.NOIRE.value))

    got_notes.append(Note(NoteName.FA.value, octave+1, NoteLength.BLANCHE.value))

    got_notes.append(Note(NoteName.SI_B.value, octave, NoteLength.BLANCHE.value))

    got_notes.append(Note(NoteName.RE.value, octave+1, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.MI_B.value, octave+1, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.MI_B.value, octave+1, NoteLength.NOIRE.value))

    got_notes.append(Note(NoteName.RE.value, octave+1, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.SI_B.value, octave, NoteLength.NOIRE.value))
    got_notes.append(Note(NoteName.SI_B.value, octave, NoteLength.NOIRE.value))

    for i in range(2):
        got_notes.append(Note(NoteName.DO.value, octave+1, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.SOL.value, octave, NoteLength.NOIRE.value))
        got_notes.append(Note(NoteName.LA.value, octave, NoteLength.CROCHE.value))
        got_notes.append(Note(NoteName.SI_B.value, octave, NoteLength.CROCHE.value))

    for n in got_notes:
        msg = slip_transaction(fd, msg_to_slip_payload(n), False)
        while msg[0].data.status == 0:
            time.sleep(0.5)
            msg = slip_transaction(fd, msg_to_slip_payload(n), False)

def do_si(fd):
    conf = ReqMelodyConf(MelodySrc.FIFO.value, 60)
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

    got(serial_fd)

if __name__ == "__main__":
    main()
