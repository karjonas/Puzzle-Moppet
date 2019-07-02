
#include "ISound3D.h"
#include "OpenALSound.h"

class OpenALSound3D : public OpenALSound, public ISound3D
{
public:
    OpenALSound3D(OpenALSoundSystem *soundSystem);
    ~OpenALSound3D();

    void SetPosition(core::vector3df pos) override;
    void SetVelocity(core::vector3df vel) override;
    void SetMaxVolumePoint(f32 proximity) override;
};
