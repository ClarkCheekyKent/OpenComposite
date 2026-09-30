# OpenComposite community fork (OpenXR)

A community fork of [OpenComposite](https://gitlab.com/znixian/OpenOVR/-/tree/openxr), translating OpenVR calls to OpenXR so compatible games can run through an OpenXR runtime.

This fork adds newer OpenVR interface compatibility and standard OpenXR eye-gaze input. It is an independent, experimental fork. Credit for the original project belongs to Campbell Suter (ZNix) and the upstream contributors; their commit history and license notices are preserved.

## Changes in this fork

- OpenVR 2.15.6 headers and registrations for `IVRCompositor_029`, `IVRSystem_026`, `IVROverlay_028`, and `IVRInput_011`.
- Eye-tracking actions through `XR_EXT_eye_gaze_interaction`, alongside existing controller and skeletal input.
- Inactive eye data when the headset or runtime does not expose gaze support.
- Overlay curvature values can be set and read; rendering remains flat OpenXR quads.

See [interface support and limitations](dist/interface-support.md) for details. Interface registration does not imply every method is implemented; some inherited unsupported calls can still abort.

## Download and install (Windows)

Download an experimental build from this repository's [Releases](https://github.com/ClarkCheekyKent/OpenComposite/releases).

1. Close the game and back up its original `openvr_api.dll`.
2. Extract the release for the game's architecture: x64 for 64-bit games, x86 for 32-bit games.
3. Replace the game's DLL with the extracted `openvr_api.dll`.
4. Select your headset's OpenXR runtime and start the game. Restore the original DLL to undo the change.

Upstream's launcher and download services distribute upstream builds. Use this fork's release packages to obtain these changes.

## Validation and known limitations

Windows Release x64 and x86 builds succeeded during development. The focused gaze test uses simulated OpenXR responses to exercise inactive tracking, position and direction, requested timestamps, tracking loss, and runtime errors.

No Man's Sky and the Pimax Crystal Super OLED have not been tested with this build. Linux clock conversion is implemented but has not been compiled or tested in this Windows workspace.

Standard OpenXR gaze supplies origin and direction without fixation depth. `vGazeTarget` is one metre along the gaze ray. Raw tracking uses stage space as an approximation; relative timing uses clock conversion where available and otherwise the latest frame time.

## Build from source

Clone this repository with `git clone --recurse-submodules https://github.com/ClarkCheekyKent/OpenComposite.git`. Existing clones should run `git submodule update --init --recursive`.

Windows prerequisites: Visual Studio C++ tools with a Windows SDK, CMake, Python 3, and the Vulkan headers/import libraries under `libs/vulkan` as described in the [upstream build documentation](doc/upstream-readme.md#windows-specific). The Vulkan SDK is not included in this repository.

From a Visual Studio developer shell, using the Visual Studio generator installed on your system:

```powershell
cmake -S . -B build/windows-x64 -A x64
cmake --build build/windows-x64 --config Release --target OCOVR
cmake -S . -B build/windows-x86 -A Win32
cmake --build build/windows-x86 --config Release --target OCOVR
```

The DLLs are `build/windows-x64/bin/Release/vrclient_x64.dll` and `build/windows-x86/bin/Release/vrclient.dll`. Rename the matching DLL to `openvr_api.dll` for per-game installation. For Linux instructions and general configuration, see the [preserved upstream README](doc/upstream-readme.md).

### Focused gaze test

The standalone test needs CMake, a C++20 compiler, Python 3, and the OpenXR SDK submodule. It does not require a headset or Vulkan SDK.

```powershell
cmake -S tests -B build/gaze-tests
cmake --build build/gaze-tests --config Release
ctest --test-dir build/gaze-tests -C Release --output-on-failure
```

## Reporting issues and contributing

Open an issue in this repository with the game, headset, OpenXR runtime and version, build used, and reproduction steps. Include relevant log excerpts after removing personal information. Contributions and hardware test reports are welcome; ongoing maintenance is on a best-effort basis.

## License and credits

The project retains the upstream [GNU GPLv3 license](LICENSE.txt) and third-party notices. Release binaries are accompanied by their corresponding tagged source; build instructions and dependency submodules are included in that source. See the individual license files for bundled components.
