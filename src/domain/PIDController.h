#pragma once

// single-axis PID controller with output clamping and anti-windup
//
// pure maths: no hardware, no clock, no simulator dependency. dt is supplied
// by the caller, so the controller stays a pure function of its inputs and
// the control loop keeps ownership of time

class PIDController {
public:
    PIDController(float kp, float ki, float kd, float outMin, float outMax);

    // runs one control cycle
    //   setpoint  what we want        (same unit as measured)
    //   measured  what we have        (e.g. deg/s for a rate loop)
    //   dt        seconds since the previous call; must be > 0
    // Returns the control output, clamped to [outMin, outMax]
    float update(float setpoint, float measured, float dt);

    // clears accumulated state, Call on mode entry and on arming, so a stale
    // integral cannot kick the motors after a period of inactivity.
    void reset();

private:
    // gains — fixed for the life of the object
    float kp_;
    float ki_;
    float kd_;

    // output limits — the range the actuator can actually accept
    float outMin_;
    float outMax_;

    // state carried between cycles
    float integral_     = 0.0f;   // error-seconds
    float prevMeasured_ = 0.0f;
    bool  first_        = true;   // no valid previous measurement yet
};