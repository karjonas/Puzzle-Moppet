
#include "StaticBody.h"
#include "ICollisionGeometry.h"
#include "CollisionGeometry.h"
#include "ode_utility.h"

StaticBody::StaticBody()
{
    // Do nothing yet...
}

StaticBody::~StaticBody()
{
    for (auto &elem : geometry)
        elem->drop();
}

void StaticBody::AddCollisionGeometry(ICollisionGeometry *geom)
{
    geom->grab();
    geometry.push_back(geom);

    if (CollisionGeometry *hasGeom = dynamic_cast<CollisionGeometry *>(geom))
    {
        dGeomID odeGeom = hasGeom->GetODEGeom();
        odeGeoms.push_back(odeGeom);
        dGeomSetBody(odeGeom, nullptr);
    }
    else
        FAIL << "A collision geometry could not be dynamically cast to "
                "CollisionGeometry.";
}

Set<ICollisionGeometry *> StaticBody::GetCollisionGeometries()
{
    Set<ICollisionGeometry *> geometries;

    for (auto &elem : geometry)
        geometries.Insert(elem);

    return geometries;
}

void StaticBody::SetPosition(const core::vector3df &pos)
{
    ITransformable::SetPosition(pos);

    for (auto &elem : odeGeoms)
        ODESetGeomPosition(elem, pos);
}

void StaticBody::SetRotation(const core::vector3df &rot)
{
    ITransformable::SetRotation(rot);

    for (auto &elem : odeGeoms)
        ODESetGeomRotation(elem, rot);
}
