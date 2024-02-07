
#include "IThirdPersonCameraController.h"
#include "IWantInput.h"

class IWorld;
class ICollisionGeometry;

class ThirdPersonCameraController : public IThirdPersonCameraController,
                                    public IWantInput
{
    IWorld *world = nullptr;
    ITransformable *following = nullptr;

    f32 heightOffset = 0.f;
    f32 minPitch = 0.f;
    f32 maxPitch = 0.f;
    f32 turnSpeed = 0.f;
    f32 turnSmooth = 0.f;

    f32 minZoom = 0.0f;
    f32 maxZoom = 0.0f;
    f32 zoomSpeed = 1.0f;
    f32 zoomSmooth = 0.0f;

    // Distance of camera from target
    f32 nextDistance = 0.f;
    f32 currentDistance = 0.f;

    // Angles from target to camera
    core::vector2df nextAngle;
    core::vector2df currentAngle;

    s32 idAxisYaw = -1;
    s32 idAxisPitch = -1;
    s32 idAxisZoom = -1;

    // Camera collider?
    ICameraCollider *collider = nullptr;

public:
    ThirdPersonCameraController(IWorld *world, scene::ISceneManager *smgr,
                                f32 distance, f32 heightOffset, f32 minPitch,
                                f32 maxPitch, f32 turnSpeed);
    ~ThirdPersonCameraController();

    void OnAxisChange(s32 id, f32 delta) override;

    void Follow(ITransformable *toFollow) override;
    void Follow(ICharacter *toFollow, ICameraCollider *collider) override;

    void SetInputAxes(s32 idAxisYaw, s32 idAxisPitch, s32 idAxisZoom) override;

    void SetTurnSpeed(f32 speed) override;
    void SetTurnSmoothing(f32 smooth) override;

    void SetPitchLimits(f32 min, f32 max) override;

    void SetZoomLimits(f32 min, f32 max) override;
    void SetZoomSpeed(f32 speed) override;
    void SetZoomSmoothness(f32 smooth) override;

    void SetAngles(const core::vector2df &angles) override;
    core::vector2df GetAngles() override;

    void SetZoom(f32 zoom) override;
    f32 GetZoom() override;

    void SetCollider(ICameraCollider *collider) override;
    IThirdPersonCameraCollider *CreateThirdPersonCameraCollider() override;

    void Update(ICamera *camera, f32 dt) override;
};
