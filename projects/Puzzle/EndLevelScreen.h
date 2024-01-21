
#include "Litha.h"
#include "level_stats.h"

class MainState;
class Level;
class GUIPane;

class EndLevelScreen : public IUpdatable, public IWantEvents
{
    IEngine *engine;
    IRenderSystem *renderSystem;

    MainState *mainState;
    Level *level;

    LevelStats stats;

    E_SCORE_RESULT scoreResult;

    GUIPane *guiBackground = nullptr;
    gui::IGUIStaticText *guiTextLevelComplete = nullptr;
    gui::IGUIStaticText *guiTextBlocksPushed = nullptr;
    gui::IGUIStaticText *guiTextElevatorJourneys = nullptr;
    gui::IGUIStaticText *guiTextUndosUsed = nullptr;
    gui::IGUIStaticText *guiTextDeaths = nullptr;
    gui::IGUIStaticText *guiTextYourRating = nullptr;
    gui::IGUIStaticText *guiTextRating = nullptr;

    IEventQueue *eventQueue;

    ISound *sound;

    void RepositionGuiElements();

public:
    EndLevelScreen(MainState *mainState, Level *level);
    ~EndLevelScreen();

    void OnEvent(const Event &event) override;
};
