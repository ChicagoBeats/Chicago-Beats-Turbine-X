# CHICAGO BEATS TURBINE X — Phase 1 Foundation

Original hybrid engine / cinematic sound-design plugin project.

## Included in this starter
- JUCE 6.1.6 / CMake project
- VST3 and Standalone targets
- Six-page navigation framework
- Initial RPM, throttle, master, drive, and width controls
- Six procedural layer slots; first-pass synthesis/noise prototypes
- Host parameter state save/restore
- Plugin-owned bottom-right resize grip and proportional editor resizing
- GitHub Actions Windows build workflow

## Important Phase 1 limitations
This is a development foundation, not a finished product. The six sound layers are deliberately simple procedural prototypes. The Sample layer is currently a placeholder oscillator; sample import/playback, proper engine models, advanced modulation, Doppler/flight-path processing, full FX, preset browser, favorites, randomization, and Sound DNA are not implemented yet.

The GUI uses a functional resizable JUCE editor with a bottom-right resize grip. Validate scaling and host behavior in the target DAW. FL Studio 11 compatibility is not guaranteed until tested in that exact host environment.

## Build locally
Requirements:
- CMake 3.22+
- A C++17 compiler
- Git
- JUCE is fetched by CMake from the pinned 6.1.6 tag

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Build outputs are under the build tree in the JUCE-generated plugin target folders.

## GitHub
Create a separate repository for TURBINE X, upload this project, and let the Actions workflow compile the Windows VST3 and Standalone targets. Review the build log before treating any artifact as validated.

## Suggested next milestones
1. Confirm clean Windows build.
2. Replace the prototype oscillators with dedicated layer DSP classes.
3. Add sample loading on the message thread and preloaded sample buffers for real-time playback.
4. Implement Engine Morph, RPM curves, Doppler and flight paths.
5. Add processing chain, modulation, randomization and preset/Sound DNA management.
6. Refine the UI to match the supplied concept image, including the turbine visualization and skin system.
