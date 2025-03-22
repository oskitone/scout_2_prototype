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

const int OCTAVE_PIN = A4;
const int GLIDE_PIN = A5;

const int PLAYING_INDICATOR_LED = 13; // ie LED_BUILTIN
const int FUNCTION_INDICATOR_LED = A3;

Notes notes(STARTING_NOTE_DISTANCE_FROM_MIDDLE_A);
KeyBuffer buffer;
Frequency frequency[BUFFER_MAX] = { {glide, CYCLES_PER_GLIDE_MAX}, {glide, CYCLES_PER_GLIDE_MAX}, {glide, CYCLES_PER_GLIDE_MAX}, {glide, CYCLES_PER_GLIDE_MAX} };

// TODO: move all of this into another file
const int SPEAKER_A = 9;
const int SPEAKER_B = 10;
const int SPEAKER_C = 11;
const int SPEAKER_D = 12;

const uint8_t SPEAKER_A_MASK = 0x02;
const uint8_t SPEAKER_B_MASK = 0x04;
const uint8_t SPEAKER_C_MASK = 0x08;
const uint8_t SPEAKER_D_MASK = 0x10;

// TODO: Move all of this below into another file to abstract away the replacement for tone()

uint8_t speaker_preload[BUFFER_MAX] = {0};

ISR(TIMER2_OVF_vect) // currently 16 MHz / 256 = 62.5 kHz
{
  static uint8_t speaker_a_ctr = 1;
  static uint8_t speaker_b_ctr = 1;
  static uint8_t speaker_c_ctr = 1;
  static uint8_t speaker_d_ctr = 1;

  if (speaker_preload[0] != 0) {
    if (--speaker_a_ctr == 0) {
      PINB |= SPEAKER_A_MASK;
      speaker_a_ctr = speaker_preload[0];
    }
  }

  if (speaker_preload[1] != 0) {
    if (--speaker_b_ctr == 0) {
      PINB |= SPEAKER_B_MASK;
      speaker_b_ctr = speaker_preload[1];
    }
  }

  if (speaker_preload[2] != 0) {
    if (--speaker_c_ctr == 0) {
      PINB |= SPEAKER_C_MASK;
      speaker_c_ctr = speaker_preload[2];
    }
  }

  if (speaker_preload[3] != 0) {
    if (--speaker_d_ctr == 0) {
      PINB |= SPEAKER_D_MASK;
      speaker_d_ctr = speaker_preload[3];
    }
  }
}

// END TODO BLOCK

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

  noInterrupts();
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2B |= (0 << CS22) | (0 << CS21) | (1 << CS20); // no prescaler
  TIMSK2 |= (1 << TOIE2); // enable timer overflow interrupt
  interrupts();   
  pinMode(SPEAKER_A, OUTPUT);
  pinMode(SPEAKER_B, OUTPUT);
  pinMode(SPEAKER_C, OUTPUT);
  pinMode(SPEAKER_D, OUTPUT);

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
    speaker_preload[i] = frequency[i].getTicks();
  }
  for (; i < BUFFER_MAX; i++) {
    if (!glideOnFreshKeyPresses) {
      frequency[i].reset();
    }
    speaker_preload[i] = 0;
  }
}

