# Teensy MIDI Pedal

Send MIDI messages from a simple Teensy-powered circuit to a host computer via USB MIDI.

## Goals

- An USB MIDI device that can act as a foot switch for a headless synth/guitar effect, e.g. ModeP
- Powered from USB

## Function

- Pressing button A sends MIDI CC 67 with value 127 when led off
- Pressing button A sends MIDI CC 67 with value 0 when led on
- Pressing button B sends MIDI CC 68 with value 127 when led off
- Pressing button B sends MIDI CC 68 with value 0 when led on
- MIDI CC state is sent periodically

## Requirements

- A Teensy board (tested with Teensy 4.0) and Teensyduino (tested with 1.8.13)
- A circuit with buttons connected from A0..A1 via a switch to GND and a led connected from A2 to GND (with 1k resistor).
