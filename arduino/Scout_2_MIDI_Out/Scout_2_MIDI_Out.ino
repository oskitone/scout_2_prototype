// SPDX-FileCopyrightText: 2025 Liz Clark
//
// SPDX-License-Identifier: MIT
/*
  Scout 2 MIDI Keyboard Controller
  * NoteOn and NoteOff messages for each key press
  * OCTAVE_PIN adjusts MIDI note number range
  * GLIDE_PIN controls pitch bend
*/

#include "Arduino.h"
#include "KeyBuffer.h"
#include "MIDI_Notes.h"

// pin defs
const int OCTAVE_PIN = A4;
const int GLIDE_PIN = A5;
const int PLAYING_INDICATOR_LED = 13;
const int FUNCTION_INDICATOR_LED = A3;

// MIDI constants
const int MIDI_CHANNEL = 1;
const int DEFAULT_VELOCITY = 64; // middle velocity

KeyBuffer buffer;
MIDI_Notes midiNotes(48); // start at C3 (MIDI note 48)

// variables
int octave = 0;
int pitchBend = 8192; // center position (no bend)
bool activeNotes[25] = {false}; // track which notes are pressed
unsigned long lastNoteTime[25] = {0}; // debounce timing for each key
const unsigned long NOTE_DEBOUNCE = 20; //

void blink(int count = 2, int timePerColor = 100) {
  while (count >= 0) {
    digitalWrite(PLAYING_INDICATOR_LED, HIGH);
    digitalWrite(FUNCTION_INDICATOR_LED, LOW);
    delay(timePerColor);
    digitalWrite(PLAYING_INDICATOR_LED, HIGH);
    digitalWrite(FUNCTION_INDICATOR_LED, HIGH);
    delay(timePerColor);
    digitalWrite(PLAYING_INDICATOR_LED, LOW);
    digitalWrite(FUNCTION_INDICATOR_LED, HIGH);
    delay(timePerColor);
    digitalWrite(PLAYING_INDICATOR_LED, LOW);
    digitalWrite(FUNCTION_INDICATOR_LED, LOW);
    delay(timePerColor);
    count = count - 1;
  }
}

void setup() {
  // MIDI baud rate:
  Serial.begin(31250);
  
  pinMode(PLAYING_INDICATOR_LED, OUTPUT);
  pinMode(FUNCTION_INDICATOR_LED, OUTPUT);

  blink();
  
  // send initial pitch bend to center position
  sendPitchBend(MIDI_CHANNEL, 8192);
}

void sendPitchBend(int channel, int bend) {
  // pitch bend uses 14-bit value split into LSB and MSB
  int lsb = bend & 0x7F;
  int msb = (bend >> 7) & 0x7F;
  
  Serial.write(0xE0 | (channel - 1)); // pitch bend message
  Serial.write(lsb);
  Serial.write(msb);
}

void sendAllNotesOff() {
  // all notes off (CC 123)
  Serial.write(0xB0 | (MIDI_CHANNEL - 1)); // CC
  Serial.write(123); // 123
  Serial.write(0);
}

void updateFromAnalogInputs() {
  int newOctave = map(analogRead(OCTAVE_PIN), 0, 1023, -2, 2); // -2 to +2 octaves
  int newPitchBend = map(analogRead(GLIDE_PIN), 0, 1023, 0, 16383); // pitch bend range
  
  if (octave != newOctave) {
    // send all notes off before changing octave, avoid stuck notes
    sendAllNotesOff();
    
    octave = newOctave;
    midiNotes.setOctaveShift(octave);
    digitalWrite(FUNCTION_INDICATOR_LED, HIGH);
  } else if (abs(pitchBend - newPitchBend) > 64) { // Add some hysteresis to reduce jitter
    pitchBend = newPitchBend;
    sendPitchBend(MIDI_CHANNEL, pitchBend);
    digitalWrite(FUNCTION_INDICATOR_LED, HIGH);
  } else {
    digitalWrite(FUNCTION_INDICATOR_LED, LOW);
  }
}

void sendNoteOn(int channel, int pitch, int velocity) {
  Serial.write(MIDI_NOTE_ON | (channel - 1));
  Serial.write(pitch);
  Serial.write(velocity);
}

void sendNoteOff(int channel, int pitch) {
  Serial.write(MIDI_NOTE_OFF | (channel - 1));
  Serial.write(pitch);
  Serial.write(0);
}

void loop() {
  // update octave and glide from analog inputs
  updateFromAnalogInputs();
  
  // update the key buffer with current key states
  buffer.populate();
  
  // update playing indicator LED
  if (buffer.isEmpty()) {
    digitalWrite(PLAYING_INDICATOR_LED, LOW);
  } else {
    digitalWrite(PLAYING_INDICATOR_LED, HIGH);
  }
  
  // scan matrix for noteon/noteoff
  for (int i = 0; i < 25; i++) {
    bool isPressed = false;
    for (int j = 0; j < buffer.getSize(); j++) {
      if (buffer.getElement(j) == i) {
        isPressed = true;
        break;
      }
    }
    // debounce
    unsigned long currentTime = millis();

    if (isPressed && !activeNotes[i]) {
      // key was just pressed
      if (currentTime - lastNoteTime[i] > NOTE_DEBOUNCE) {
        int midiNote = midiNotes.get(i);
        if (midiNote >= 0) {
          sendNoteOn(MIDI_CHANNEL, midiNote, DEFAULT_VELOCITY);
          activeNotes[i] = true;
          lastNoteTime[i] = currentTime;
        }
      }
    } else if (!isPressed && activeNotes[i]) {
      // key was just released
      if (currentTime - lastNoteTime[i] > NOTE_DEBOUNCE) {
        int midiNote = midiNotes.get(i);
        if (midiNote >= 0) {
          sendNoteOff(MIDI_CHANNEL, midiNote);
          activeNotes[i] = false;
          lastNoteTime[i] = currentTime;
        }
      }
    }
  }
}
