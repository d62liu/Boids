# Boids Simulation

## Project Overview

Boids is an artificial life program, developed by Craig Reynolds in 1986, that simulates the flocking behavior of birds. The core principles of the Boids algorithm are:

- **Separation:** Steer to avoid crowding local flockmates.  
- **Alignment:** Steer towards the average heading of local flockmates.  
- **Cohesion:** Steer to move toward the average position (center of mass) of local flockmates.  

This project implements these principles in C++ and provides real-time visualization using the SDL2 library. Additionally, it incorporates a quadtree data structure to efficiently manage spatial partitioning, enhancing the performance of neighborhood queries essential for the Boids algorithm.

---

## Features

- **Real-Time Visualization:** Utilizes SDL2 to render the simulation, allowing users to observe the flocking behavior as it evolves.  
- **Quadtree Optimization:** Implements a quadtree to optimize spatial queries, improving the efficiency of determining nearby boids.  
- **Modular Design:** The codebase is organized into distinct modules, facilitating understanding and potential extensions.  

---

## Installation and Setup

To set up and run the Boids simulation on your local machine, follow these steps:

### 1. Clone the Repository

```bash
git clone https://github.com/d62liu/Boids.git
```

### 2. Install SDL2

Ensure that the SDL2 library is installed on your system. SDL2 is a cross-platform development library providing low-level access to audio, keyboard, mouse, and graphics hardware. You can download it from the [official SDL website](https://www.libsdl.org/download-2.0.php).

### 3. Build the Project

Navigate to the project directory and use the provided `Makefile` to build the project:

```bash
cd Boids
make
```

This will compile the source files and generate the executable.

### 4. Run the Simulation

```bash
./main
```

This will launch the simulation window, displaying the boids exhibiting flocking behavior.

---

## Code Structure

- **`src/` Directory:** Contains the main source files for the simulation.
  - `boid_main.cpp` and `boid_main.h`: Define the core logic for initializing and running the simulation.
  - `boid_interface.cpp` and `boid_interface.h`: Handle the interface between the simulation logic and the SDL2 visualization components.
- **`Makefile`:** Provides build instructions to compile the project.
- **`SDL2.dll`:** Includes the SDL2 dynamic link library for convenience on Windows.
- **`README.md`:** This documentation file.

---

## Dependencies

- **SDL2:** Simple DirectMedia Layer (SDL) version 2.x is required for rendering the simulation. Make sure it is installed and accessible in your system's library path.

---

## Customization

The simulation parameters—such as the number of boids, their speed, and interaction radii—can be adjusted within the source files. Modifying these values allows you to experiment with different flocking behaviors and dynamics.

---

## Troubleshooting

- **SDL2 Issues:** If the application fails to launch due to missing SDL2 dependencies, make sure SDL2 is correctly installed and your system’s library path includes the SDL2 binaries.
- **Compilation Errors:** Ensure your development environment includes a compatible C++ compiler and that all required source files are present.

---

## License

This project is licensed under the MIT License.



