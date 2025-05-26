#include "MIDI_Notes.h"
#include "Arduino.h"

MIDI_Notes::MIDI_Notes(int baseNote) {
  _baseNote = baseNote;
  _octaveShift = 0;
  updateNotes();
}

void MIDI_Notes::updateNotes() {
  for (int i = 0; i < NOTES_COUNT; i++) {
    // Calculate MIDI note number for each key
    // baseNote + key index + octave shift (12 semitones per octave)
    _notes[i] = _baseNote + i + (_octaveShift * 12);
    
    // Clamp to valid MIDI range (0-127)
    if (_notes[i] < 0) _notes[i] = 0;
    if (_notes[i] > 127) _notes[i] = 127;
  }
}

int MIDI_Notes::get(int keyIndex) {
  if (keyIndex >= 0 && keyIndex < NOTES_COUNT) {
    return _notes[keyIndex];
  }
  return -1; // Invalid key
}

void MIDI_Notes::setOctaveShift(int shift) {
  _octaveShift = shift;
  updateNotes();
}

int MIDI_Notes::getOctaveShift() {
  return _octaveShift;
}
