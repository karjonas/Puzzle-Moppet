
#include "IMotionSensor.h"
#include <vector>
#include "InfiniteRunningAverage.h"

// Note: This implementation has not really been tested.
class MotionSensor : public IMotionSensor
{
    IMotionCallback *translateCallback = nullptr;
    IMotionCallback *rotateCallback = nullptr;
    IMotionCallback *anyCallback = nullptr;

    bool relative = false;
    bool initialEvents = true;

    f32 minTranslateSpeed = 0.0001f;
    f32 minRotateSpeed = 0.001f;

    u32 averagingCount = 0;
    u32 averagingSkip = 0;

    // for counting skips...
    u32 skipCounter = 0;

    // Counts the first two updates.
    // (cannot find a change in motion until after that)
    u8 updateCount = 0;

    // maybe we don't consider all axes
    s32 translateAxesEnabled = AXIS_X | AXIS_Y | AXIS_Z;

    // Cached values from previous update.

    irr::core::vector3df lastPos;
    irr::core::vector3df lastRot;

    std::vector<f32> posDeltaHistory;
    std::vector<f32> rotDeltaHistory;

    InfiniteRunningAverage posDeltaAverage;
    InfiniteRunningAverage rotDeltaAverage;

    bool wasTranslating = false;
    bool wasRotating = false;

    void ResetAveraging();

public:
    MotionSensor();
    ~MotionSensor();

    void SetTranslateMotionCallback(IMotionCallback *callback) override;
    void SetRotateMotionCallback(IMotionCallback *callback) override;
    void SetAnyMotionCallback(IMotionCallback *callback) override;

    void SetRelative(bool relative) override;
    void EnableInitialEvents(bool initialEvents) override;

    void SetMinTranslateSpeed(f32 speed) override;
    void SetMinRotateSpeed(f32 speed) override;

    void SetAveragingCount(u32 count) override;
    void SetAveragingSkip(u32 skip) override;

    void EnableTranslateAxes(s32 flags) override;

    void Update(f32 dt) override;
};
