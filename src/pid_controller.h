#pragma once

// pid controller
// watches how far off center the face is and
// figures out how much to move the servo
// learned about this from a youtube video lol

class PIDController {
public:
    PIDController(float kp, float ki, float kd, float outMin, float outMax) {
        _kp = kp;
        _ki = ki;
        _kd = kd;
        _outMin = outMin;
        _outMax = outMax;
        _integral = 0;
        _prevError = 0;
        _firstRun = true;
    }

    float update(float error, float dt) {
        // p - react to current error
        float p = _kp * error;

        // i - build up over time to fix small offsets
        _integral += error * dt;
        float i = _ki * _integral;

        // d - smooth out sudden changes
        float d = 0;
        if (!_firstRun && dt > 0) {
            d = _kd * ((error - _prevError) / dt);
        }

        _prevError = error;
        _firstRun = false;

        float output = p + i + d;

        // dont let it go too crazy
        if (output > _outMax) output = _outMax;
        if (output < _outMin) output = _outMin;

        // anti windup - stop integral stacking when maxed out
        if (output == _outMax || output == _outMin) {
            _integral -= error * dt;
        }

        return output;
    }

    // call this when no face is detected
    // so the values dont build up and cause issues
    void reset() {
        _integral = 0;
        _prevError = 0;
        _firstRun = true;
    }

private:
    float _kp, _ki, _kd;
    float _outMin, _outMax;
    float _integral;
    float _prevError;
    bool _firstRun;
};
