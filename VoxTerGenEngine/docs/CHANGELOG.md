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