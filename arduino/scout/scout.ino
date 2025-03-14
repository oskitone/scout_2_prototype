#include "Frequency.h"
#include "KeyBuffer.h"
#include "Notes.h"

// SETTINGS
int octave = 3;
float glide = .25;
bool glideOnFreshKeyPresses = true;
bool printToSerial = false;

const int CYCLES_PER_GLIDE_MAX = printToSerial ? 25 : 250;
const int STARTING_NOTE_DISTANCE_FROM_MIDDLE_A = -9;

// TODO: use other output pins 10,11,12
const int SPEAKER_PIN = 10;

const int OCTAVE_PIN = A4;
const int GLIDE_PIN = A5;

const int PLAYING_INDICATOR_LED = 13; // ie LED_BUILTIN
const int FUNCTION_INDICATOR_LED = A3;

Notes notes(STARTING_NOTE_DISTANCE_FROM_MIDDLE_A);
KeyBuffer buffer;
Frequency frequency(glide, CYCLES_PER_GLIDE_MAX);

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

  blink();
}

void updateFromAnalogInputs() {
  int newOctave = map(analogRead(OCTAVE_PIN), 0, 1023, 0, 4);
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
  buffer.populate();

  // TODO: do this less often...
  updateFromAnalogInputs();

  if (printToSerial) {
    frequency.print();
  }

  if (buffer.isEmpty()) {
    if (!glideOnFreshKeyPresses) {
      frequency.reset();
    }

    noTone(SPEAKER_PIN);
    digitalWrite(PLAYING_INDICATOR_LED, LOW);
  } else {
    frequency.update(notes.get(buffer.getFirst()) / 4 * pow(2, octave), glide);

    tone(SPEAKER_PIN, frequency.get());
    digitalWrite(PLAYING_INDICATOR_LED, HIGH);
  }
}
