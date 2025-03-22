#include "NewTone.h"
#include "KeyBuffer.h"
#include "Arduino.h"

uint16_t speaker_preload[BUFFER_MAX] = {0};

// Note: rolling this into a loop causes us to miss the timing.
ISR(TIMER2_OVF_vect) // currently 16 MHz / 256 = 62.5 kHz
{
  static uint16_t speaker_a_ctr = 1;
  static uint16_t speaker_b_ctr = 1;
  static uint16_t speaker_c_ctr = 1;
  static uint16_t speaker_d_ctr = 1;

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

void newToneSetup() {
  noInterrupts();
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2B |= (0 << CS22) | (0 << CS21) | (1 << CS20); // no prescaler
  TIMSK2 |= (1 << TOIE2); // enable timer overflow interrupt
  interrupts();

  for (uint8_t i = 0; i < BUFFER_MAX; i++) {
    pinMode(SPEAKER_PINS[i], OUTPUT);
  }
}
