#include "Frequency.h"
#include "KeyBuffer.h"
#include "Notes.h"

// PCB NOTES:
// * Response on analog pot inputs is because they're log? Should be linear.
// * Power switch is reversed. Typical!

// SETTINGS
int octave = 3;
float glide = .25;
bool glideOnFreshKeyPresses = true;
bool printToSerial = false;

const int CYCLES_PER_GLIDE_MAX = printToSerial ? 25 : 250;
const int STARTING_NOTE_DISTANCE_FROM_MIDDLE_A = -9;

const int SPEAKER_PIN = 9; // or 10,11,12
// TODO: why again does Mozi use pin 9?
// NOTE: PB0 is D8,D11,D10,D9,D12,D13 (last two bits are shared w/ external crystal)

const int OCTAVE_PIN = A4;
const int GLIDE_PIN = A5;

// TODO: use + other pin
const int GREEN = LED_BUILTIN;

Notes notes(STARTING_NOTE_DISTANCE_FROM_MIDDLE_A);
KeyBuffer buffer;
Frequency frequency(glide, CYCLES_PER_GLIDE_MAX);

void blink(int count = 2, int wait = 200) {
  while (count >= 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(wait);
    digitalWrite(LED_BUILTIN, LOW);
    delay(wait);

    count = count - 1;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  blink();
}

void loop() {
  buffer.populate();

  // TODO: do this less often...
  octave = map(analogRead(OCTAVE_PIN), 0, 1023, 0, 4);
  glide = float(analogRead(GLIDE_PIN)) / 1023;

  if (printToSerial) {
    frequency.print();
  }

  if (buffer.isEmpty()) {
    if (!glideOnFreshKeyPresses) {
      frequency.reset();
    }

    noTone(SPEAKER_PIN);
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    frequency.update(
      notes.get(buffer.getFirst()) / 4 * pow(2, octave),
      glide
    );

    tone(SPEAKER_PIN, frequency.get());
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
