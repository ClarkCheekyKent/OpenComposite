# Interface coverage for this custom build

Updated 2026-09-29. Comparison uses the included Valve OpenVR 2.15.6 header; this is not a claim of complete SteamVR compatibility.

## Input 011 update

The DLL exposes IVRInput_011 with the correct method ordering. Existing controller and skeletal calls use the current OpenComposite input implementation.

The two eye tracking queries now retrieve live combined gaze through XR_EXT_eye_gaze_interaction:

- Parses Valve's `eyetracking` action manifest type and creates an OpenXR pose action in the application's action set.
- Binds it to `/user/eyes_ext/input/gaze_ext/pose` and recreates its action space with each new session.
- Reports active, valid and tracked states from the runtime. Missing or unavailable gaze returns an inactive result, without invented tracking data.
- Locates gaze in the requested seated or standing space. Raw tracking uses the OpenXR stage as an approximation.
- Uses the runtime's predicted frame time for the next-frame query. Relative-time queries use an OpenXR-converted system clock when available; otherwise they use the latest frame time plus the requested offset.
- Provides gaze action origins and binding information alongside the two tracking calls.

OpenXR's standard gaze extension supplies a ray without fixation depth. `vGazeTarget` is a point one metre along the live gaze direction, rather than a measured fixation point. Hardware and the selected runtime must expose eye tracking; brand-specific SDKs are not used.

Both Release x64 and x86 builds succeeded. A focused native test of the production gaze query passed with simulated OpenXR responses for inactive tracking, position/direction, requested timestamps, tracking loss and runtime errors. No Man's Sky and the Pimax Crystal Super OLED have not been tested with this build. Linux clock conversion is implemented but has not been compiled or tested in this Windows workspace.

References: [Valve's eye action documentation](https://partner.steamgames.com/doc/steamhardware/steamframe/input), [OpenXR eye gaze specification](https://registry.khronos.org/OpenXR/specs/1.0/man/html/XR_EXT_eye_gaze_interaction.html).

## Latest interface versions absent from this build

- IVRApplications_008 (Applications versions through 007 are registered)
- IVRNotifications_002
- IVRTrackedCamera_006
- IVRResources_001
- IVRDriverManager_001
- IVRIOBuffer_002
- IVRSpatialAnchors_001
- IVRDebug_001
- IVRIPCResourceManagerClient_003

Registered interfaces can still contain unsupported methods, including aborting stubs inherited from upstream. Registering an interface does not imply every function or feature is implemented. Older interface revisions were not exhaustively compared in this check.
