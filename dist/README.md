# OpenComposite OpenVR API builds

These Windows release builds come from OpenComposite commit `cff07db75c4823afe93ed7027b03d5f7bc86f164` with the OpenVR 2.15.6 header, `IVRCompositor_029`, `IVRSystem_026`, and `IVROverlay_028` support added in this workspace.

- `x64/openvr_api.dll`: 64-bit games
- `x86/openvr_api.dll`: 32-bit games

Back up the game's original `openvr_api.dll`, then use the DLL matching the game's architecture. Both builds also expose `IVRCompositor_028`.

The new `IVRCompositor_029::GetSubmitTexture` call returns `VRCompositorError_RequestFailed`, since OpenComposite does not provide shared submit textures. Other compositor methods continue to use the existing implementation.

The new `IVRSystem_026` methods for distortion sets and eye tracked foveation report unsupported. Its overlay event method returns ordinary system events with an invalid overlay handle.

The new `IVROverlay_028` subview methods return `VROverlayError_RequestFailed`; subview overlays are not implemented.

Overlay curvature settings can be set and read without aborting, but overlays still render as flat OpenXR quads.
