// SPDX-FileCopyrightText: 2025 Liz Clark
//
// SPDX-License-Identifier: MIT
/*
  MIDI Notes helper. Adjusts assigned MIDI note numbers similar to the Notes helper.
*/
#ifndef MIDI_Notes_h
#define MIDI_Notes_h

#include "Arduino.h"

const int NOTES_COUNT = 25;
const int MIDI_NOTE_OFF = 0x80;
const int MIDI_NOTE_ON = 0x90;

class MIDI_Notes {
public:
  MIDI_Notes(int baseNote = 48); // Default to C3 (middle C)
  int get(int keyIndex);
  void setOctaveShift(int shift);
  int getOctaveShift();
  
private:
  int _baseNote;
  int _octaveShift;
  int _notes[NOTES_COUNT];
  void updateNotes();
};

#endif
