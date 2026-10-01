Flight Control Simulation Testbed

A modular C++ flight control system for a quadcopter, developed and tested
entirely in simulation. Fontys HBO-ICT, semester 3 individual SD project.

The flight control software reads sensors, estimates attitude, runs a
cascaded PID structure and drives four motors. All devices are reached
through a hardware abstraction layer, so the simulated sensors and motors
can be replaced by real drivers without changing the control code.