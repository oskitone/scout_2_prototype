#ifndef NewTone_h
#define NewTone_h

#include "KeyBuffer.h"
#include "Arduino.h"

#define OSCILLATORS_MAX BUFFER_MAX

const int SPEAKER_PINS[OSCILLATORS_MAX] = {9, 10, 11, 12};
const uint8_t SPEAKER_MASK[OSCILLATORS_MAX] = {0x02, 0x04, 0x08, 0x10};

// 256 counts in an 8-bit counter / ATmega328's 16 MHz clock speed.
// The interrupt happens every time this timer overflows, which is at every
// 1/(16/256 MHz) or 16 microseconds.
const uint8_t MICROSECONDS_PER_TIMER_INTERRUPT = 16;

extern uint16_t speaker_preload[OSCILLATORS_MAX];

void newToneSetup();
void loadTone(uint8_t oscillator, uint16_t halfPeriod);

#endif
