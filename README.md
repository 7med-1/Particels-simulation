# Particle Collision Simulator (Raylib)

A simple 2D particle simulation written in C using the raylib graphics library.
This project demonstrates basic physics concepts such as movement, boundary
collisions, and elastic circle to circle collisions.

---

## Overview

The program opens a window and simulates multiple particles moving freely.
Each particle is represented as a circle with a position, radius, and velocity.
Particles bounce off the window edges and collide with each other realistically.

---

## Features

- Real time 2D simulation
- 100 moving particles
- Elastic particle to particle collisions
- Boundary collision with screen edges
- Stable collision response using vector math
- Written in pure C with raylib

---

## How It Works

Each particle contains:
- Position (x, y)
- Radius
- Velocity (vx, vy)

Every frame, the following steps happen:
1. Update particle positions using velocity
2. Handle collisions with window borders
3. Detect circle to circle collisions
4. Resolve overlaps
5. Recalculate velocities using normal and tangent vectors

All particles have equal mass and collisions are perfectly elastic.

---

## Collision Physics

- Collisions are detected using CheckCollisionCircles
- A normal vector is calculated between particle centers
- Velocity is split into normal and tangent components
- Normal components are swapped
- Final velocity is reconstructed from the new components

This approach produces stable and realistic results.

---

## Requirements

- C compiler (gcc or clang)
- raylib library installed

---


### Linux / macOS

```bash
gcc main.c -o particles -lraylib -lm
./particles
```
Windows (MSYS2)

```bash
gcc main.c $(pkg-config --cflags --libs raylib) -o particles
particles.exe
```


Configuration

You can modify the following values in the code:

-NUM_PARTICELS to change particle count

-WIDTH and HEIGHT for window size

-Particle radius and velocity range


In addition to circular particles, the project also includes heart shaped particles implemented in a separate source file. These heart shapes are generated procedurally using a mathematical parametric equation and rendered as connected line segments. For collision handling and movement, each heart is approximated using a circular bounding radius, allowing the same physics system used for particles to be reused while keeping the visual heart shape. This approach keeps the simulation fast, stable, and easy to extend.

