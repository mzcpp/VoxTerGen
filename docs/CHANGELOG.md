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

## Changelog

## [0.6.2] - 9 October 2026

### Added

- Introduced a unified `Noise` base class and `NoiseType` enum for selecting noise algorithms.
- Added `FractionalBrownianMotion` with configurable octaves, lacunarity, persistence, and signal transformations, including absolute, squared, inverted absolute, and ridged shaping.
- Added terrain and cave generator classes to establish a foundation for future procedural generation features and algorithm expansion.
- Added heightmap-based terrain generation using fBm, configurable noise selection, terrain shaping curves, and bedrock placement.
- Added `ThreadSafeDeque` with thread-safe front/back insertion and removal.
- Added terrain-generation states and chunk readiness checks to coordinate terrain and mesh generation.
- Added world seed and thread-pool job limit constants.

### Changed

- Refactored Perlin, Simplex, Worley, OpenSimplex2F, and OpenSimplex2S noise implementations to use the common `Noise` interface, making it easier to extend the engine with additional algorithms in the future.
- Updated `VectorFieldWalker` to use a generic scalar field, configurable step size and frequency, and optional results for invalid or zero-length directions.
- Reworked chunk generation to schedule terrain generation and mesh building separately, prioritizing nearby chunks.
- Updated chunk lifecycle handling with terrain-generation tracking, stop-token synchronization, and atomic readiness transitions.
- Added terrain dependency checks before scheduling chunk meshes and neighboring chunk mesh rebuilds.
- Improved thread-pool exception handling with logging and explicit worker shutdown.
- Increased chunk height from 128 to 384 blocks and the default render radius from 2 to 32 chunks.
- Increased the observer's movement speed from 25 to 250.
- Added default initialization for several mesh, raycast, camera, rendering, event, and block data structures.
- Removed the previous chunk initialization workflow and retained a temporary flat-chunk filler for testing.

### Removed

- Replaced the generic `Fractal` implementation with `FractionalBrownianMotion`.
- Removed the explicit world and chunk-manager initialization methods previously used to prepopulate chunks.

### Future Expansion

- Established a modular foundation for adding and comparing further noise algorithms, terrain-generation techniques, and cave-generation approaches.
- Prepared the cave-generation interface for future implementation of tunnel carving and vector-field-based cave systems.
- Added extensible terrain-shaping and coordinate-warping utilities to support more varied procedural landscapes.

---

## [0.6.1] - 2 September 2026

### Added
- Added rendering of transparent blocks by splitting `ChunkMeshRenderPass` into `ChunkOpaqueRenderPass` and `ChunkTransparentRenderPass`.
- Added `ChunkWireframeRenderPass` for rendering the wireframe of the currently occupied chunk.
- Added a screen-centered crosshair using `UIRenderPass`.
- Added a fog effect to hide chunk streaming artifacts.
- Added `EventDispatcher` for basic event distribution.

### Changed
- Split rendering classes into separate 2D and 3D rendering classes.
- Renamed and refactored numerous functions to improve consistency and code organization.

### Fixed
- Fixed chunk meshes not being rebuilt correctly when neighboring chunks are loaded or unloaded.

---

## [0.6.0] - 16 August 2026

### Added
- Added multithreaded chunk streaming and mesh generation.
- Added custom `ThreadPool`, `ThreadSafeQueue`, and `ThreadSafePriorityQueue` classes.
- Added a custom `Timer` class for profiling.
- Added priority-based chunk streaming based on distance from the observer.

### Changed
- Redesigned chunk meshing to use snapshots of neighboring chunks instead of querying the world during mesh generation.
- Added chunk mesh generation cancellation using `std::stop_token`.
- Improved chunk lifetime management during asynchronous mesh generation using `std::shared_ptr`.

---

## [0.5.3] - 24 July 2026

### Added
- Implemented frustum culling to avoid rendering chunks outside the camera's view.

---

## [0.5.2] - 22 July 2026

### Added
- Added support for loading cubemap textures from a texture atlas.
- Added skybox rendering.

---

## [0.5.1] - 17 July 2026

### Added
- Implemented Digital Differential Analyzer (DDA) raycasting for voxel traversal.
- Added block selection highlight rendered with a dedicated render pass.

---

## [0.5.0] - 30 June 2026

### Added
- Basic AABB collision detection and collision resolution against voxel terrain.
- Observer physics system.
- Walking with acceleration, friction and maximum movement speed.
- Gravity and jumping.
- Airborne and Grounded movement states.
- Noclip mode toggle.
- Fixed-timestep movement simulation.
- Horizontal movement independent of camera pitch.
- Velocity-based movement with collision clipping.

### Changed
- Refactored observer movement to use acceleration and velocity instead of direct position changes.
- Separated input gathering from movement simulation.
- Camera now follows observer movement after physics simulation.

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
