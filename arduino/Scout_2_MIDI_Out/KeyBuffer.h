#define CIRCULAR_BUFFER_DEBUG
#include <CircularBuffer.hpp>
#include <Keypad.h>

#ifndef KeyBuffer_h
#define KeyBuffer_h

#include "Arduino.h"

#define BUFFER_MAX 25  // increased to handle all keys. 6 note chords are necessary.

class KeyBuffer {
public:
  KeyBuffer();
  bool isEmpty();
  char getFirst();
  void print();
  void printBuffer();
  void populate();
  uint8_t getSize();
  char getElement(uint8_t e);

private:
  CircularBuffer<int, BUFFER_MAX> _buffer;
  bool isInBuffer(int c);
  bool removeFromBuffer(int c);
};

#endif
