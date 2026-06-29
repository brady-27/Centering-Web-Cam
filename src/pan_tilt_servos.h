#pragma once

#include <Arduino.h>
#include "config.h"

// controls the two sg90 servos for pan and tilt
// uses esp32 ledc pwm instead of the arduino servo library
// standard hobby servos want 50hz signal
// 1000us pulse = 0 degrees, 2000us pulse = 180 degrees

#define SERVO_FREQ      50
#define SERVO_RES_BITS  16
#define SERVO_MIN_US    1000
#define SERVO_MAX_US    2000
#define SERVO_PERIOD_US 20000  // 1/50hz = 20ms = 20000us

class PanTiltServos {
public:
    PanTiltServos(int panPin, int tiltPin) {
        _panPin = panPin;
        _tiltPin = tiltPin;
        _panAngle = PAN_CENTER_ANGLE;
        _tiltAngle = TILT_CENTER_ANGLE;
    }

    void begin() {
        // attach both servos
        ledcAttach(_panPin, SERVO_FREQ, SERVO_RES_BITS);
        ledcAttach(_tiltPin, SERVO_FREQ, SERVO_RES_BITS);

        // start in the middle
        setPan(_panAngle);
        setTilt(_tiltAngle);
        Serial.println("servos ready");
    }

    void setPan(float angle) {
        // keep it within limits
        if (angle < PAN_MIN_ANGLE) angle = PAN_MIN_ANGLE;
        if (angle > PAN_MAX_ANGLE) angle = PAN_MAX_ANGLE;
        _panAngle = angle;
        ledcWrite(_panPin, angleToDuty(angle));
    }

    void setTilt(float angle) {
        if (angle < TILT_MIN_ANGLE) angle = TILT_MIN_ANGLE;
        if (angle > TILT_MAX_ANGLE) angle = TILT_MAX_ANGLE;
        _tiltAngle = angle;
        ledcWrite(_tiltPin, angleToDuty(angle));
    }

    // move relative to current position
    void adjustPan(float delta) {
        setPan(_panAngle + delta);
    }

    void adjustTilt(float delta) {
        setTilt(_tiltAngle + delta);
    }

    // go back to middle
    void center() {
        setPan(PAN_CENTER_ANGLE);
        setTilt(TILT_CENTER_ANGLE);
        Serial.println("servos centered");
    }

    float getPan()  { return _panAngle; }
    float getTilt() { return _tiltAngle; }

private:
    int _panPin, _tiltPin;
    float _panAngle, _tiltAngle;

    // convert angle to the pwm duty value
    // math: figure out the pulse width, then scale to 16 bit duty
    uint32_t angleToDuty(float angle) {
        float pulseUs = SERVO_MIN_US + (angle / 180.0f) * (SERVO_MAX_US - SERVO_MIN_US);
        uint32_t maxDuty = (1 << SERVO_RES_BITS) - 1;
        return (uint32_t)((pulseUs / SERVO_PERIOD_US) * maxDuty);
    }
};
