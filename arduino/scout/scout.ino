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

uint8_t speaker_a_preload = 0;
uint8_t speaker_b_preload = 0;
uint8_t speaker_c_preload = 0;
uint8_t speaker_d_preload = 0;

ISR(TIMER2_OVF_vect) // currently 16 MHz / 256 = 62.5 kHz
{
  static uint8_t speaker_a_ctr = 1;
  static uint8_t speaker_b_ctr = 1;
  static uint8_t speaker_c_ctr = 1;
  static uint8_t speaker_d_ctr = 1;

  if (speaker_a_preload != 0) {
    if (--speaker_a_ctr == 0) {
      PINB |= SPEAKER_A_MASK;
      speaker_a_ctr = speaker_a_preload;
    }
  }

  if (speaker_b_preload != 0) {
    if (--speaker_b_ctr == 0) {
      PINB |= SPEAKER_B_MASK;
      speaker_b_ctr = speaker_b_preload;
    }
  }

  if (speaker_c_preload != 0) {
    if (--speaker_c_ctr == 0) {
      PINB |= SPEAKER_C_MASK;
      speaker_c_ctr = speaker_c_preload;
    }
  }

  if (speaker_d_preload != 0) {
    if (--speaker_d_ctr == 0) {
      PINB |= SPEAKER_D_MASK;
      speaker_d_ctr = speaker_d_preload;
    }
  }

  // TCNT2 = {potentially preload instead of nasty hack};
}

//TODO: Dynamically calculate these, potentially dithering to get to correct frequency. Need to think about this.
//      The precision is such that this gets noticably out of tune across two octaves.
//      Because I am using a table right now, glides are also broken.
uint8_t freqTable[25] = {239, 225, 213, 201, 190, 179, 169, 159, 150, 142, 134, 127, 119, 113, 106, 100, 95, 89, 84, 80, 75, 71, 67, 63, 60}; // counts for C3-C5 at a base of 61.5 kHz

// END TODO BLOCK

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

  noInterrupts();
  TCCR2A = 0;
  TCCR2B = 0;
 // TCNT2 = {something}; // potentially could preload the timer
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

    //noTone(SPEAKER_PIN);
    digitalWrite(PLAYING_INDICATOR_LED, LOW);
  } else {
    //frequency.update(notes.get(buffer.getFirst()) / 4 * pow(2, octave), glide);

    //tone(SPEAKER_PIN, frequency.get());
   
    digitalWrite(PLAYING_INDICATOR_LED, HIGH);
  }

  uint8_t size = buffer.getSize();
  speaker_a_preload = (size > 0) ? freqTable[buffer.getElement(0)] : 0;
  speaker_b_preload = (size > 1) ? freqTable[buffer.getElement(1)] : 0;
  speaker_c_preload = (size > 2) ? freqTable[buffer.getElement(2)] : 0;
  speaker_d_preload = (size > 3) ? freqTable[buffer.getElement(3)] : 0;
}

