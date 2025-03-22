#include "NewTone.h"
#include "KeyBuffer.h"
#include "Arduino.h"

uint16_t speaker_preload[BUFFER_MAX] = {0};

// Note: rolling this into a loop causes us to miss the timing.
ISR(TIMER2_OVF_vect) // 16 MHz / 256 = 62.5 kHz
{
  static int16_t speaker_ctr[BUFFER_MAX] = {0};

  if (speaker_preload[0] != 0) {
    speaker_ctr[0] -= INT_US;
    while (speaker_ctr[0] <= 0) {
      PINB |= SPEAKER_MASK[0];
      speaker_ctr[0] += speaker_preload[0];
    }
  }

  if (speaker_preload[1] != 0) {
    speaker_ctr[1] -= INT_US;
    while (speaker_ctr[1] <= 0) {
      PINB |= SPEAKER_MASK[1];
      speaker_ctr[1] += speaker_preload[1];
    }
  }

  if (speaker_preload[2] != 0) {
    speaker_ctr[2] -= INT_US;
    while (speaker_ctr[2] <= 0) {
      PINB |= SPEAKER_MASK[2];
      speaker_ctr[2] += speaker_preload[2];
    }
  }

  if (speaker_preload[3] != 0) {
    speaker_ctr[3] -= INT_US;
    while (speaker_ctr[3] <= 0) {
      PINB |= SPEAKER_MASK[3];
      speaker_ctr[3] += speaker_preload[3];
    }
  }
}

void newToneSetup() {
  noInterrupts();
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2B |= (0 << CS22) | (0 << CS21) | (1 << CS20); // no prescaler -- interrupt every 256 ticks at 16 MHz = 62.5 kHz
  TIMSK2 |= (1 << TOIE2); // enable timer overflow interrupt
  interrupts();

  for (uint8_t i = 0; i < BUFFER_MAX; i++) {
    pinMode(SPEAKER_PINS[i], OUTPUT);
  }
}

void loadTone(uint8_t oscillator, uint16_t period) {
  if (oscillator < BUFFER_MAX) {
    speaker_preload[oscillator] = period;
  }
}