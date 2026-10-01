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


// how the system interprets the stick channels
enum class Mode { Manual, Stabilised };

// one frame from the receiver: stick positions, not physical units.
struct ReceiverFrame {
    float roll = 0.0f;      // -1 .. +1   right stick, horizontal
    float pitch = 0.0f;     // -1 .. +1   right stick, vertical
    float yaw = 0.0f;       // -1 .. +1   left stick, horizontal
    float throttle = 0.0f;  //  0 .. +1   left stick, vertical
    Mode  modeSwitch = Mode::Manual;
};

// what the controller asks of the airframe, before mixing
struct Demand {
    float roll = 0.0f, pitch = 0.0f, yaw = 0.0f;   // torque demands, -1 .. +1
    float throttle = 0.0f;                          //  0 .. +1
};

// what each motor is told to do
struct MotorCommands {
    float frontLeft = 0.0f, frontRight = 0.0f;
    float rearLeft = 0.0f,  rearRight = 0.0f;      // each 0 .. +1
};