OpenComposite custom build - IVRInput_011 with OpenXR gaze support
2026-09-27, Windows x64

The included openvr_api.dll replaces the previous custom DLL.
Close the game, back up its current openvr_api.dll, and replace it with this copy.

Normal controller input uses OpenComposite's existing OpenXR input implementation.
Eye tracking actions (manifest type "eyetracking") now retrieve live data through
XR_EXT_eye_gaze_interaction on any runtime that exposes that extension and supports
eye gaze on the connected headset. A headset/runtime without gaze support returns
inactive data. It does not disable normal controller inputs.

The standard API provides origin and direction, without fixation distance.
vGazeTarget is represented as a point one metre along the live gaze ray.
Raw tracking coordinates approximate the runtime's stage space. Relative timing
uses clock conversion when available, and latest frame time as a fallback.

Release x64 and x86 builds succeeded. Focused simulated OpenXR gaze tests passed.
This build has not been tested with NMS or a Pimax Crystal Super OLED.
Some other OpenVR interfaces/methods remain unsupported; this is not complete
SteamVR compatibility.
