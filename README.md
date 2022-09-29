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
- A circuit with buttons connected from A0..A1 via a switch to GND and a led connected from A2 to GND (with 1k resistor).
