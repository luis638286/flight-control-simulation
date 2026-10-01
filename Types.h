#pragma once

// one sample from the inertial measurement unit
struct ImuSample {
    float gx = 0.0f, gy = 0.0f, gz = 0.0f;   // angular rate, deg/s, body axes
    float ax = 0.0f, ay = 0.0f, az = 0.0f;   // specific force, m/s^2, body axes
};

struct attitude {
    float roll = 0.0f;   // deg, positive = right side down
    float pitch = 0.0f;  // deg, positive = nose up
};