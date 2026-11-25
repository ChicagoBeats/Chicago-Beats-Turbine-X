# Claude implementation brief — CHICAGO BEATS TURBINE X

You are continuing an original JUCE/C++ audio plugin project. Read the entire repository before editing. Preserve the product name and black / red / chrome identity.

## Product
CHICAGO BEATS TURBINE X is an original real-time hybrid synthesis + sample-layer sound-design instrument/effect for engine, turbine, combustion, pressure, propulsion, mechanical, and cinematic sound design. It is inspired by the broad category of dynamic engine sound-design tools, but must have original DSP, UI, workflows, and branding. Do not copy another product's exact interface, presets, code, or branding.

## Hard requirements
- Deliver VST3 and Standalone targets.
- Use JUCE and CMake.
- Preserve host automation and reliable state serialization.
- Never allocate memory, load files, lock mutexes, or perform blocking operations in the audio callback.
- File selection and decoding must occur off the audio thread; transfer prepared buffers safely to the DSP.
- Build a plugin-owned bottom-right resize grip. Resizing must scale/reflow the complete UI (controls, text, panels, graphics), not merely resize the outer window.
- Keep all six pages: ENGINE, LAYER MATRIX, MOTION DESIGNER, FX PROCESSING, RANDOM LAB, PRESET VAULT.
- Keep all six layers: Turbine, Combustion, Pressure, Propulsion, Mechanical, Sample.
- Use clear, professional DSP architecture with unit-testable components where practical.
- Do not claim FL Studio 11 compatibility until tested in FL Studio 11 specifically.

## Build sequence
1. Inspect and compile the existing Phase 1 project. Fix compile errors before adding features. Report exact compiler/toolchain versions and build result.
2. Replace prototype layer code with separate DSP classes and meaningful sound generation.
3. Implement sample import, waveform display, sample start/end, pitch, loop, reverse, and safe buffer handoff.
4. Implement RPM/throttle dynamics, engine morphing, envelopes, modulation, and layer routing.
5. Implement Doppler and flight-path movement with automation.
6. Implement FX chain: filtering, saturation, compression, stereo width, delay, reverb, and modulation.
7. Implement Random Lab with parameter locks and reproducible seeds.
8. Implement Preset Vault, favorites, user preset files, and Sound DNA state snapshots.
9. Refine the interface to the supplied concept image while keeping controls legible and functional.
10. Add tests and CI checks; document remaining limitations.

## Required working style
- Make small, buildable commits.
- After each milestone, compile and run available tests.
- Never replace working code with pseudocode or TODO-only stubs.
- If a feature cannot be completed, state exactly what remains.
- Keep the UI responsive and the audio thread real-time safe.
- Provide a concise change log and exact build instructions at each handoff.
