#include "EndLevelScreen.h"
#include "MainState.h"
#include "Level.h"
#include "GUIPane.h"
#include "utils/paths.h"

const f32 timeBeforeShowing = 2.f;

EndLevelScreen::EndLevelScreen(MainState *mainState, Level *level)
    : mainState(mainState)
    , level(level)
{
    engine = GetEngine();
    renderSystem = engine->GetRenderSystem();

    stats = level->GetLevelStats();
    scoreResult = input_score(level->GetShortName(), stats);

    engine->RegisterEventInterest(this, "ButtonDown");
    engine->RegisterEventInterest(this, "EndLevelText");
    engine->RegisterEventInterest(this, "ScreenFadeFinished");
    engine->RegisterEventInterest(this, "EndLevelScreenShow");
    engine->RegisterEventInterest(this, "EndLevelScreenClose");
    engine->RegisterEventInterest(this, "EndLevelScreenListItem");
    engine->RegisterEventInterest(this, "EndLevelScreenListItemFinalScore");

    engine->RegisterEventInterest(this, "ScreenResize");

    sound = engine->GetSoundSystem()->CreateSound2D();
    sound->SetVolume(0.5);

    eventQueue = engine->CreateEventQueue();
    GetUpdater().AddUpdatable(eventQueue);
    eventQueue->drop();

    // Wait a brief period before showing.
    Event event("EndLevelScreenShow");
    eventQueue->AddTimeWait(timeBeforeShowing);
    eventQueue->AddEvent(event);
}

EndLevelScreen::~EndLevelScreen()
{
    if (sound)
        sound->drop();

    engine->UnregisterAllEventInterest(this);

    guiBackground->remove();
    guiTextLevelComplete->remove();
    guiTextBlocksPushed->remove();
    if (guiTextElevatorJourneys)
        guiTextElevatorJourneys->remove();
    if (guiTextUndosUsed)
        guiTextUndosUsed->remove();
    guiTextDeaths->remove();
    guiTextYourRating->remove();
    guiTextRating->remove();
}

void EndLevelScreen::RepositionGuiElements()
{
    if (!guiBackground)
        return;

    ASSERT(guiTextLevelComplete);
    ASSERT(guiTextBlocksPushed);
    // ASSERT(guiTextElevatorJourneys); // Optional
    // ASSERT(guiTextUndosUsed); // Optional
    ASSERT(guiTextDeaths);
    ASSERT(guiTextYourRating);
    ASSERT(guiTextRating);

    video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
    s32 screenWidth = driver->getScreenSize().Width;
    s32 screenHeight = driver->getScreenSize().Height;

    // Layout background
    s32 bgHeight = 0;
    {
        // pad above + below
        bgHeight += 2 * guiTextLevelComplete->getTextHeight();
        bgHeight += 1.5 * guiTextBlocksPushed->getTextHeight();
        if (guiTextElevatorJourneys)
            bgHeight += 1.5 * guiTextElevatorJourneys->getTextHeight();
        if (guiTextUndosUsed)
            bgHeight += 1.5 * guiTextUndosUsed->getTextHeight();
        bgHeight += 1.5 * guiTextDeaths->getTextHeight();
        bgHeight += 1.5 * std::max(guiTextYourRating->getTextHeight(),
                                   guiTextRating->getTextHeight());

        auto rect = core::rect<s32>(0, 0, screenWidth, bgHeight);
        rect += core::vector2di(0, screenHeight / 2 - bgHeight / 2); // center y
        guiBackground->setRelativePosition(rect);
    }

    // Layout all text except rating
    s32 yPos = s32(screenHeight / 2 - bgHeight / 2);

    for (gui::IGUIStaticText *element :
         {guiTextLevelComplete, guiTextBlocksPushed, guiTextElevatorJourneys,
          guiTextUndosUsed, guiTextDeaths})
    {
        if (element == nullptr)
            continue; // elevator optional

        const int width = element->getTextWidth();
        const int height = element->getTextHeight();

        // Add padding before first item
        if (element == guiTextLevelComplete)
            yPos += height * 0.5;

        auto rect = core::rect<s32>(0, 0, width, height);
        rect += core::vector2di(screenWidth / 2 - width / 2, yPos);
        element->setRelativePosition(rect);
        yPos += height * 1.5;
    }

    // Layout rating
    {
        // "Your rating"
        core::recti rect = core::recti(0, 0, guiTextYourRating->getTextWidth(),
                                       guiTextYourRating->getTextHeight());
        rect += core::vector2di(screenWidth / 2 - rect.getWidth() / 2, yPos);

        // "Good", "Perfect" etc.
        core::recti rect2 = core::recti(0, 0, guiTextRating->getTextWidth(),
                                        guiTextRating->getTextHeight());
        rect2 += core::vector2di(screenWidth / 2 - rect2.getWidth() / 2, yPos);

        // Move side by side
        rect -= core::vector2di(rect2.getWidth() / 2, 0);
        rect2 += core::vector2di(rect.getWidth() / 2, 0);

        guiTextYourRating->setRelativePosition(rect);
        guiTextRating->setRelativePosition(rect2);
    }
}

void EndLevelScreen::OnEvent(const Event &event)
{
    video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
    s32 screenWidth = driver->getScreenSize().Width;
    s32 screenHeight = driver->getScreenSize().Height;

    constexpr f32 betweenItemWait = 1.0f;
    constexpr f32 itemFadeOnTime = 0.5f;

    auto addFade = [&](gui::IGUIStaticText *element)
    {
        element->setVisible(true);
        auto *fade =
            new GUIElementFade(engine->GetIrrlichtDevice()->getGUIEnvironment(),
                               element, this, itemFadeOnTime, itemFadeOnTime,
                               false);
        fade->drop();
        fade->OnPostRender(0);
    };

    if (event.IsType("ScreenResize"))
    {
        RepositionGuiElements();
    }
    else if (event.IsType("EndLevelScreenShow"))
    {
        {
            ASSERT(guiBackground == nullptr);

            guiBackground =
                new GUIPane(engine->GetIrrlichtDevice()
                                ->getGUIEnvironment()
                                ->getRootGUIElement(),
                            core::recti(-10, 0, screenWidth + 10, screenHeight),
                            video::SColor(200, 0, 0, 0));
            guiBackground->drop();
        }

        {
            ASSERT(guiTextLevelComplete == nullptr);

            core::stringw text = L"Level Complete!";
            guiTextLevelComplete = add_static_text2(text.c_str());
        }

        {
            ASSERT(guiTextBlocksPushed == nullptr);

            eventQueue->AddTimeWait(betweenItemWait);
            Event event("EndLevelScreenListItem");
            event["text"] << "Blocks pushed: " << stats.pushes;
            eventQueue->AddEvent(event);
            gui::IGUIStaticText *element = add_static_text(
                core::stringw(event["text"].To<core::stringc>()).c_str());
            guiTextBlocksPushed = element;
            guiTextBlocksPushed->setVisible(false);
        }

        // presumably if there are elevators present, we will use them at least
        // once. so this if should fail only if there are no elevators.
        if (stats.elevatorMoves > 0)
        {
            ASSERT(guiTextElevatorJourneys == nullptr);

            eventQueue->AddTimeWait(betweenItemWait);
            Event event("EndLevelScreenListItem");
            event["text"] << "Elevator journeys: " << stats.elevatorMoves;
            eventQueue->AddEvent(event);
            gui::IGUIStaticText *element = add_static_text(
                core::stringw(event["text"].To<core::stringc>()).c_str());
            guiTextElevatorJourneys = element;
            guiTextElevatorJourneys->setVisible(false);
        }

        // ugly hack.
        // we don't want to show undo stats on first level.
        // (since tutorial doesn't introduce undo until second level)
        if (level->GetShortName() != "first.lev")
        {
            ASSERT(guiTextUndosUsed == nullptr);

            eventQueue->AddTimeWait(betweenItemWait);
            Event event("EndLevelScreenListItem");
            event["text"] << "Undos used: " << stats.undos;
            eventQueue->AddEvent(event);
            gui::IGUIStaticText *element = add_static_text(
                core::stringw(event["text"].To<core::stringc>()).c_str());
            guiTextUndosUsed = element;
            guiTextUndosUsed->setVisible(false);
        }

        {
            ASSERT(guiTextDeaths == nullptr);

            eventQueue->AddTimeWait(betweenItemWait);
            Event event("EndLevelScreenListItem");
            event["text"] << "Deaths: " << stats.deaths;
            eventQueue->AddEvent(event);
            gui::IGUIStaticText *element = add_static_text(
                core::stringw(event["text"].To<core::stringc>()).c_str());
            guiTextDeaths = element;
            guiTextDeaths->setVisible(false);
        }

        {
            ASSERT(guiTextYourRating == nullptr);
            ASSERT(guiTextRating == nullptr);

            eventQueue->AddTimeWait(betweenItemWait);
            Event event("EndLevelScreenListItemFinalScore");
            event["text"] << "Your rating: ";
            event["text2"] << get_result_description(scoreResult);
            eventQueue->AddEvent(event);
            gui::IGUIStaticText *element = add_static_text2(
                core::stringw(event["text"].To<core::stringc>()).c_str());
            guiTextYourRating = element;
            guiTextYourRating->setVisible(false);

            gui::IGUIStaticText *element2 = add_static_text2(
                core::stringw(event["text2"].To<core::stringc>()).c_str());
            element2->setOverrideColor(get_rating_col(scoreResult)
                                       // and make a bit brighter since we
                                       // are on a black background
                                       + video::SColor(50, 0, 0, 0));
            guiTextRating = element2;
            guiTextRating->setVisible(false);
        }

        RepositionGuiElements();

        // Stop player teleport effects.
        // Not sure whether to do this or not.
        // level->ClearEndLevelTeleportEffects();
    }
    else if (event.IsType("EndLevelScreenListItem"))
    {
        core::stringw text = event["text"].To<core::stringc>();
        auto textStartsWith = [&text](const char *other) -> bool
        { return text.subString(0, strlen(other)) == other; };

        if (textStartsWith("Blocks pushed:"))
        {
            addFade(guiTextBlocksPushed);
        }
        else if (textStartsWith("Elevator journeys:"))
        {
            addFade(guiTextElevatorJourneys);
        }
        else if (textStartsWith("Undos used:"))
        {
            addFade(guiTextUndosUsed);
        }
        else if (textStartsWith("Deaths:"))
        {
            addFade(guiTextDeaths);
        }

        sound->Play(paths::get_sfx("appear.ogg"));
    }
    else if (event.IsType("EndLevelScreenListItemFinalScore"))
    {
        addFade(guiTextYourRating);
        addFade(guiTextRating);

        switch (scoreResult)
        {
        case ESR_AWFUL:
            sound->Play(paths::get_sfx("laugh.ogg"));
            break;
        case ESR_FAIR:
            sound->SetVolume(0.15);
            sound->Play(paths::get_sfx("fair.ogg"));
            break;
        case ESR_GOOD:
            sound->SetVolume(0.15);
            sound->Play(paths::get_sfx("good.ogg"));
            break;
        case ESR_EXCELLENT:
            sound->SetVolume(0.15);
            sound->Play(paths::get_sfx("excellent.ogg"));
            break;
        case ESR_PERFECT:
            sound->SetVolume(0.25);
            sound->Play(paths::get_sfx("perfect.ogg"));
            break;
        case ESR_EXTRAORDINARY:
            sound->SetVolume(0.25);
            sound->Play(paths::get_sfx("extraordinary.ogg"));
            break;
        default:
            WARN << "Unknown score result.";
            break;
        }
    }
    else if (event.IsType("ButtonDown") &&
             eventQueue->IsEmpty()) // && event["button"] == KEY_LBUTTON)
    {
        // Fade off
        renderSystem->ScreenFade(0.f, 2, true);

        // And close this.
        Event event("EndLevelScreenClose");
        TimedEvent(event, 2.f);
    }
    else if (event.IsType("ButtonDown") && !eventQueue->IsEmpty() &&
             eventQueue->IsEventWaiting("EndLevelScreenListItemFinalScore"))
    {
        // Speed up displaying...
        eventQueue->ScaleTimes(0.1);
    }
    else if (event.IsType("EndLevelScreenClose"))
    {
        // Finished.
        // So call next level and remove this screen.
        mainState->NextLevel(true);
        engine->GetWorld()->GetUpdater().RemoveUpdatable(this);
    }
    else if (event.IsType("EndLevelText"))
    {
    }
}
