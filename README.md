*This project has been created as part of the 42 curriculum by crappo*

# Description

Codexion is a project that simulates the Dining Philosophers problem using threads. Threads represent the philosophers (coders) and mutexes represent the forks (dongles).

Each coder cycles through the following states:
- Compile
- Debug
- Refactor

To compile, a coder must hold two dongles (left and right). If a coder does not start compiling before `time_to_burnout` expires, they burn out and the simulation stops.

The simulation ends when:
- A coder burns out, or
- All coders have compiled a required number of times (if specified).

# Instructions

## Compilation

```
make
```

## Usage

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

### Arguments

- number_of_coders: Number of coders (and dongles).
- time_to_burnout: Maximum time (in ms) without compiling before burnout.
- time_to_compile: Time (in ms) spent compiling.
- time_to_debug: Time (in ms) spent debugging.
- time_to_refactor: Time (in ms) spent refactoring.
- number_of_compiles_required: (Optional) Minimum number of compiles per coder before stopping.
- dongle_cooldown: Time (in ms) a dongle remains unavailable after being released.
- scheduler: Scheduling policy (fifo or edf).

# Some Possible Tests

No burnout:
```
./codexion 4 410 200 100 100 0 0 fifo
```

Immediate burnout:
```
./codexion 1 800 200 200 200 1 0 fifo
```

EDF scheduling test:
```
./codexion 5 800 200 200 200 7 0 edf
```

Cooldown test:
```
./codexion 4 800 200 100 100 5 50 edf
```

# Blocking Cases Handled

- Deadlock prevention using an asymmetric fork acquisition strategy (even IDs pick right first, odd IDs pick left first).
- Protection against data races using dedicated mutexes for shared resources.
- Dongle cooldown system preventing immediate reuse after release.
- Burnout detection handled by a monitoring mechanism.
- Fair resource arbitration using FIFO or EDF scheduling policies.

# Thread Synchronization Mechanisms

- One mutex per dongle to protect access.
- A mutex for logging to prevent mixed console output.
- A mutex protecting the global simulation state.
- Condition variables to block threads efficiently while waiting for dongles or cooldown expiration.
- A monitoring thread to continuously check burnout conditions.