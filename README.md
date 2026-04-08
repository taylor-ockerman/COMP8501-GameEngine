# Sandy Sim - COMP 8051 Game Engine Project

A custom 2D game engine and sandbox project built in C++ using SDL3 for BCIT’s game architecture course. The project combines a traditional entity-component-system architecture with a grid-based particle simulation, allowing the player to move through a side-scrolling world while spawning and interacting with materials such as sand, water, smoke, stone, wood, oil, steam, and fire. The engine TMX maps for level data, SDL_ttf for UI text, SDL_image for textures, SDL_mixer for audio, and TinyXML2 for asset and map parsing.

At the core of the project is an ECS-style world manager. Entities are lightweight containers, while behavior is implemented through systems such as keyboard input, gravity, movement, collider synchronization, particle simulation, particle syncing, particle interaction, collision, animation, camera, HUD, UI rendering, and destruction. During each frame, the world updates these systems in order, then processes audio events, prepares render state, synchronizes deferred entities, and removes destroyed ones.

The gameplay scene is built from a TMX map and a particle grid. When a gameplay scene loads, the engine creates wall colliders from the map data, initializes a camera, spawns the player entity, and creates UI elements such as the particle counter, menu overlay, and brush HUD. A `BrushState` entity stores the currently selected particle, brush size, and mouse position. Mouse input updates the brush in world space, while keyboard input handles player movement, jumping, material selection, menu toggling, and quitting. This gives the project its hybrid feel: part platform-style game engine, part falling-sand sandbox. 

The particle system is one of the main engine features. Particles are stored in a grid and updated by behavior type: powders and liquids are processed bottom-up, while gases are processed top-down. Movement is based on local rules such as falling, diagonal spreading, sideways flow, and density-based displacement. The simulation is optimized with chunking, so only active regions of the grid are updated each frame. On top of movement, a separate interaction system handles material logic such as fire spreading to flammable particles, water extinguishing fire into steam, and smoke or steam expiring over time. This separation between simulation and interaction helps keep the design organized and extensible. 

## Running the Project

This project is built with **CMake** and targets **Windows**. The current `CMakeLists.txt` expects local SDL3 library folders and includes hard-coded paths for:

- SDL3
- SDL3_image
- SDL3_ttf
- SDL3_mixer

Before building, update those paths in `CMakeLists.txt` so they match your machine. The project links against those four SDL libraries and copies the `assets` folder into the build output after compilation.

### Basic setup

1. Install or download:
   - SDL3
   - SDL3_image
   - SDL3_ttf
   - SDL3_mixer

2. Open `CMakeLists.txt` and change the Windows library paths to your local install locations.

3. Configure and build the project with CMake in your IDE or from the command line.

4. Run the generated executable. The engine starts in the main menu, then loads the gameplay scene through the scene manager.

## Controls

- **A / D** – move left and right  
- **Space** – jump  
- **Left Click** – paint/spawn the selected particle  
- **Mouse Wheel** – increase or decrease brush size  
- **1–8** – select particle type  
- **E** – eraser  
- **Esc** – toggle menu  
- **F10** – quit the game 

## Project Structure

- `src/ecs/` – entity, component, world, particle grid, and core ECS systems  
- `src/scene/` – scene setup and gameplay/main menu initialization  
- `src/manager/` – scene, texture, asset, and audio management  
- `src/utils/` – helper utilities such as math, collision, and particle helpers  
- `assets/` – maps, textures, fonts, audio, UI, and animation data :contentReference
