# Learning sources

How the knowledge behind this project was acquired, and where each source shows up
in the design. Listed roughly in the order it was needed.

My approach throughout was to watch an explanation first, then immediately implement
a small version of it and check the result against something I could make.
Where a source and my implementation disagreed, the disagreement is noted.

---

## Control theory

### MATLAB — *Understanding PID Control*
<https://www.youtube.com/watch?v=NVLXCwc8HzM&list=PLn8PRpmsu08pQBgjxYFXSsODEF3Jqmm-y>

The primary source for the control law. Used for:

- What each of the three terms contributes, and why P alone leaves a steady-state
  error that only the integral term can remove.
- Integrator windup: why a saturated actuator causes the integral to grow without
  effect, and why the controller then responds late and overshoots.
- Anti-windup strategies. This playlist is where the **conditional integration**
  ("clamping") approach came from, which is what `PIDController::update()`
  implements.
- Actuator saturation and the reasoning for keeping margin below a physical limit.

Where it shows up: `src/domain/PIDController.cpp`, specifically the anti-windup
block and the choice of output limits.

**A point where I diverged from the video:** the videos set the saturation limit
slightly below the actuator's physical maximum. In this project the equivalent
concern is handled on the plant side instead (by varying `kThrust` in
`PhysicsModel`) rather than by lowering the controller's output limit, because a
lower controller limit constrains what is commanded without changing what the
simulated drone can actually deliver. The controller's `±1.0` range is a normalised
contract with the mixer, not a claim about hardware.

### MathWorks documentation — *Anti-Windup Control Using a PID Controller*
<https://www.mathworks.com/help/simulink/slref/anti-windup-control-using-a-pid-controller.html>

Used to confirm terminology. Simulink's "clamping" anti-windup option is
conditional integration, not clamping of the integrator value — two different
techniques that share a name in informal use. Also the source for back-calculation
as the alternative method.

### Wikibooks — *Control Systems / Feedback Loops*
<https://en.wikibooks.org/wiki/Control_Systems/Feedback_Loops>

Reference for terminology that the video sources assumed: plant, setpoint, error
signal, open versus closed loop, steady-state error, disturbance rejection. Used to
make the written chapters use the standard vocabulary rather than improvised terms.

---

## Drone dynamics and simulation

### MATLAB  — *Drone Simulation and Control*
<https://www.youtube.com/watch?v=hGcGPUqB67Q&list=PLy8TVa88QVfoOotVLloGWvK6PMSdjEtSb>

Used for:

- The cascaded control structure: an outer angle loop producing a rate setpoint for
  an inner rate loop, and why the inner loop must be faster than the outer one.
- Treating the three rotational axes as independent SISO loops, and the conditions
  under which that assumption holds.
- Motor mixing: converting a thrust demand plus three torque demands into four
  individual motor commands.
- Why a quadcopter is not passively stable and therefore needs continuous
  correction at a fixed rate.

Where it shows up: the structure of `FlightController` (five PID instances, two
modes), `Mixer`, and the axis-isolation tests in `tests/test_physics.cpp` which
verify the decoupling assumption the architecture depends on.

---

## C++

### Bro Code — *C++ Full Course*
<https://www.youtube.com/watch?v=-TkoO8Z07hI>

General language grounding: types, functions, pointers and references, headers
versus source files, scope and lifetime.

### Bro Code — *What is INHERITANCE in C++?*
<https://www.youtube.com/watch?v=5HYIFFuGcvk>

Base and derived classes, access specifiers.

### The Cherno — *C++ series*
<https://www.youtube.com/watch?v=fLgTtaqqJp0&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb>

The main C++ reference for this project. Episodes used:

| Topic | Where it shows up |
|---|---|
| Classes versus structs | `Types.h` uses structs for plain data; everything with behaviour is a class |
| Constructors and destructors | member-initialiser lists in `PIDController`; virtual destructors on every interface |
| Virtual functions | the `hal/` interfaces and their `sim/` implementations |
| Interfaces | `IClock`, `IImuSensor`, `IMotorOutput`, `IReceiver` |

### *Interfaces vs Abstract Classes / Inheritance*
<https://www.youtube.com/watch?v=5rs39FxBdcs>

Used to settle the distinction between an abstract class (has at least one pure
virtual method, may still carry data and implemented methods) and an interface (all
methods pure virtual, no data). The `hal/` headers follow the stricter interface
form, an interface carrying data would stop being a contract and start
being a base class with its own opinions.

---

## Tooling


### CMake
Project build configuration, including `FetchContent` to pull GoogleTest at
configure time so the repository has no vendored dependencies and builds from a
fresh clone with two commands.

---

## AI assistance

Claude (Anthropic) was used throughout as an interactive tutor and reviewer rather
than as a code generator. Typical uses:

- Explaining concepts the video sources assumed, in response to specific questions
  (what a monotonic clock is, why a fixed-rate loop is required, what saturation
  means in the mixer as distinct from in the controller).
- Reviewing code I had written and identifying faults — for example a public member
  that broke the monotonicity guarantee of `SimClock`, and an integer division in
  `seconds()` that silently truncated.
- Setting out design alternatives with their trade-offs (conditional integration
  versus back-calculation versus a fixed integral limit) for me to choose between.
- Diagnosing toolchain errors during the Windows/MSVC/CMake setup.
