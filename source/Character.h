
#include "ICharacter.h"

class IWorld;
class IPhysics;
class IMesh;
class ICollisionGeometry;

class Character : public ICharacter
{
    IWorld *world;
    IMesh *mesh;
    IDynamicBody *body;

    // pointer kept as body mass/density may need changing
    ICollisionGeometry *geom;

    // Mesh is offset from the body so that it rests exactly on the ground
    f32 meshHeightOffset;

    core::vector2df moveVec;
    core::vector2df relativeMotion; // motion relative to moveVec angle

    f32 moveSpeed;

    core::vector3df attemptedMove;

    f32 turnSmooth;

    // Mesh animations to be used.
    s32 idAnimIdle;
    s32 idAnimWalk;

    // Turned a signed byte into -1.0, 0.0, or +1.0
    f32 ClampDir(c8 dir)
    {
        if (dir == 0)
            return 0.0;
        else if (dir > 0)
            return 1.0;
        else
            return -1.0;
    }

public:
    Character(IWorld *world, IPhysics *physics, IMesh *mesh, s32 idAnimIdle,
              s32 idAnimWalk, f32 height);
    ~Character();

    void SetPosition(const core::vector3df &pos) override;
    void SetRotation(const core::vector3df &rot) override;

    core::vector3df GetPosition() override;
    core::vector3df GetRotation() override;

    void SetAnimations(s32 idAnimIdle, s32 idAnimWalk) override;
    bool IsSetAnimations(s32 idAnimIdle, s32 idAnimWalk) override;

    void SetGravityEnabled(bool enabled) override;
    void SetDensity(f32 density) override;

    void SetMaterial(ICollisionMaterial *material) override;

    void SetMoveSpeed(f32 speed) override;

    void SetMoveVec(const core::vector2df &vec) override;
    core::vector2df GetMoveVec() override;

    void SetMedialMotion(c8 dir) override;
    void SetLateralMotion(c8 dir) override;

    void SetTurnSmoothing(f32 smooth) override { turnSmooth = smooth; }

    core::vector3df GetAttemptedMove() override;

    void StopMoving() override;

    IMesh *GetMesh() override { return mesh; }
    IDynamicBody *GetBody() override { return body; }

    void Update(f32 dt) override;
};
