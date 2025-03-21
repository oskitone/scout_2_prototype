#include "KeyBuffer.h"
#include "Arduino.h"

#define CIRCULAR_BUFFER_DEBUG
#include <CircularBuffer.hpp>
#include <Keypad.h>

const byte ROWS = 5;
const byte COLS = 5;
byte key_indexes[ROWS][COLS] = {{1, 6, 11, 16, 21},
                                {2, 7, 12, 17, 22},
                                {3, 8, 13, 18, 23},
                                {4, 9, 14, 19, 24},
                                {5, 10, 15, 20, 25}};
byte rowPins[ROWS] = {7, 8, 14, 15, 16};
byte colPins[COLS] = {2, 3, 4, 5, 6};

Keypad _buttons = Keypad(makeKeymap(key_indexes), rowPins, colPins, ROWS, COLS);

KeyBuffer::KeyBuffer() {
  CircularBuffer<int, BUFFER_MAX> _buffer;

  const int KEYPAD_LIBRARY_MINIMUM_DEBOUNCE = 1;
  _buttons.setDebounceTime(KEYPAD_LIBRARY_MINIMUM_DEBOUNCE);
}

bool KeyBuffer::isEmpty() { return _buffer.isEmpty(); }

bool KeyBuffer::isInBuffer(int c) {
  bool value = false;

  if (!_buffer.isEmpty()) {
    for (int i = 0; i < _buffer.size(); i++) {
      if (c == _buffer[i]) {
        value = true;
        break;
      }
    }
  }

  return value;
}

bool KeyBuffer::removeFromBuffer(int c) {
  int newStack[BUFFER_MAX - 1] = {};
  int newI = 0;

  bool hasRemoval = false;

  for (int i = 0; i < _buffer.size(); i++) {
    if (c != _buffer[i]) {
      newStack[newI++] = _buffer[i];
    } else {
      hasRemoval = true;
    }
  }

  if (hasRemoval) {
    _buffer.clear();

    for (byte i = 0; i < newI; i++) {
      _buffer.push(newStack[i]);
    }
  }
}

void KeyBuffer::populate() {
  bool isActive = false;

  _buttons.getKeys(); // populate keys

  for (int i = LIST_MAX-1; i >= 0; i--) {
    byte kstate = _buttons.key[i].kstate;
    byte kchar = _buttons.key[i].kchar - 1;

    if (kstate == PRESSED || kstate == HOLD) {
      if (!isInBuffer(kchar)) {
        _buffer.unshift(kchar);
      }

      isActive = true;
    } else if (isInBuffer(kchar)) {
      removeFromBuffer(kchar);
    }
  }

  if (!isActive) {
    _buffer.clear();
  }
}

void KeyBuffer::print() {
  if (!_buffer.isEmpty()) {
    Serial.print("[");
    for (int i = 0; i < _buffer.size(); i++) {
      Serial.print(_buffer[i]);

      if (i < _buffer.size() - 1) {
        Serial.print(",");
      }
    }
    Serial.print("] (" + String(_buffer.size()) + "/" + String(BUFFER_MAX) +
                 ")");
    Serial.println();
  }
}

char KeyBuffer::getFirst() { return _buffer.first(); }

uint8_t KeyBuffer::getSize() { return _buffer.size(); }

char KeyBuffer::getElement(uint8_t e) { return _buffer[e]; }
