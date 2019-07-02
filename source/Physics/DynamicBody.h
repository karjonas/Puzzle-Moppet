
#ifndef DYNAMIC_BODY_H
#define DYNAMIC_BODY_H

#include "IDynamicBody.h"
#include <ode/ode.h>
#include <vector>

class DynamicBody : public IDynamicBody
{
    std::vector<ICollisionGeometry *> geometry;

    dBodyID body;

    void FillODEMassFromGeometry(dMass *dmass, f32 density,
                                 ICollisionGeometry *geom);

public:
    DynamicBody(dWorldID world);
    ~DynamicBody();

    void AddCollisionGeometry(ICollisionGeometry *geom) override;
    Set<ICollisionGeometry *> GetCollisionGeometries() override;

    void SetPosition(const core::vector3df &pos) override;
    void SetRotation(const core::vector3df &rot) override;

    core::vector3df GetPosition() override;
    core::vector3df GetRotation() override;

    void SetMass(f32 density, ICollisionGeometry *geom) override;
    void AddMass(f32 density, ICollisionGeometry *geom) override;

    void SetGravityEnabled(bool enabled) override;

    void SetLinearDamping(f32 amount) override;
    void SetAngularDamping(f32 amount) override;

    void ApplyForce(const core::vector3df &force) override;
    void ApplyTorque(const core::vector3df &torque) override;

    void SetLinearVelocity(const core::vector3df &vel) override;
    void SetAngularVelocity(const core::vector3df &vel);

    core::vector3df GetLinearVelocity() override;

    void ZeroLinearVelocity(s32 axes) override;
    void ZeroAngularVelocity() override;

    void AddThruster(IThruster *thruster) override;
};

#endif
