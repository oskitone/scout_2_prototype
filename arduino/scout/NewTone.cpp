#include "NewTone.h"
#include "Arduino.h"
#include "KeyBuffer.h"

uint16_t speaker_preload[OSCILLATORS_MAX] = {0};

// Note: rolling this into a loop causes us to miss the timing.

// 16 MHz / 256 = 62.5 kHz
ISR(TIMER2_OVF_vect) {
  static int16_t speaker_counter[OSCILLATORS_MAX] = {0};

  if (speaker_preload[0] != 0) {
    speaker_counter[0] -= MICROSECONDS_PER_TIMER_INTERRUPT;
    while (speaker_counter[0] <= 0) {
      PINB |= SPEAKER_MASK[0];
      speaker_counter[0] += speaker_preload[0];
    }
  }

  if (speaker_preload[1] != 0) {
    speaker_counter[1] -= MICROSECONDS_PER_TIMER_INTERRUPT;
    while (speaker_counter[1] <= 0) {
      PINB |= SPEAKER_MASK[1];
      speaker_counter[1] += speaker_preload[1];
    }
  }

  if (speaker_preload[2] != 0) {
    speaker_counter[2] -= MICROSECONDS_PER_TIMER_INTERRUPT;
    while (speaker_counter[2] <= 0) {
      PINB |= SPEAKER_MASK[2];
      speaker_counter[2] += speaker_preload[2];
    }
  }

  if (speaker_preload[3] != 0) {
    speaker_counter[3] -= MICROSECONDS_PER_TIMER_INTERRUPT;
    while (speaker_counter[3] <= 0) {
      PINB |= SPEAKER_MASK[3];
      speaker_counter[3] += speaker_preload[3];
    }
  }
}

void newToneSetup() {
  noInterrupts();

  TCCR2A = 0;
  TCCR2B = 0;

  // no prescaler -- interrupt every 256 ticks at 16 MHz = 62.5 kHz
  TCCR2B |= (0 << CS22) | (0 << CS21) | (1 << CS20);

  // enable timer overflow interrupt
  TIMSK2 |= (1 << TOIE2);

  interrupts();

  for (uint8_t i = 0; i < OSCILLATORS_MAX; i++) {
    pinMode(SPEAKER_PINS[i], OUTPUT);
  }
}

void loadTone(uint8_t oscillator, uint16_t halfPeriod) {
  if (oscillator < OSCILLATORS_MAX) {
    speaker_preload[oscillator] = halfPeriod;
  }
}