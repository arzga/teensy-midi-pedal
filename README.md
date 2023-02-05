# Teensy MIDI Pedal

![Stompbox](/hardware/stompbox.jpg?raw=true "Stompbox")

Send MIDI messages from a simple Teensy-powered circuit to a host computer via USB MIDI.

## Goals

- An USB MIDI device that can act as a foot switch for a headless synth/guitar effect like Raspberry Pi running Patchbox OS/ModeP.
- Powered from USB.
- MIDI CC states are sent periodically for toggle buttons and knobs (not for special "momentary buttons")

## Function

- Pressing L+R buttons simultaneously enables "MIDI learn" mode
- Holding L button for 1 seconds switches to normal controls
- Holding L button for 2 seconds switches to alternative controls 1 (e.g. looper)
- Holding L button for 3 seconds switches to alternative controls 2 (e.g. settings for amp simulation)

### Normal controls

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

### Alternative controls 1 (e.g. Suupaloopa looper)

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

### Alternative controls 2 (e.g. settings)

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

### Midi learn mode

In MIDI learn mode periodical MIDI CC send is disabled until you touch any button or knob. This prevents CC messages from messing with learning process. LEDs alternate to indicate learn mode. 

## Requirements

- A Teensy board (tested with Teensy 4.0) and Teensyduino (tested with 1.8.13)
- A circuit
  - Teensy button pins connected via momentary switch to GND
  - Teensy LED pins (PWM capable) connected via 3.3kOhm to led, then GND.
  - Teensy knob pins (analog read capable) connected to 10kOhm potentiomenter center/wiper. +3.3V connected to pot left side (from below), GND to right.

Actual pin numbers can be configured in the sketch's setup method.
