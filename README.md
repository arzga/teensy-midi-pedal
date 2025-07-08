# MIDI Control Stompbox For Guitar FX

![Stompbox](/hardware/stompbox-with-print.jpg?raw=true "Stompbox")

My motivation of this project was to adjust my MIDI controllable guitar effects with ease. My setup with [Patchbox OS/ModeP virtual pedalboard](https://blokas.io/modep/) is compact and sounds great, but required a separate phone, computer or display for any adjustments during play.

Enter my MIDI controller stompbox: With few knobs and toggle buttons I can now turn on effects, control looper and adjust the key parameters. Once I have built the virtual pedalboards and bound the parameters to knobs and switches, I'm can play guitar conveniently without having a clumsy screen around.

This repository contains the source code for a [Teensy microcontroller](https://www.pjrc.com/store/teensy40.html) that reads analog knobs and sends MIDI CC messages to a host computer via USB MIDI, along with the [wiring sketches](/hardware) and my print templates for the a [readymade enclosure](https://en.uraltone.com/uraltone-1590bb-double-valmiiksi-porattu-efektikotelo.html).

## Features

- USB powered MIDI device
- Controls an external (guitar) FX with MIDI CC
- FX adjustments are visible from controller knob positions and LEDs
- Sends all toggle button/knob states MIDI CC periodically
- Compatible with [Patchbox OS/ModeP virtual pedalboard](https://blokas.io/modep/) running on a Raspberry Pi (tested with 3B)

## Usage

- Pressing L or R button toggles a MIDI CC between value 0 (off) and 127 (on). The actual controller depends on the selected page (1, 2 or 3).
- Holding L button for 1 seconds switches to page 1 (e.g. fx & tone controls)
- Holding L button for 2 seconds switches to page 2 (e.g. looper controls)
- Holding L button for 3 seconds switches to page 3 (e.g. amp/cab simulation)
- Pressing L+R buttons simultaneously enables "MIDI learn" mode (see below)

### MIDI learn mode ("teach" to be more accurate)

MIDI learn ("teach") mode waits for you to turn a knob or press a button before sending any MIDI. LEDs alternate to indicate learn mode.

Why is this special mode even needed? During normal operation, the controller periodically sends current MIDI CC values to the receiving MIDI device. Without the "teach mode", this MIDI dump would mess the learning process.  

### Page 1 default MIDI CCs (e.g. for controlling Tone/FX)

- Pressing L button sends MIDI CC 67 with value 127 when led off.
- Pressing L button sends MIDI CC 67 with value 0 when led on.
- Pressing R button sends MIDI CC 68 with value 127 when led off.
- Pressing R button sends MIDI CC 68 with value 0 when led on.
- Holding R button for 1 led blinks sends program change 0 on MIDI channel 15
- Holding R button for 2 led blinks sends program change 1 on MIDI channel 15
- Holding R button for 3 led blinks sends program change 2 on MIDI channel 15
- Knob 1 sends MIDI CC 69
- Knob 2 sends MIDI CC 70
- Knob 3 sends MIDI CC 71
- Knob 4 sends MIDI CC 72

### Page 2 default MIDI CCs (e.g. for controlling Suupaloopa looper)

- Pressing L button sends MIDI CC 80 with value 127 when led off. (e.g. Play)
- Pressing L button sends MIDI CC 80 with value 0 when led on.
- Pressing R button sends MIDI CC 81 with value 127 when led off. (e.g. Record)
- Pressing R button sends MIDI CC 81 with value 0 when led on.
- Holding R button for 1 led blinks sends momentary 127 on MIDI CC 103 (e.g. Undo)
- Holding R button for 2 led blinks sends momentary 127 on MIDI CC 104 (e.g. Redo)
- Holding R button for 3 led blinks sends momentary 127 on MIDI CC 105 (e.g. Reset)
- Knob 1 sends MIDI CC 82
- Knob 2 sends MIDI CC 83
- Knob 3 sends MIDI CC 84
- Knob 4 sends MIDI CC 85

### Page 3: Auxillary controls (e.g. amp/cab settings)

- Pressing L button sends MIDI CC 86 with value 127 when led off.
- Pressing L button sends MIDI CC 86 with value 0 when led on.
- Pressing R button sends MIDI CC 87 with value 127 when led off.
- Pressing R button sends MIDI CC 87 with value 0 when led on.
- Holding R button for 1 led blinks sends momentary 127 on MIDI CC 106
- Holding R button for 2 led blinks sends momentary 127 on MIDI CC 107
- Holding R button for 3 led blinks sends momentary 127 on MIDI CC 108
- Knob 1 sends MIDI CC 88
- Knob 2 sends MIDI CC 89
- Knob 3 sends MIDI CC 90
- Knob 4 sends MIDI CC 91

## Requirements

- A Teensy board (tested with Teensy 4.0) and Teensyduino (tested with 1.8.13)
- Arduino IDE with additional Teensy board manager URL configured
- [A wiring circuit](/hardware/connections.pdf)
  - Teensy button pins connected via momentary switch to GND
  - Teensy LED pins (PWM capable) connected via 3.3kOhm to led, then GND.
  - Teensy knob pins (analog read capable) connected to 10kOhm potentiomenter center/wiper. +3.3V connected to pot left side (from below), GND to right.

Actual pin numbers can be configured in the sketch's setup method.
