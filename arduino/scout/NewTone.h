#ifndef NewTone_h
#define NewTone_h

#include "KeyBuffer.h"
#include "Arduino.h"

const int SPEAKER_PINS[BUFFER_MAX] = {9, 10, 11, 12};
const uint8_t SPEAKER_MASK[BUFFER_MAX] = {0x02, 0x04, 0x08, 0x10};

const uint8_t INT_US = 16; // uSec = 256 counts in an 8-bit counter / 16 MHz

extern uint16_t speaker_preload[BUFFER_MAX];

void newToneSetup();
void loadTone(uint8_t oscillator, uint16_t period);

#endif
