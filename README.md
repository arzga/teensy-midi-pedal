# MIDI Control Stompbox For Guitar FX

![Stompbox](/hardware/stompbox-with-print.jpg?raw=true "Stompbox")

Teensy program for sending MIDI CC messages from a simple Teensy-powered circuit to a host computer via USB MIDI.

## Goals

- USB powered MIDI device
- Controls an external (guitar) FX with MIDI CC
- State of the FX is visible on the controller (sends all toggle button/knob states MIDI CC periodically)
- Compatible with Patchbox OS/ModeP running on a Raspberry Pi 3+

## Function

- Pressing L or R button toggles a MIDI CC between value 0 (off) and 127 (on). The actual controller depends on the selected page (1, 2 or 3).
- Holding L button for 1 seconds switches to normal controls (page 1, e.g. fx & tone controls)
- Holding L button for 2 seconds switches to alternative controls 1 (page 2, e.g. looper)
- Holding L button for 3 seconds switches to alternative controls 2 (page 3, e.g. amp/cab simulation)
- Pressing L+R buttons simultaneously enables "MIDI learn" mode (see below)

### MIDI learn mode ("teach" to be more accurate)

MIDI learn ("teach") mode waits for you to turn a knob or press a button before sending any MIDI. LEDs alternate to indicate learn mode.

Why is this special mode even needed? During normal operation, the controller periodically sends current MIDI CC values to the receiving MIDI device. Without the "teach mode", this MIDI dump would mess the learning process.  

### Page 1 default MIDI CCs (e.g. for controlling Tone/FX)

- Pressing L button sends MIDI CC 67 with value 127 when led off.
- Pressing L button sends MIDI CC 67 with value 0 when led on.
- Pressing R button sends MIDI CC 68 with value 127 when led off.
- Pressing R button sends MIDI CC 68 with value 0 when led on.
- Holding R button for 1 seconds sends momentary 127 on MIDI CC 100
- Holding R button for 2 seconds sends momentary 127 on MIDI CC 101
- Holding R button for 3 seconds sends momentary 127 on MIDI CC 102
- Knob 1 sends MIDI CC 69
- Knob 2 sends MIDI CC 70
- Knob 3 sends MIDI CC 71
- Knob 4 sends MIDI CC 72

### Page 2 default MIDI CCs (e.g. for controlling Suupaloopa looper)

- Pressing L button sends MIDI CC 80 with value 127 when led off. (e.g. Play)
- Pressing L button sends MIDI CC 80 with value 0 when led on.
- Pressing R button sends MIDI CC 81 with value 127 when led off. (e.g. Record)
- Pressing R button sends MIDI CC 81 with value 0 when led on.
- Holding R button for 1 seconds sends momentary 127 on MIDI CC 103 (e.g. Undo)
- Holding R button for 2 seconds sends momentary 127 on MIDI CC 104 (e.g. Redo)
- Holding R button for 3 seconds sends momentary 127 on MIDI CC 105 (e.g. Reset)
- Knob 1 sends MIDI CC 82
- Knob 2 sends MIDI CC 83
- Knob 3 sends MIDI CC 84
- Knob 4 sends MIDI CC 85

### Page 3: Auxillary controls (e.g. amp/cab settings)

- Pressing L button sends MIDI CC 86 with value 127 when led off.
- Pressing L button sends MIDI CC 86 with value 0 when led on.
- Pressing R button sends MIDI CC 87 with value 127 when led off.
- Pressing R button sends MIDI CC 87 with value 0 when led on.
- Holding R button for 1 seconds sends momentary 127 on MIDI CC 106
- Holding R button for 2 seconds sends momentary 127 on MIDI CC 107
- Holding R button for 3 seconds sends momentary 127 on MIDI CC 108
- Knob 1 sends MIDI CC 88
- Knob 2 sends MIDI CC 89
- Knob 3 sends MIDI CC 90
- Knob 4 sends MIDI CC 91

## Requirements

- A Teensy board (tested with Teensy 4.0) and Teensyduino (tested with 1.8.13)
- A circuit
  - Teensy button pins connected via momentary switch to GND
  - Teensy LED pins (PWM capable) connected via 3.3kOhm to led, then GND.
  - Teensy knob pins (analog read capable) connected to 10kOhm potentiomenter center/wiper. +3.3V connected to pot left side (from below), GND to right.

Actual pin numbers can be configured in the sketch's setup method.
