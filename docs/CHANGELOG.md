# VoxTerGen Changelog

All notable changes to VoxTerGen will be documented in this file.  
The format follows Keep a Changelog and Semantic Versioning.

---

## [Unreleased]

### Added
-

### Changed
-

### Fixed
-

---

## [0.5.2] - 22 July 2026

### Added
- Added support for loading cubemap textures from a texture atlas
- Added skybox rendering

---

## [0.5.1] - 17 July 2026

### Added
- Implemented Digital Differential Analyzer (DDA) raycasting for voxel traversal
- Added block selection highlight rendered with a dedicated render pass

---

## [0.5.0] - 30 June 2026

### Added
- Basic AABB collision detection and collision resolution against voxel terrain
- Observer physics system
- Walking with acceleration, friction and maximum movement speed
- Gravity and jumping
- Airborne and Grounded movement states
- Noclip mode toggle
- Fixed-timestep movement simulation
- Horizontal movement independent of camera pitch
- Velocity-based movement with collision clipping

### Changed
- Refactored observer movement to use acceleration and velocity instead of direct position changes
- Separated input gathering from movement simulation
- Camera now follows observer movement after physics simulation

---

## [0.4.0] - 2 May 2026

### Added
- Basic gradient and cellular noise algorithms to static lib project for procedural generation of terrain and caves.
- Fractal class for combining multiple octaves of noise.
- Vector field based walker for generating procedural caves.

---

## [0.3.0] - 3 April 2026

### Added
- Chunk streaming logic for loading and unloading chunks based on observer's position.
- Chunk event queue to handle communication between engine components.
- Improved rendering pipeline to support chunk streaming.

---

## [0.2.0] - 28 March 2026

### Added
- Basic single-threaded chunk mesh rendering pipeline.
- Chunk build queue to limit mesh generation per frame.
- Separation of CPU-side and GPU-side chunk data for mesh generation and upload.

---

## [0.1.0] - 4 March 2026

### Added
- Chunk and block management system for efficient world data storage and access.
- World initialization system for generating chunks and blocks.
- Greedy and naive mesh generation algorithms for chunk geometry.

---

## [0.0.0] - 17 December 2025

### Added
- Initial project setup with three components:
  - Engine application
  - Static algorithms library
  - CLI benchmarking application
- OpenGL and SDL2 integration with window creation and graphics context initialization.
- Basic fixed-timestep game loop handling input, state updates, and rendering.
- Empty scene with a movable camera.
- Input handling system for camera navigation.
