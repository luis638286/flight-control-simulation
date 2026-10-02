#include "domain/PIDController.h"

#include <algorithm>
#include <cassert>

PIDController::PIDController(float kp, float ki, float kd,
                             float outMin, float outMax)
    : kp_(kp), ki_(ki), kd_(kd), outMin_(outMin), outMax_(outMax) {
    assert(outMin < outMax);
}

float PIDController::update(float setpoint, float measured, float dt) {
    assert(dt > 0.0f);

    const float error = setpoint - measured;

    // derivative on measurement, not on error. A step change in the setpoint
    // would otherwise produce a one-cycle spike ("derivative kick")
    float derivative = 0.0f;
    if (!first_) {
        derivative = -(measured - prevMeasured_) / dt;
    }
    prevMeasured_ = measured;
    first_ = false;

    integral_ += error * dt;

    const float out     = kp_ * error + ki_ * integral_ + kd_ * derivative;
    const float clamped = std::clamp(out, outMin_, outMax_);

    // conditional integration (the "clamping" anti-windup method): while the
    // output is saturated, do not accumulate error that pushes further into
    // the limit. Error of the opposite sign still unwinds the integral, so
    // the controller recovers as soon as the error reverses
    const bool saturated      = (out != clamped);
    const bool pushingFurther = (error > 0.0f) == (out > outMax_);
    if (saturated && pushingFurther) {
        integral_ -= error * dt;
    }

    return clamped;
}

void PIDController::reset() {
    integral_     = 0.0f;
    prevMeasured_ = 0.0f;
    first_        = true;
}