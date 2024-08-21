#pragma once

#include <Arduino.h>
#include <OneButton.h>
#include "OneRotaryEncoder.h"

//#define ENABLE_MIDI
#define MIDI_CONTROLLER_MAX_DEVICES 20

class MidiController
{
public:
  MidiController();
  void Setup();
  void Tick();
  
private:  
  OneButton* buttons[MIDI_CONTROLLER_MAX_DEVICES] = {};
  OneRotaryEncoder* encoders[MIDI_CONTROLLER_MAX_DEVICES] = {};

  static void SendProgramChange(int program, int channel);
  static void SendControlChange(uint8_t number, uint8_t value, uint8_t channel);
};