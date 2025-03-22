#include "Frequency.h"
#include "KeyBuffer.h"
#include "Notes.h"
#include "NewTone.h"

// SETTINGS
int octave = 3;
float glide = .25;
bool glideOnFreshKeyPresses = true;
bool printToSerial = false;

const int CYCLES_PER_GLIDE_MAX = printToSerial ? 25 : 250;
const int STARTING_NOTE_DISTANCE_FROM_MIDDLE_A = -9;

const int OCTAVE_PIN = A4;
const int GLIDE_PIN = A5;

const int PLAYING_INDICATOR_LED = 13; // ie LED_BUILTIN
const int FUNCTION_INDICATOR_LED = A3;

Notes notes(STARTING_NOTE_DISTANCE_FROM_MIDDLE_A);
KeyBuffer buffer;
Frequency frequency[BUFFER_MAX] = { {glide, CYCLES_PER_GLIDE_MAX}, {glide, CYCLES_PER_GLIDE_MAX}, {glide, CYCLES_PER_GLIDE_MAX}, {glide, CYCLES_PER_GLIDE_MAX} };

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
  Serial.begin(9600);

  pinMode(PLAYING_INDICATOR_LED, OUTPUT);
  pinMode(FUNCTION_INDICATOR_LED, OUTPUT);

  newToneSetup();

  blink();
}

void updateFromAnalogInputs() {
  int newOctave = map(analogRead(OCTAVE_PIN), 0, 1023, -1, 4);
  float newGlide = float(analogRead(GLIDE_PIN)) / 1023;

  if (octave != newOctave || glide != newGlide) {
    octave = newOctave;
    glide = newGlide;

    digitalWrite(FUNCTION_INDICATOR_LED, HIGH);
  } else {
    digitalWrite(FUNCTION_INDICATOR_LED, LOW);
  }
}

void loop() {
  uint8_t i;
  uint8_t size;

  buffer.populate();
  size = buffer.getSize();

  // TODO: do this less often...
  updateFromAnalogInputs();

  if (printToSerial) {
    for (i = 0; i < size; i++) {
      frequency[i].print();
    }
  }

  if (buffer.isEmpty()) {
    digitalWrite(PLAYING_INDICATOR_LED, LOW);
  } else {
    digitalWrite(PLAYING_INDICATOR_LED, HIGH);
  }

  for (i = 0; i < size; i++) {
    frequency[i].update(notes.get(buffer.getElement(i)) / 4 * pow(2, octave), glide);
    speaker_preload[i] = frequency[i].getPeriod();
  }
  for (; i < BUFFER_MAX; i++) {
    if (!glideOnFreshKeyPresses) {
      frequency[i].reset();
    }
    speaker_preload[i] = 0;
  }
}

