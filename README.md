# MIDI Control Stompbox For Guitar FX

![Stompbox](/hardware/stompbox-with-print.jpg?raw=true "Stompbox")

## Why all this?

My motivation of this project was to adjust my MIDI controllable guitar effects with ease. I like my [Raspberry Pi based ModeP virtual pedalboard](https://blokas.io/modep/) (by awesome folks at MOD and Blokas) as it's versatile and compact, but to control it I needed a separate phone, computer or display. 

**Enter the MIDI controller stompbox**: I can now play and control the virtual pedalboard with physical foot switches and knobs, turn on the effects and control the looper.

### This repository contains

- The source code for a [Teensy microcontroller](https://www.pjrc.com/store/teensy40.html) that reads analog knobs and sends MIDI CC messages to a host computer via USB MIDI
- [Wiring sketches](/hardware) and my print template I used with a [readymade enclosure](https://en.uraltone.com/uraltone-1590bb-double-valmiiksi-porattu-efektikotelo.html)

## Features (default setup)

- USB powered MIDI device that controls an external (guitar) FX
- Effect settings are constantly synced and visible from LEDs and controller knob positions
- 2 foot switches (momentary), 4 analog knobs, 2 leds
- 3 "pages" with different control mappings
- Compatible with [Patchbox OS/ModeP virtual pedalboard](https://blokas.io/modep/) running on a Raspberry Pi (tested with 3B)

## Usage

- Tapping the left (L) or right (R) foot switch toggles an effect on/off. Action depends on the page (1, 2 or 3).
- Holding L button for 1 led blinks switches to page 1 (e.g. fx & tone controls)
- Holding L button for 2 led blinks switches to page 2 (e.g. looper controls)
- Holding L button for 3 led blinks switches to page 3 (e.g. amp/cab simulation)
- Holding R button for 1 to 3 led blinks performs an auxillary action (e.g. switch patch or undo/redo looper)
- Pressing L+R buttons simultaneously enables "MIDI teach" mode (see below)

### MIDI teach mode

MIDI teach mode waits for you to turn a knob or press a button before sending any MIDI. LEDs alternate to indicate mode.

Why a teach mode? During normal operation, the controller periodically sends current MIDI CC values to the receiving MIDI device to keep the effect device in sync. Without the "teach mode", this would mess the MIDI learn.

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

- A MIDI compatible effects board (or a synthesizer)
- A Teensy microcontroller (tested with Teensy 4.0)
- Arduino IDE with additional Teensy board manager URL configured (tested with 2.1.0)
- [A wiring circuit](/hardware/connections.pdf) like this one
  - Teensy button pins connected via momentary switch to GND
  - Teensy LED pins (PWM capable) connected via 3.3kOhm to led, then GND.
  - Teensy knob pins (analog read capable) connected to 10kOhm potentiomenter center/wiper. +3.3V connected to pot left side (from below), GND to right.

Actual pin numbers can be configured in the program sketch's setup method.
