
#include "IMotionSensor.h"
#include <vector>
#include "InfiniteRunningAverage.h"

// Note: This implementation has not really been tested.
class MotionSensor : public IMotionSensor
{
    IMotionCallback *translateCallback;
    IMotionCallback *rotateCallback;
    IMotionCallback *anyCallback;

    bool relative;
    bool initialEvents;

    f32 minTranslateSpeed;
    f32 minRotateSpeed;

    u32 averagingCount;
    u32 averagingSkip;

    // for counting skips...
    u32 skipCounter;

    // Counts the first two updates.
    // (cannot find a change in motion until after that)
    u8 updateCount;

    // maybe we don't consider all axes
    s32 translateAxesEnabled;

    // Cached values from previous update.

    core::vector3df lastPos;
    core::vector3df lastRot;

    std::vector<f32> posDeltaHistory;
    std::vector<f32> rotDeltaHistory;

    InfiniteRunningAverage posDeltaAverage;
    InfiniteRunningAverage rotDeltaAverage;

    bool wasTranslating;
    bool wasRotating;

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
