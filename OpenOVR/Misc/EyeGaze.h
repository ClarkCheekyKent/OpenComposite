#pragma once

#include "generated/interfaces/vrtypes.h"
#include <openxr/openxr.h>

namespace xr_utils {

// XR_EXT_eye_gaze_interaction exposes a combined gaze ray, without fixation depth.
// Use a point one metre along that ray so callers can recover its direction.
inline XrResult ReadEyeGaze(XrSession session, XrAction action, XrSpace gazeSpace,
    XrSpace baseSpace, XrTime time, vr::VREyeTrackingData_t& data)
{
	data = {};
	XrActionStateGetInfo getInfo{ XR_TYPE_ACTION_STATE_GET_INFO };
	getInfo.action = action;
	XrActionStatePose state{ XR_TYPE_ACTION_STATE_POSE };
	XrResult result = xrGetActionStatePose(session, &getInfo, &state);
	if (XR_FAILED(result))
		return result;
	data.bActive = state.isActive == XR_TRUE;
	if (!data.bActive)
		return XR_SUCCESS;

	XrSpaceLocation location{ XR_TYPE_SPACE_LOCATION };
	result = xrLocateSpace(gazeSpace, baseSpace, time, &location);
	if (XR_FAILED(result)) {
		data = {};
		return result;
	}
	const XrSpaceLocationFlags valid = XR_SPACE_LOCATION_POSITION_VALID_BIT | XR_SPACE_LOCATION_ORIENTATION_VALID_BIT;
	const XrSpaceLocationFlags tracked = XR_SPACE_LOCATION_POSITION_TRACKED_BIT | XR_SPACE_LOCATION_ORIENTATION_TRACKED_BIT;
	data.bValid = (location.locationFlags & valid) == valid;
	data.bTracked = data.bValid && (location.locationFlags & tracked) == tracked;
	if (!data.bValid)
		return XR_SUCCESS;

	const XrVector3f& p = location.pose.position;
	const XrQuaternionf& q = location.pose.orientation;
	// Rotate OpenXR's forward vector (0, 0, -1) by the gaze orientation.
	const XrVector3f direction{
		-2.0f * (q.x * q.z + q.w * q.y),
		-2.0f * (q.y * q.z - q.w * q.x),
		2.0f * (q.x * q.x + q.y * q.y) - 1.0f,
	};
	data.vGazeOrigin = { p.x, p.y, p.z };
	data.vGazeTarget = { p.x + direction.x, p.y + direction.y, p.z + direction.z };
	return XR_SUCCESS;
}

} // namespace xr_utils
