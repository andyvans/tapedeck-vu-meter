#include "AudioControls.h"
#include <MIDI.h>
#include "constants.h"
#include "AcidBoxMidi.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

AudioControls::AudioControls()
{
  // Set switch pins to digital input
  pinMode(LEFT_SWITCH_PIN, INPUT);
  pinMode(RIGHT_SWITCH_PIN, INPUT);

  // Setup encoders and buttons
  int buttonCount = -1;
  int encoderCount = -1;

  // REWIND button
  auto button1 = new OneButton(REWIND_BUTTON_PIN, true);
  button1-> attachClick([](void *scope) {
    ((AudioControls*)scope)->SendControlChange(CC_808_VOLUME, 0, DRUM_MIDI_CHAN);
  }, this);
  buttons[++buttonCount] = button1;

  // // FAST FORWARD button
  // auto button2 = new OneButton(FAST_FORWARD_BUTTON_PIN, true);
  // button2-> attachClick([]() {
  //   SendControlChange(CC_303_VOLUME, 0, SYNTH1_MIDI_CHAN);
  // });
  // buttons[++buttonCount] = button2;

  // // PLAY button
  // auto button3 = new OneButton(PLAY_BUTTON_PIN, true);
  // button3-> attachClick([]() {
  //   SendControlChange(CC_303_VOLUME, 0, SYNTH2_MIDI_CHAN);
  // });
  // buttons[++buttonCount] = button3;

  // // STOP button
  // auto button4 = new OneButton(STOP_BUTTON_PIN, true);
  // button4-> attachClick([]() {
  //   SendControlChange(CC_303_VOLUME, 0, SYNTH2_MIDI_CHAN);
  // });
  // buttons[++buttonCount] = button4;

  // Encoder 1
  auto encoder1 = new OneRotaryEncoder(ROTARY_ENCODER_1_A_PIN, ROTARY_ENCODER_1_B_PIN, ROTARY_ENCODER_1_SW_PIN, 127);
  encoders[++encoderCount] = encoder1;

  // Encoder 2
  //auto encoder2 = new OneRotaryEncoder(255, ROTARY_ENCODER_2_A_PIN, ROTARY_ENCODER_2_B_PIN, ROTARY_ENCODER_2_SW_PIN);
  //encoders[++encoderCount] = encoder2;

  // Encoder 3
  //auto encoder3 = new OneRotaryEncoder(255, ROTARY_ENCODER_3_A_PIN, ROTARY_ENCODER_3_B_PIN, ROTARY_ENCODER_3_SW_PIN);
  //encoders[++encoderCount] = encoder3;
  
  // Encoder 4
  //auto encoder4 = new OneRotaryEncoder(255, ROTARY_ENCODER_4_A_PIN, ROTARY_ENCODER_4_B_PIN, ROTARY_ENCODER_4_SW_PIN);
  //encoders[++encoderCount] = encoder4;
}

void AudioControls::Setup()
{
  #ifdef ENABLE_MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
  #endif
}

void AudioControls::Tick()
{
#ifdef ENABLE_MIDI
  MIDI.read();
#endif

  for (int i = 0; i < MIDI_CONTROLLER_MAX_DEVICES; i++)
  {
    if (buttons[i] != NULL) buttons[i]->tick();
    if (encoders[i] != NULL) encoders[i]->Tick();
  }

  ProcessAudioControl();
}

void AudioControls::ProcessAudioControl()
{
  AudioControlMode midiState = AudioControlMode::Mode1; //ReadSwitchStates();

  auto encoderSwitch1 = encoders[0]->GetSwitchState();
  //auto encoderSwitch2 = encoders[1]->GetSwitchState();
  //auto encoderSwitch3 = encoders[2]->GetSwitchState();
  //auto encoderSwitch4 = encoders[3]->GetSwitchState();

  auto encoderPos1 = encoders[0]->GetPosition();  
  //auto encoderPos2 = encoders[1]->GetPosition();

#ifndef ENABLE_MIDI
  if (encoderSwitch1.hasNewState || encoderPos1.hasNewPosition)
  {
    Serial.print("Encoder switch: ");
    Serial.print(encoderSwitch1.hasNewState);
    Serial.print(encoderSwitch1.state);
    Serial.print(" Encoder new: ");
    Serial.print(encoderPos1.hasNewPosition);
    Serial.print(" Encoder position: ");
    Serial.print(encoderPos1.position);
    Serial.println();
  }
#endif

  switch (midiState)
  {
  case AudioControlMode::Mode1:
    if (encoderSwitch1.hasNewState)
    {
      // if (encoderSwitch1.state == EncoderSwitchPress::Clicked)
      // {
      //   SendControlChange(CC_808_VOLUME, 0, DRUM_MIDI_CHAN);
      // }
      // else if (encoderSwitch1.state == EncoderSwitchPress::LongPressed)
      // {
      //   SendControlChange(CC_808_VOLUME, 127, DRUM_MIDI_CHAN);
      // }
      //SendControlChange(CC_808_VOLUME, 0, DRUM_MIDI_CHAN);
    }
    // if (encoderSwitch2.hasNewState)
    // {
    //   if (encoderSwitch2.state == EncoderSwitchPress::Clicked)
    //   {
    //     SendControlChange(CC_303_VOLUME, 0, SYNTH1_MIDI_CHAN);
    //   }
    //   else if (encoderSwitch2.state == EncoderSwitchPress::LongPressed)
    //   {
    //     SendControlChange(CC_303_VOLUME, 255, SYNTH1_MIDI_CHAN);
    //   }
    // }

    if (encoderPos1.hasNewPosition)
    {
      SendControlChange(CC_808_VOLUME, encoderPos1.position, DRUM_MIDI_CHAN);
    }    
    // if (encoderPos2.hasNewPosition)  
    // {
    //   SendControlChange(CC_303_VOLUME, encoderPos2.position, SYNTH1_MIDI_CHAN);
    // }
    break;
  case AudioControlMode::Mode2:
    break;
  }

  // SendControlChange(CC_808_VOLUME, pos, DRUM_MIDI_CHAN);
  // SendControlChange(CC_808_VOLUME, 0, DRUM_MIDI_CHAN);
  // SendControlChange(CC_808_VOLUME, 255, DRUM_MIDI_CHAN);
  // SendControlChange(CC_303_VOLUME, pos, SYNTH1_MIDI_CHAN);
  // SendControlChange(CC_303_VOLUME, 0, SYNTH1_MIDI_CHAN);
  // SendControlChange(CC_303_VOLUME, 255, SYNTH1_MIDI_CHAN);
  // SendControlChange(CC_303_VOLUME, pos, SYNTH2_MIDI_CHAN);
  // SendControlChange(CC_303_VOLUME, 0, SYNTH2_MIDI_CHAN);
  // SendControlChange(CC_303_VOLUME, 255, SYNTH2_MIDI_CHAN);
  // SendControlChange(CC_ANY_REVERB_TIME, pos, SYNTH1_MIDI_CHAN);
  // SendControlChange(CC_ANY_REVERB_TIME, 0, SYNTH1_MIDI_CHAN);
  // SendControlChange(CC_ANY_REVERB_TIME, 255, SYNTH1_MIDI_CHAN);
}

AudioControlMode AudioControls::GetControlSet()
{
  bool leftSwitchState = digitalRead(LEFT_SWITCH_PIN) == HIGH;
  bool rightSwitchState = digitalRead(RIGHT_SWITCH_PIN) == HIGH;

  if (leftSwitchState && rightSwitchState)
  {
    return AudioControlMode::Mode4;
  }
  else if (leftSwitchState)
  {
    return AudioControlMode::Mode2;
  }
  else if (rightSwitchState)
  {
    return AudioControlMode::Mode3;
  }
  else
  {
    return AudioControlMode::Mode1;
  }
}

void AudioControls::SendProgramChange(int program, int channel)
{
#ifdef ENABLE_MIDI
  MIDI.sendProgramChange(program, channel);
#else
  Serial.print("Program change:");
  Serial.print(program);
  Serial.print(" ");
  Serial.print(channel);
  Serial.println();
#endif
}

void AudioControls::SendControlChange(uint8_t program, uint8_t value, uint8_t channel)
{
  // Midi values are 0-127. Rotary encoder values are 0-255.
  int midiValue = value;//map(value, 0, 255, 0, 127);

#ifdef ENABLE_MIDI
  MIDI.sendControlChange(number, midiValue, channel);
#else
  Serial.print("Control change:");
  Serial.print(program);
  Serial.print(" ");
  Serial.print(midiValue);
  Serial.print(" ");
  Serial.print(channel);
  Serial.println();
#endif
}
