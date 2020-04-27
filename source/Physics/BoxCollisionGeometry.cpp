
#include "BoxCollisionGeometry.h"

BoxCollisionGeometry::BoxCollisionGeometry(dSpaceID space,
                                           const core::vector3df &size)
    : CollisionGeometry(space)
{
    SetODEGeom(dCreateBox(space, size.X, size.Y, size.Z));

    this->size = size;
}

BoxCollisionGeometry::~BoxCollisionGeometry() = default;

core::vector3df BoxCollisionGeometry::GetSize()
{
    return size;
}
