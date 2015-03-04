
#include "ISoundQueue.h"
#include "IUpdatable.h"

class ISoundSystem;

struct QueuedSoundInfo
{
	ISound *sound;
	s32 flags;
	bool started;
	f32 lastPlayPosition;
};

class SoundQueue : public ISoundQueue, public IUpdatable
{
	ISoundSystem *soundSystem;
	
	std::vector<QueuedSoundInfo> soundInfos;
	std::vector<ISound *> tempSounds;
	
protected:
	void OnPause() override;
	void OnResume() override;
	
public:
	SoundQueue(ISoundSystem *soundSystem);
	~SoundQueue();
	
	void Add(ISound *sound, const c8 *soundFile, s32 flags) override;
	
	void Add2D(const c8 *soundFile, s32 flags) override;
	void Add3D(const c8 *soundFile, s32 flags) override;
	
	void BreakLoop() override;
	
	const std::vector<ISound *> &GetAllSounds() override;
	
	void Update(f32 dt) override;
};


