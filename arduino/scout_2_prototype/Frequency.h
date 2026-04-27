#ifndef Frequency_h
#define Frequency_h

#include "Arduino.h"
#include "Notes.h"

const int GLIDE_MAX = 9;

class Frequency {
public:
  Frequency(int glide, int cyclesPerGlideMax);
  void update(float target, int glide);
  float get();
  uint16_t getHalfPeriod();
  void reset();
  void print();

private:
  float _frequency = 0;
  uint16_t _halfPeriodMicroseconds = 0;
  float _glide;
  float _glideStep;
  float _previousTarget;
  float _target;
  int _cyclesPerGlideMax;
};

#endif
