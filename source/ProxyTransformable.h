
#include "IProxyTransformable.h"

class IWorld;

class ProxyTransformable : public IProxyTransformable
{
    IWorld *world;
    ITransformable *other;

    core::vector3df positionOffset;

public:
    ProxyTransformable(IWorld *world, ITransformable *other);
    ~ProxyTransformable();

    ITransformable *GetProxiedTransformable() override;

    void SetPositionOffset(const core::vector3df &offset) override;

    // wrapped methods

    void SetPosition(const core::vector3df &pos) override;
    void SetRotation(const core::vector3df &rot) override;

    core::vector3df GetPosition() override;
    core::vector3df GetRotation() override;
};
