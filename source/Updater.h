
#ifndef UPDATER_H
#define UPDATER_H

#include "IUpdater.h"
#include "IUpdatable.h"
#include <vector>

struct UpdatableInfo
{
    IUpdatable *ptr;
    bool keptRef;
};

class Updater : public IUpdater
{
    std::vector<UpdatableInfo> updatables;
    f32 virtualTime;
    f32 lastDeltaTime;
    bool updatablesArePaused;

public:
    Updater()
    {
        updatablesArePaused = false;
        virtualTime = 0.f;
        lastDeltaTime = 0.f;
    }

    ~Updater() { RemoveAllUpdatables(); }

    void InitAllUpdateTimes() override
    {
        for (auto &elem : updatables)
            elem.ptr->InitUpdateTime();
    }

    void UpdateAllUpdatables(f32 virtualTime, f32 dt) override
    {
        // Cache the parent's virtual time
        this->virtualTime = virtualTime;

        // Cache delta time
        lastDeltaTime = dt;

        // NOTE: cannot use range-based loop here since elements
        // can be added or removed during update
        for (u32 i = 0; i < updatables.size(); i++)
            updatables[i].ptr->Update(dt);
    }

    f32 GetVirtualTime() const override { return virtualTime; }

    f32 GetLastDeltaTime() const override { return lastDeltaTime; }

    void AddUpdatable(IUpdatable *updatable, bool keepRef = true) override
    {
        if (!updatable)
            return;

        // check not already present
        for (auto &elem : updatables)
        {
            if (elem.ptr == updatable)
                return;
        }

        if (keepRef)
            updatable->grab();

        UpdatableInfo info = {updatable, keepRef};
        updatables.push_back(info);

        // maybe pause
        if (updatablesArePaused)
            updatable->Pause();
    }

    void RemoveUpdatable(IUpdatable *updatable) override
    {
        for (u32 i = 0; i < updatables.size(); i++)
        {
            if (updatables[i].ptr == updatable)
            {
                bool keptRef = updatables[i].keptRef;

                updatables.erase(updatables.begin() + i);

                // did we grab it?
                if (keptRef)
                    updatable->drop();

                break;
            }
        }
    }

    void RemoveAllUpdatables() override
    {
        while (updatables.size())
            RemoveUpdatable(updatables.front().ptr);
    }

    void RemoveAllUpdatablesRecursive() override
    {
        for (size_t i = 0; i < updatables.size(); i++)
            updatables[i].ptr->GetUpdater().RemoveAllUpdatablesRecursive();

        RemoveAllUpdatables();
    }

    void PauseAllUpdatables() override
    {
        updatablesArePaused = true;

        for (auto &elem : updatables)
            elem.ptr->Pause();
    }

    void ResumeAllUpdatables() override
    {
        updatablesArePaused = false;

        for (auto &elem : updatables)
            elem.ptr->Resume();
    }
};

#endif
