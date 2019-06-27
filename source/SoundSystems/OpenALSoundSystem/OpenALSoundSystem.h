
#include "SoundSystem.h"
#include "openal_stuff.h"
#include <map>

class OpenALSoundSystem : public SoundSystem
{
	ALCdevice *device;
	ALCcontext *context;
	
	std::map<core::stringc, ALuint> buffers;
	
	f32 globalVolume2D;
	f32 globalVolume3D;
	
public:
	OpenALSoundSystem();
	~OpenALSoundSystem();
	
	bool GetOpenALBuffer(const core::stringc &fileName, ALuint *buffer);
	
	void PreloadSound(const c8 *soundFile) override;
	
	void SetListenerPosition(core::vector3df pos) override;
	void SetListenerOrientation(core::vector3df lookVec, core::vector3df upVec) override;
	void SetListenerVelocity(core::vector3df vel) override;
	
	ISound2D *CreateSound2D() override;
	ISound3D *CreateSound3D() override;
	
	void SetGlobalVolume(f32 volume) override;
	void SetGlobalVolume2D(f32 volume) override;
	void SetGlobalVolume3D(f32 volume) override;
	
	void StopAllSounds() override;
};

