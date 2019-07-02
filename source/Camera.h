
#include "ICamera.h"

class Camera : public ICamera
{
    scene::ICameraSceneNode *irrCamera;

    f32 targetDistance;

    // Target distance wants interpolating.
    // (for smooth zooming)
    f32 cachedTargetDistance;

public:
    Camera(scene::ICameraSceneNode *irrCamera);

    void SetTarget(const core::vector3df &target) override;
    void SetOrbitTarget(const core::vector3df &target) override;

    void SetTargetDistance(f32 distance) override;
    f32 GetTargetDistance() override;

    f32 GetFOV() override { return irrCamera->getFOV(); }
    f32 GetAspectRatio() override { return irrCamera->getAspectRatio(); }

    void CacheInterpolatableState() override;

    // Camera has custom position interpolation code.
    // (regular method does not ensure camera will remain looking at target)
    core::vector3df GetInterpolatedPosition(f32 alpha) override;

    void ReceiveRenderPosition(core::vector3df pos) override;
    void ReceiveRenderRotation(core::vector3df rot) override;
};
