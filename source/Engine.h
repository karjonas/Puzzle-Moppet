
#include "IEngine.h"
#include <stack>
#include "Buttons.h"
#include <map>

class Kernel;
class LogicTask;
class RenderTask;
class World;

class Engine : public IEngine, public IEventReceiver
{
    IrrlichtDevice *device;
    Kernel *kernel;
    World *world;
    ISoundSystem *soundSystem;
    io::IFileSystem *filesys;
    ITimer *timer;
    gui::ICursorControl *curcon;

    core::dimension2du cachedDesktopResolution;
    io::path startDir;

    // command line args used to launch the app.
    // (may be empty, exe name is not included)
    // useful if we want to restart the app.
    std::vector<io::path> args;

    // Engine init settings.
    VariantMap initSettings;

    u32 engineStartTime;
    u32 pauseTime;

    // push/pop working directory history.
    std::stack<io::path> pushedWDs;

    LogicTask *logicTask;
    RenderTask *renderTask;

    ButtonStates buttonStates;

    // for calculating mouse delta
    core::position2df lastMousePos;
    bool autoCentreMouseX;
    bool autoCentreMouseY;

    // event system
    std::map<core::stringc, Set<IWantEvents *>> specificEventInterest;
    Set<IWantEvents *> allEventInterest;

    // event queue
    std::vector<Event> eventQueue;

    // Should the engine re-launch the application on exiting?
    bool restartOnExit;

    void ClearButtonStates();

protected:
    void OnPause() override;
    void OnResume() override;

public:
    VariantMap GetDefaultSettings() override;
    VariantMap GetCreationSettings() override;

    Engine(int argc, const char **argv, const VariantMap *settings);
    ~Engine();

    IrrlichtDevice *GetIrrlichtDevice() override;

    IWorld *GetWorld() override;
    IRenderSystem *GetRenderSystem() override;
    ISoundSystem *GetSoundSystem() override;

    IUpdater &GetLogicUpdater() override;
    IUpdater &GetRenderUpdater() override;

    IUpdater *CreateUpdater() override;
    IEventQueue *CreateEventQueue() override;

    void InitUpdateTiming() override;

    bool GetButtonState(s32 button) override;
    ButtonStates GetButtonStates() override;
    void SetButtonState(s32 button, bool state) override;

    f32 GetEngineTime() override;

    f32 GetLogicInterpolationAlpha() override;

    void Run() override;
    void Exit() override;
    void Restart() override;
    bool WillRestart() override;

    void SetWindowTitle(const core::stringw &title) override
    {
        device->setWindowCaption(title.c_str());
    }

    void PushWorkingDirectory(const io::path &path) override;
    void PopWorkingDirectory() override;
    io::path GetStartingDirectory() override;
    io::path GetLocalSettingsDir() override;
    io::path GetSettingsPath() override;

    void RegisterEventInterest(IWantEvents *receiver,
                               const core::stringc &eventName) override;
    void UnregisterEventInterest(IWantEvents *receiver,
                                 const core::stringc &eventName) override;
    void RegisterAllEventInterest(IWantEvents *receiver) override;
    void UnregisterAllEventInterest(IWantEvents *receiver) override;
    void PostEvent(const Event &event) override;
    void QueueEvent(const Event &event, f32 delay) override;
    void ProcessEventQueue() override;

    void SetAutoMouseCentring(bool centreX, bool centreY) override;
    void CentreMouse() override;
    core::dimension2du GetDesktopResolution() override;

    bool OnEvent(const SEvent &irrEvent) override;
};
