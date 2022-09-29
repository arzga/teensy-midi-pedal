# Teensy MIDI Pedal

Send MIDI messages from a simple Teensy-powered circuit to a host computer via USB MIDI.

## Goals

- An USB MIDI device that can act as a foot switch for a headless synth/guitar effect like Raspberry Pi running Patchbox OS/ModeP.
- Powered from USB.

## Function

- Pressing L button sends MIDI CC 67 with value 127 when led off.
- Pressing L button sends MIDI CC 67 with value 0 when led on.
- Pressing R button sends MIDI CC 68 with value 127 when led off.
- Pressing R button sends MIDI CC 68 with value 0 when led on.
- Knob 1 sends MIDI CC 69
- Knob 2 sends MIDI CC 70
- Knob 3 sends MIDI CC 71
- Knob 4 sends MIDI CC 72
- MIDI CC state is sent periodically
- Pressing L+R buttons simultaneously enables "MIDI learn" mode

In MIDI learn mode leds alternate and periodical MIDI CC send is disabled until you touch any button or knob. This prevents CC messages from messing with learning process.

## Requirements

- A Teensy board (tested with Teensy 4.0) and Teensyduino (tested with 1.8.13)
- A circuit
  - Teensy button pins connected via momentary switch to GND
  - Teensy led pins connected via 3.3kOhm to led, then GND.
  - Teensy knob pins connected to 10kOhm potentiomenter center/wiper. +3.3V connected to pot left side (from below), GND to right.
