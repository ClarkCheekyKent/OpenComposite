// Focused test of the production gaze query against deterministic OpenXR responses.
#include "../OpenOVR/Misc/EyeGaze.h"
#include <cmath>
#include <cstdio>

static XrResult stateResult = XR_SUCCESS;
static XrResult locateResult = XR_SUCCESS;
static XrActionStatePose state{ XR_TYPE_ACTION_STATE_POSE };
static XrSpaceLocation location{ XR_TYPE_SPACE_LOCATION };
static int locateCalls = 0;
static XrTime observedTime = 0;
static const XrSession session = reinterpret_cast<XrSession>(1);
static const XrAction action = reinterpret_cast<XrAction>(2);
static const XrSpace gazeSpace = reinterpret_cast<XrSpace>(3);
static const XrSpace baseSpace = reinterpret_cast<XrSpace>(4);

XRAPI_ATTR XrResult XRAPI_CALL xrGetActionStatePose(XrSession s, const XrActionStateGetInfo* info, XrActionStatePose* out)
{
	if (s != session || info->action != action || info->subactionPath != XR_NULL_PATH)
		return XR_ERROR_HANDLE_INVALID;
	*out = state;
	return stateResult;
}
XRAPI_ATTR XrResult XRAPI_CALL xrLocateSpace(XrSpace space, XrSpace base, XrTime time, XrSpaceLocation* out)
{
	if (space != gazeSpace || base != baseSpace)
		return XR_ERROR_HANDLE_INVALID;
	++locateCalls;
	observedTime = time;
	*out = location;
	return locateResult;
}

#define CHECK(expr) do { if (!(expr)) { std::printf("FAIL line %d: %s\n", __LINE__, #expr); return 1; } } while (0)
static bool near(float a, float b) { return std::fabs(a - b) < 0.00001f; }

int main()
{
	vr::VREyeTrackingData_t data{};
	auto read = [&]() { return xr_utils::ReadEyeGaze(session, action, gazeSpace, baseSpace, 1005000000, data); };
	data.bActive = data.bValid = data.bTracked = true;
	CHECK(read() == XR_SUCCESS);
	CHECK(!data.bActive && !data.bValid && !data.bTracked && locateCalls == 0);

	state.isActive = XR_TRUE;
	location.locationFlags = XR_SPACE_LOCATION_POSITION_VALID_BIT | XR_SPACE_LOCATION_ORIENTATION_VALID_BIT
	    | XR_SPACE_LOCATION_POSITION_TRACKED_BIT | XR_SPACE_LOCATION_ORIENTATION_TRACKED_BIT;
	location.pose.position = { 1.0f, 2.0f, 3.0f };
	location.pose.orientation.w = 1.0f;
	CHECK(read() == XR_SUCCESS);
	CHECK(data.bActive && data.bValid && data.bTracked);
	CHECK(observedTime == 1005000000);
	CHECK(near(data.vGazeOrigin.v[0], 1.0f) && near(data.vGazeOrigin.v[1], 2.0f) && near(data.vGazeOrigin.v[2], 3.0f));
	CHECK(near(data.vGazeTarget.v[0], 1.0f) && near(data.vGazeTarget.v[1], 2.0f) && near(data.vGazeTarget.v[2], 2.0f));

	const float rootHalf = std::sqrt(0.5f);
	location.pose.orientation = { 0.0f, rootHalf, 0.0f, rootHalf };
	CHECK(read() == XR_SUCCESS);
	CHECK(near(data.vGazeTarget.v[0], 0.0f) && near(data.vGazeTarget.v[2], 3.0f));

	location.locationFlags = XR_SPACE_LOCATION_POSITION_VALID_BIT | XR_SPACE_LOCATION_ORIENTATION_VALID_BIT;
	CHECK(read() == XR_SUCCESS && data.bValid && !data.bTracked);
	location.locationFlags = XR_SPACE_LOCATION_ORIENTATION_VALID_BIT;
	CHECK(read() == XR_SUCCESS && data.bActive && !data.bValid && !data.bTracked);
	CHECK(data.vGazeOrigin.v[0] == 0.0f && data.vGazeTarget.v[0] == 0.0f);

	stateResult = XR_ERROR_HANDLE_INVALID;
	const int previousCalls = locateCalls;
	CHECK(read() == XR_ERROR_HANDLE_INVALID && !data.bActive && locateCalls == previousCalls);
	stateResult = XR_SUCCESS;
	locateResult = XR_ERROR_TIME_INVALID;
	CHECK(read() == XR_ERROR_TIME_INVALID && !data.bActive && !data.bValid && !data.bTracked);
	std::puts("PASS: inactive gaze, live position/direction, timing, tracking loss and runtime errors");
	return 0;
}
