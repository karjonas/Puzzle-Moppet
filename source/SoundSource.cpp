
#include "SoundSource.h"
#include "ISoundSystem.h"
#include "ISound3D.h"
#include "ISoundQueue.h"

SoundSource::SoundSource(ISoundSystem *soundSystem)
{
    this->soundSystem = soundSystem;

    sound = soundSystem->CreateSound3D();
    soundQueue = soundSystem->CreateSoundQueue();
}

SoundSource::~SoundSource()
{
    sound->drop();
    soundQueue->drop();
}

ISound3D *SoundSource::GetSound()
{
    return sound;
}

ISoundQueue *SoundSource::GetSoundQueue()
{
    return soundQueue;
}

void SoundSource::OnPause()
{
    sound->Pause();
    soundQueue->Pause();
}

void SoundSource::OnResume()
{
    sound->Resume();
    soundQueue->Resume();
}

void SoundSource::Update(f32 dt)
{
    if (IsPaused())
        return;

    // Create a list of all sounds, from "sound" and "soundQueue".

    std::vector<ISound *> tempAllSounds;

    tempAllSounds.push_back(sound);

    const std::vector<ISound *> &soundQueueSounds = soundQueue->GetAllSounds();

    for (auto &soundQueueSound : soundQueueSounds)
        tempAllSounds.push_back(soundQueueSound);

    // Set all 3D sounds with a position and velocity

    core::vector3df pos = GetAbsolutePosition();
    core::vector3df vel = GetAbsoluteLinearVelocity();

    for (auto &tempAllSound : tempAllSounds)
    {
        // Is it a 3D sound?
        if (auto *sound3d = dynamic_cast<ISound3D *>(tempAllSound))
        {
            sound3d->SetPosition(pos);
            sound3d->SetVelocity(vel);
        }
    }
}
