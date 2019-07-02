
#ifndef SOUND_SOURCE_H
#define SOUND_SOURCE_H

#include "ISoundSource.h"

class ISoundSystem;

class SoundSource : public ISoundSource
{
    ISoundSystem *soundSystem;
    ISound3D *sound;
    ISoundQueue *soundQueue;

protected:
    void OnPause() override;
    void OnResume() override;

public:
    SoundSource(ISoundSystem *soundSystem);
    ~SoundSource();

    ISound3D *GetSound() override;
    ISoundQueue *GetSoundQueue() override;

    void Update(f32 dt) override;
};

#endif
