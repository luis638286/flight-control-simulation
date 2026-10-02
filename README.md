# Flight Control Simulation Testbed

A modular C++ flight control system for a quadcopter, developed and verified
entirely in simulation.

The deliverable is the **flight controller**, not the simulator. All hardware is
reached through a hardware abstraction layer, so the same control code runs against
a simulated plant or against real sensors and motors — the simulated world is just
one implementation of the interfaces the controller depends on.

Fontys HBO-ICT, semester 3 individual project (Intelligent Devices).

---

## What it does

Each 2 ms cycle, the controller reads the IMU, estimates attitude, compares it with
what the pilot asked for, and drives four motors:

```
receiver ──► setpoints ──► [angle PID] ──► [rate PID] ──► mixer ──► 4 motors
                                 ▲              ▲
                          attitude estimate  gyro rate
                                 └──── IMU ─────┘
```

Two flight modes:

| Mode | Stick means | Released sticks |
|---|---|---|
| Manual | a rotation rate | stop rotating, hold current attitude |
| Stabilised | a tilt angle | return to level |

---

## Why it is structured this way

```
src/
├── Types.h      shared data types — depends on nothing
├── hal/         interfaces only: IImuSensor, IMotorOutput, IReceiver, IClock
├── domain/      the flight controller — PIDs, attitude estimator, mixer
├── sim/         a fake world: physics model, simulated sensors and motors
├── infra/       CSV logging
└── app/         composition root: builds the objects and runs the loop
```

Each layer may only depend on the ones below it. `domain/` never includes anything
from `sim/`, which is what makes the portability claim checkable rather than
aspirational:


---

## Build

Requires CMake 3.16+ and a C++17 compiler. GoogleTest is fetched automatically at
configure time, so there are no dependencies.


---

## Verification

The plant model is verified against analytic cases *before* any controller is
tuned against it, so that later misbehaviour can be attributed to the control law
rather than to the simulation:

| Case | Expected |
|---|---|
| Free fall for 1 s | −9.81 m/s vertical velocity |
| Hover throttle | altitude held |
| Roll demand only | roll changes, pitch and yaw do not |
| Pitch demand only | pitch changes, roll and yaw do not |
| Yaw demand only | yaw changes, roll and pitch do not |

The axis-isolation cases justify treating the three rotational axes as independent
single-input loops, which is the assumption the controller's structure rests on.

---

## Status

- [x] Simulated clock and fixed-rate loop
- [ ] PID controller with anti-windup
- [ ] Motor mixer
- [ ] Physics model
- [ ] HAL interfaces and simulated devices
- [ ] Complementary filter attitude estimator
- [ ] Flight controller — Manual mode
- [ ] CSV logging and plots
- [ ] Stabilised mode
- [ ] Guarded mode switching
- [ ] Scenarios and gain tuning

---

## Known limitations

- Saturation occurring inside the mixer is not reported back to the PID
  controllers, so integrator windup remains possible at high throttle combined
  with large attitude demands.
- The controller tells each motor how hard to spin, but never learns how fast it actually spun. It corrects using the gyro instead. This matches most real flight controllers, where the speed controllers only receive commands and send nothing back.
- The simulation assumes a perfectly periodic 2 ms cycle with zero computation
  time. Results validate the control law, not its real-time schedulability.
- The physics model treats roll, pitch and yaw as completely independent. On a real drone, rotating quickly on two axes at once produces a small twisting force on the third; this effect is not simulated.

---

## Documentation

- [`docs/sources.md`](docs/sources.md) — learning sources and where each one
  informed the design