#include "Frequency.h"
#include "Arduino.h"
#include "Notes.h"

Frequency::Frequency(float glide, int cyclesPerGlideMax) {
  _glide = glide;
  _cyclesPerGlideMax = cyclesPerGlideMax;
}

float Frequency::get() { return _frequency; }

uint16_t Frequency::getTicks() { return _ticks; }

uint16_t Frequency::getPeriod() { return _period; }

void Frequency::update(float target, float glide) {
  _target = target;
  bool needsUpdate = _frequency != _target;

  if (needsUpdate) {
    if ((_frequency == 0) || (glide == 0)) {
      _frequency = _target;
    } else {
      if (_target != _previousTarget) {
        _glideStep =
            abs(_target - _previousTarget) / (glide * _cyclesPerGlideMax);
      }

      _frequency = (_target > _frequency)
                       ? min(_target, _frequency + _glideStep)
                       : max(_target, _frequency - _glideStep);
    }
    _ticks = (uint16_t)((31250.0f / _frequency) + 0.5f);
    _period = (uint16_t)((500000.0f / _frequency) + 0.5f); //uSec
  }

  if (!needsUpdate) {
    _previousTarget = _target;
  }
}

void Frequency::reset() { _frequency = 0; _ticks = 0; _period = 0; }

void Frequency::print() {
  Serial.println("frequency:" + String(_frequency) +
                 ",period:" + String(_period) +
                 ",target:" + String(_target) +
                 ",previousTargetFrequency:" + String(_previousTarget));
}
