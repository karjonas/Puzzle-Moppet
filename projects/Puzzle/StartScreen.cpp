
#include "StartScreen.h"
#include "MainState.h"
#include "Level.h"
#include "options.h"
#include "utils/paths.h"
#include "Colors.h"

#include <string>

extern ISound *bgAmbientSound;
extern core::stringc globalLogFurthestLevel;

#define START_MENU_ID 9999
#define START_NEW_GAME_MENU_ID 8297
#define LEVEL_SELECT_MENU_ID 13865
#define OPTIONS_MENU_ID 31337

enum E_MENU_ITEM
{
    EMI_PLAY,
    EMI_EXIT,
    EMI_NEW_GAME,
    EMI_OPTIONS,

    // New Game confirmation prompt
    EMI_NEW_GAME_YES,
    EMI_NEW_GAME_NO,

    // Level selectors
    EMI_LEVEL_PREV,
    EMI_LEVEL_NEXT,

    // Options menu
    EMI_OPTIONS_FULLSCREEN,
    EMI_OPTIONS_SCREEN_RES,
    EMI_OPTIONS_SHADERS,
    EMI_OPTIONS_SKY_EFFECTS,
    EMI_OPTIONS_BLOOM,
    EMI_OPTIONS_WIND_VOLUME,
    EMI_OPTIONS_MUSIC_VOLUME,
    EMI_OPTIONS_SFX_VOLUME,
    EMI_OPTIONS_MOVEMENT,
    EMI_OPTIONS_MOUSE_INVERT,
    EMI_OPTIONS_CANCEL,
    EMI_OPTIONS_OK
};

StartScreen::StartScreen(MainState **mainStatePtrLoc)
{
    this->mainStatePtrLoc = mainStatePtrLoc;

    engine = GetEngine();
    renderSystem = engine->GetRenderSystem();
    device = engine->GetIrrlichtDevice();
    world = engine->GetWorld();

    mainState = nullptr;

    engine->RegisterEventInterest(this, "ButtonDown");
    engine->RegisterEventInterest(this, "ScreenFadeFinished");
    engine->RegisterEventInterest(this, "MenuButton");

    fadingIntoGame = false;

    device->getCursorControl()->setVisible(true);

    // Fade on
    renderSystem->ScreenFade(0.0, 0.0, true);
    renderSystem->ScreenFade(1.0, 2, true);

    // TEST. Level previewing!

    levelPreview = nullptr;
    camControl = nullptr;
    levelTitleText = nullptr;
    levelRatingText = nullptr;
    levelFractionText = nullptr;

    core::stringc continueLevel = find_next_level();

    levelFileNames = find_levels();

    // let's remove the special levels from that list
    for (u32 i = 0; i < levelFileNames.size(); i++)
    {
        if (levelFileNames[i] == "intro.lev" ||
            levelFileNames[i] == "finalscene1.lev" ||
            levelFileNames[i] == "finalscene2.lev"
            // also not the first level
            || levelFileNames[i] == "first.lev")
        {
            levelFileNames.erase(levelFileNames.begin() + i);
            i--;
        }
    }

    levelTitles = file::loadsettings(paths::get_levels_name_file());

    // ensure a furthest level file exists (e.g. from old version that didn't
    // have one)
    if (file::get(get_full_save_path_furthest()).size() == 0)
    {
        NOTE << "furthest file did not exist...";
        NOTE << "Will attempt to create.";

        if (!file::put(get_full_save_path_furthest(),
                       file::get(get_full_save_path())))
            WARN << "(failed)";
    }

    // furthestLevelReached = "pyramid_mining.lev"; // for testing... need to
    // load from file. furthestLevelReached =
    // file::get(get_full_save_path_furthest());
    furthestLevelReached = find_next_level(true);

    globalLogFurthestLevel = furthestLevelReached;

    CreatePreviewLevel(continueLevel);

    // Some title text! Woo!
    titleMesh = world->AddMesh("titletext.b3d");
    world->GetCamera()->AddChild(titleMesh);
    titleMesh->SetPosition(core::vector3df(-0.01, 0.39, 1));

    /*
    IMesh *titleBorder = world->AddMesh("titletext_border.b3d");
    titleBorder->GetMaterial(0).Lighting = false;
    titleMesh->AddChild(titleBorder);
    */

    // shaders
    levelPreview->ApplyWoodShaders(titleMesh);

    // eh?
    // engine->InitUpdateTiming();

    // Start menu
    // want level to have been created first...
    CreateFirstMenu();

    // bgAmbientSound->SetVolume(0.05);
    // bgAmbientSound->Play(paths::get_media_dir()+"/music.ogg");
}

void StartScreen::CreatePreviewLevel(core::stringc previewLevelName)
{
    // no game saves yet, user is running game for first time?
    // (or they deleted saves)
    // We display the preview differently in that case.
    firstRun = false;

    if (!previewLevelName.size())
    {
        firstRun = true;
        previewLevelName = "titlescreen.lev";
    }

    levelPreview = new Level(nullptr, level_path_rel_exe(previewLevelName));
    levelPreview->SetLogicEnabled(false);
    world->GetUpdater().AddUpdatable(levelPreview);
    levelPreview->drop();

    if (!firstRun)
    {
        // let's hide the player
        // don't use SetVisible as level update resets that...
        levelPreview->GetPlayer()->GetMesh()->DisableAllMaterials(0);
    }

    // camera

    camTarget = world->AddTransform();
    camControl = world->CreateThirdPersonCameraController(0.f);
    camControl->Follow(camTarget);
    world->SetCameraController(camControl);

    UpdateLevelCamera();

    // World not paused, so need this!
    engine->SetAutoMouseCentring(false, false);
}

void StartScreen::UpdateLevelCamera()
{
    camTarget->SetPosition(levelPreview->GetCentrePoint());

    f32 dist = world->GetCamera()->GetTargetDistance();

    if (firstRun)
    {
        dist = 4.f;
    }

    camControl->SetZoomLimits(dist, dist); // hack, just sets distance
}

void StartScreen::CreateFirstMenu()
{
    menu = new SimpleVerticalMenu(START_MENU_ID);

    if (os::path::exists(get_full_save_path()))
    {
        NOTE << "Game save exists, will show continue game option.";
        menu->AddItem("Continue", EMI_PLAY);
        menu->AddItem("New Game", EMI_NEW_GAME);
    }
    else
    {
        NOTE << "No game save exists, showing only Start Game option.";
        menu->AddItem("Start Game", EMI_PLAY);
    }

    menu->AddItem("Options", EMI_OPTIONS);
    menu->AddItem("Exit", EMI_EXIT);
    menu->SetMouseOverSound(paths::get_sfx("beep.ogg"));
    menu->Finalise();

    // level selector
    CreateLevelSelectButtons();
}

void StartScreen::CreateLevelSelectButtons()
{
    {
        video::IVideoDriver *driver = device->getVideoDriver();
        u32 screenWidth = driver->getScreenSize().Width;
        u32 screenHeight = driver->getScreenSize().Height;

        ASSERT(levelPreview);

        core::stringc levelTitle;

        if (levelTitles.count(levelPreview->GetShortName()))
            levelTitle = levelTitles[levelPreview->GetShortName()];
        else
            levelTitle = "";

        menuLevelSelect = new SimpleEitherSideToggleMenu(LEVEL_SELECT_MENU_ID);
        menuLevelSelect->AddImageItem("arrow_left.png", EMI_LEVEL_PREV);
        menuLevelSelect->AddImageItem("arrow_right.png", EMI_LEVEL_NEXT);

        // show level select only if more than one level
        // and NOT first run either...
        if (levelFileNames.size() > 1 && !firstRun)
        {
            // Can click to a previous level if the current level is not the
            // first.
            bool canGoPrev = levelPreview->GetShortName() != levelFileNames[0];

            // Can click to a next level if the current level is not the last.
            // AND if the current level is not the furthest reached.
            bool canGoNext =
                levelPreview->GetShortName() !=
                    levelFileNames[levelFileNames.size() - 1] &&
                levelPreview->GetShortName() != furthestLevelReached
                // and a furthest level has actually been set
                // (if not, it's probably first run with no levels to choose
                // from!)
                && furthestLevelReached.size();

            // So display the prev/next options depending on those states.
            menuLevelSelect->Enable(canGoPrev, canGoNext);
        }
        else
        {
            menuLevelSelect->Enable(false, false);
        }

        // menuLevelSelect->SetHeading(levelTitle);
        menuLevelSelect->SetMouseOverSound(paths::get_sfx("beep.ogg"));
        menuLevelSelect->Finalise();

        // level name

        if (levelTitleText)
        {
            levelTitleText->remove();
            levelTitleText = nullptr;
        }

        core::stringw text;

        /*
        // find fraction of levels this one is
        {
            u32 i = 0;
            for (; i < levelFileNames.size(); i ++)
            {
                if (levelFileNames[i] == levelPreview->GetShortName())
                    break;
            }

            // not found?
            if (i >= levelFileNames.size())
            {
            }
            else // found!
            {
                text += i+1;
                text += L"/";
                text += levelFileNames.size();
                text += " ";
            }
        }
        */

        text += levelTitle;
        levelTitleText = add_static_text2(text.c_str());

        f32 distFromTop = 0.24;

        levelTitleText->setRelativePosition(core::vector2di(
            screenWidth / 2 -
                levelTitleText->getRelativePosition().getWidth() / 2,
            s32(screenHeight * distFromTop)));

        levelTitleText->setOverrideColor(Colors::text_level_title());

        levelTitleText->setAlignment(irr::gui::EGUIA_CENTER,
                                     irr::gui::EGUIA_CENTER,
                                     irr::gui::EGUIA_CENTER,
                                     irr::gui::EGUIA_CENTER);

        // level rating text?

        if (levelRatingText)
        {
            levelRatingText->remove();
            levelRatingText = nullptr;
        }

        if (levelPreview->GetShortName() != "titlescreen.lev")
        {
            E_SCORE_RESULT previousRating =
                get_saved_score(levelPreview->GetShortName());

            core::stringw ratingDescription;

            if (previousRating != ESR_UNKNOWN)
                ratingDescription = get_result_description(previousRating);
            else
                ratingDescription = L"uncompleted";

            if (ratingDescription.size() > 0)
            {
                core::stringw ratingText = L"(";
                ratingText += ratingDescription;
                ratingText += L")";

                levelRatingText = add_static_text(ratingText.c_str());

                const s32 yOffset = 10;
                core::recti rect = levelRatingText->getRelativePosition();
                rect += core::vector2di(
                    screenWidth / 2 - rect.getWidth() / 2,
                    levelTitleText->getRelativePosition().LowerRightCorner.Y +
                        yOffset);
                levelRatingText->setRelativePosition(rect);

                if (previousRating != ESR_UNKNOWN)
                    levelRatingText->setOverrideColor(
                        get_rating_col(previousRating));
                else
                    levelRatingText->setOverrideColor(
                        Colors::text_level_default_rating());
            }
        }

        // hmm, let's position rating after
        if (levelRatingText)
        {
            const s32 gap = 10;

            // move title left by half rating size
            levelTitleText->setRelativePosition(
                levelTitleText->getRelativePosition() -
                core::vector2di(
                    levelRatingText->getRelativePosition().getWidth() / 2 +
                        gap / 2,
                    0));

            // and re-position rating text to just after title text
            levelRatingText->setRelativePosition(
                levelTitleText->getRelativePosition().UpperLeftCorner +
                core::vector2di(
                    levelTitleText->getRelativePosition().getWidth() + gap / 2,
                    levelTitleText->getRelativePosition().getHeight() / 2 -
                        levelRatingText->getRelativePosition().getHeight() /
                            2));
            // Set the alignment relative to the window
            levelRatingText->setAlignment(irr::gui::EGUIA_CENTER,
                                          irr::gui::EGUIA_CENTER,
                                          irr::gui::EGUIA_CENTER,
                                          irr::gui::EGUIA_CENTER);
        }

        // Level fraction completed...

        if (levelFractionText)
        {
            levelFractionText->remove();
            levelFractionText = nullptr;
        }

        {
            u32 i = 0;
            for (; i < levelFileNames.size(); i++)
            {
                if (levelFileNames[i] == levelPreview->GetShortName())
                    break;
            }

            // not found?
            if (i >= levelFileNames.size())
            {
            }
            else // found!
            {
                const std::wstring str = L"Puzzle " + std::to_wstring(i + 1) +
                                         L" of " +
                                         std::to_wstring(levelFileNames.size());
                levelFractionText = add_static_text(str.c_str());

                s32 yGap = -2;

                levelFractionText->setRelativePosition(core::vector2di(
                    screenWidth / 2 -
                        levelFractionText->getRelativePosition().getWidth() / 2,
                    levelTitleText->getRelativePosition().LowerRightCorner.Y +
                        yGap));

                levelFractionText->setOverrideColor(
                    Colors::text_level_fraction());
                levelFractionText->setAlignment(irr::gui::EGUIA_CENTER,
                                                irr::gui::EGUIA_CENTER,
                                                irr::gui::EGUIA_CENTER,
                                                irr::gui::EGUIA_CENTER);
            }
        }
    }
}

StartScreen::~StartScreen()
{
    if (menu)
        delete menu;

    if (menuLevelSelect)
        delete menuLevelSelect;

    if (levelTitleText)
        levelTitleText->remove();

    if (levelRatingText)
        levelRatingText->remove();

    if (levelFractionText)
        levelFractionText->remove();

    if (camControl)
        camControl->drop();

    engine->UnregisterAllEventInterest(this);
}

void StartScreen::AndSoItBegins()
{
    // bgAmbientSound->SetVolume(0.035);
    // bgAmbientSound->Play(paths::get_sfx("windy.ogg"));

    device->getCursorControl()->setVisible(false);

    if (menu)
    {
        delete menu;
        menu = nullptr;
    }

    if (menuLevelSelect)
    {
        delete menuLevelSelect;
        menuLevelSelect = nullptr;
    }

    world->SetCameraController(nullptr);
    world->RemoveTransformable(camTarget);

    // Don't want start screen to do anything else now.
    engine->UnregisterAllEventInterest(this);

    // World needs this.
    engine->SetAutoMouseCentring(true, true);

    core::stringc playLevelName;

    // Remove level preview?
    if (levelPreview)
    {
        playLevelName = levelPreview->GetShortName();
        engine->GetWorld()->GetUpdater().RemoveUpdatable(levelPreview);
        levelPreview = nullptr;
    }

    // Remove title text?
    world->RemoveTransformable(titleMesh);

    // create main state
    mainState = new MainState(mainStatePtrLoc);

    // Give the pointer to main.cpp
    *mainStatePtrLoc = mainState;

    // start first level or load and continue...
    if (playLevelName.size() && playLevelName != "titlescreen.lev" &&
        os::path::exists(get_full_save_path()))
        mainState->StartLevel(playLevelName);
    else
        mainState->StartFirstLevel();

    // And remove self!
    // (don't want Update called any more since it calls SetAutoMouseCentring)
    engine->GetLogicUpdater().RemoveUpdatable(this);
}

void StartScreen::Update(f32 dt)
{
    IUpdatable::Update(dt);

    if (levelPreview)
    {
        if (camControl)
        {
            if (!firstRun)
            {
                camControl->SetAngles(
                    core::vector2df(20 + sin(GetVirtualTime() * 0.1f) * 20.f,
                                    GetVirtualTime() * 5.f));
            }
            else // different rotation on first run, with character standing
                 // there.
            {
                camControl->SetAngles(
                    core::vector2df(0 + sin(GetVirtualTime() * 0.1f) * 20.f,
                                    -90 +
                                        sin(GetVirtualTime() * 0.05f) * 30.f));
            }
        }
    }

    // do this all the time, since any world resuming (from alt+tab) will have
    // wiped this state.
    engine->SetAutoMouseCentring(false, false);
}

void StartScreen::ShowOptionsMenu(VariantMap settings)
{
    f32 marginBottom = 0.2;

    for (auto &elem : optionsMenus)
    {
        delete elem;
        elem = nullptr;
    }

    auto *vertMenu = new SimpleVerticalMenu(OPTIONS_MENU_ID, marginBottom);

#ifndef __APPLE__
    // temp disabled on Mac since problems with mouse control in windowed mode.
    // (we default fullscreen)
    vertMenu->AddItem(core::stringw("Full screen: ") +
                          (settings["fullScreen"] ? "Yes" : "No"),
                      EMI_OPTIONS_FULLSCREEN);
#endif

    core::dimension2du screenSize(settings["screenWidth"],
                                  settings["screenHeight"]);
    core::dimension2du desktopSize = engine->GetDesktopResolution();
    core::stringw sizeString = L"Other"; // current size matches neither

    if (screenSize == core::dimension2du(1024, 768))
        sizeString = L"Default";
    else if (screenSize == desktopSize)
    {
        sizeString = L"Desktop";
    }

    // but don't show if desktop res is the same as default res...
    if (desktopSize != core::dimension2du(1024, 768))
        vertMenu->AddItem(core::stringw("Screen size: ") + sizeString,
                          EMI_OPTIONS_SCREEN_RES);

    vertMenu->AddItem(core::stringw("Nice graphics: ") +
                          (settings["shadersEnabled"] ? "Yes" : "No"),
                      EMI_OPTIONS_SHADERS);
    vertMenu->AddItem(core::stringw("Sky effects: ") +
                          (settings["skyEffects"] ? "Yes" : "No"),
                      EMI_OPTIONS_SKY_EFFECTS);
    vertMenu->AddItem(core::stringw("Glow effect: ") +
                          (settings["postProcessingEnabled"] ? "Yes" : "No"),
                      EMI_OPTIONS_BLOOM);

    vertMenu->AddItem(core::stringw("Wind volume: ") +
                          (settings["windVolume"] == 0
                               ? "Off"
                               : settings["windVolume"].To<core::stringc>()),
                      EMI_OPTIONS_WIND_VOLUME);

    vertMenu->AddItem(core::stringw("SFX volume: ") +
                          (settings["sfxVolume"] == 0
                               ? "Off"
                               : settings["sfxVolume"].To<core::stringc>()),
                      EMI_OPTIONS_SFX_VOLUME);

    vertMenu->AddItem(core::stringw("Moppet movement: ") +
                          (settings["gridBasedMovement"] ? "Grid based"
                                                         : "Unrestricted"),
                      EMI_OPTIONS_MOVEMENT);

    core::stringw mouseInvertStr;

    if (settings["invertMouseX"] && settings["invertMouseY"])
        mouseInvertStr = "Invert both X and Y";
    else if (settings["invertMouseX"])
        mouseInvertStr = "Invert X Only";
    else if (settings["invertMouseY"])
        mouseInvertStr = "Invert Y Only";
    else
        mouseInvertStr = "No";

    vertMenu->AddItem(core::stringw("Invert mouse: ") + mouseInvertStr,
                      EMI_OPTIONS_MOUSE_INVERT);

    vertMenu->SetMouseOverSound(paths::get_sfx("beep.ogg"));
    vertMenu->Finalise();
    optionsMenus[0] = vertMenu;

    {
        gui::IGUIEnvironment *guienv =
            GetEngine()->GetIrrlichtDevice()->getGUIEnvironment();
        gui::IGUIFont *font = guienv->getFont("font2.xml");

        const std::vector<gui::IGUIElement *> &elements =
            vertMenu->GetElements();

        for (auto &element : elements)
        {
            // find colon in element text
            // find distance the text up to that colon is
            // then horizontally offset somehow
            // (all colons should align)

            const wchar_t *s = element->getText();
            core::stringw upToColon;

            for (u32 j = 0; s[j] && s[j] != ':'; j++)
                upToColon += s[j];

            u32 upToColonDist = font->getDimension(upToColon.c_str()).Width;

            // offset left by that amount
            element->setRelativePosition(
                element->getRelativePosition() +
                core::vector2di(element->getRelativePosition().getWidth() / 2,
                                0) -
                core::vector2di(upToColonDist, 0) + core::vector2di(20, 0));
        }
    }

    video::IVideoDriver *driver = device->getVideoDriver();
    u32 screenHeight = driver->getScreenSize().Height;

    auto *horizMenu = new SimpleHorizontalMenu(
        OPTIONS_MENU_ID, s32(screenHeight - screenHeight * marginBottom) + 20,
        25, false);
    horizMenu->AddItem("Cancel", EMI_OPTIONS_CANCEL);
    horizMenu->AddItem("Apply", EMI_OPTIONS_OK);
    horizMenu->SetMouseOverSound(paths::get_sfx("beep.ogg"));
    horizMenu->Finalise();
    optionsMenus[1] = horizMenu;

    /*
    SimpleHorizontalMenu *title = new SimpleHorizontalMenu(OPTIONS_MENU_ID);
    title->SetHeading(L"Options");
    title->Finalise();
    optionsMenus.push_back(title);
    */
}

void StartScreen::deleteMenues()
{
    ASSERT(menu);
    delete menu;
    menu = nullptr;

    ASSERT(menuLevelSelect);
    delete menuLevelSelect;
    menuLevelSelect = nullptr;

    if (levelTitleText)
    {
        levelTitleText->remove();
        levelTitleText = nullptr;
    }

    if (levelRatingText)
    {
        levelRatingText->remove();
        levelRatingText = nullptr;
    }

    if (levelFractionText)
    {
        levelFractionText->remove();
        levelFractionText = nullptr;
    }
}

void StartScreen::OnEvent(const Event &event)
{
    if (event.IsType("ButtonDown") && event["button"] == KEY_ESCAPE)
    {
        NOTE << "Exiting from start screen... (esc pressed)";
        engine->Exit();
    }

    if (event.IsType("ScreenFadeFinished") && fadingIntoGame)
    {
        NOTE << "Fade finished, starting game!";

        // AndSoItBegins();

        // This is changed to a delay, with a new event to call the
        // AndSoItBegins method. this is needed as that method deletes
        // previewLevel *while* previewLevel is still present in the event list.
        // (see Engine::PostEvent)
        engine->RegisterEventInterest(this, "AndSoItBegins");
        Event newEvent("AndSoItBegins");
        engine->QueueEvent(newEvent);
    }

    if (event.IsType("AndSoItBegins"))
    {
        AndSoItBegins();
    }

    if (event.IsType("MenuButton"))
    {
        // Handling menu buttons!

        if (event["menu"] == START_MENU_ID)
        {
            if (event["button"] == EMI_PLAY)
            {
                NOTE << "Beginning play from start screen...";
                renderSystem->ScreenFadeFromCurrent(0.0, 2, true);
                fadingIntoGame = true;
            }
            else if (event["button"] == EMI_EXIT)
            {
                NOTE << "Exiting from start screen... (exit clicked)";
                engine->Exit();
            }
            else if (event["button"] == EMI_OPTIONS)
            {
                NOTE << "Showing options menu...";
                deleteMenues();

                originalConfig = get_current_config();
                newConfig = originalConfig; // (will be modified by user
                                            // changing options menu items)

                ShowOptionsMenu(originalConfig);
            }
            else if (event["button"] == EMI_NEW_GAME)
            {
                NOTE << "Showing new game confirmation prompt...";
                deleteMenues();

                auto *horizMenu =
                    new SimpleHorizontalMenu(START_NEW_GAME_MENU_ID);
                // Text and options copied from similar menu in MainState
                horizMenu->SetHeading(
                    L"This will completely clear your game progress. Are you "
                    L"sure?");
                menu = horizMenu;
                menu->AddItem("Cancel", EMI_NEW_GAME_NO);
                menu->AddItem("Start New Game", EMI_NEW_GAME_YES);
                menu->SetMouseOverSound(paths::get_sfx("beep.ogg"));
                menu->Finalise();
            }
        }
        else if (event["menu"] == START_NEW_GAME_MENU_ID)
        {
            if (event["button"] == EMI_NEW_GAME_NO)
            {
                ASSERT(menu);
                delete menu;
                menu = nullptr;

                // Go back to first menu.
                CreateFirstMenu();
            }
            else if (event["button"] == EMI_NEW_GAME_YES)
            {
                // Remove the save file
                // (as in MainState)
                delete_game_saves();

                // Then play the game
                Event newEvent("MenuButton");
                newEvent["menu"] = START_MENU_ID;
                newEvent["button"] = EMI_PLAY;
                OnEvent(newEvent);
            }
        }
        else if (event["menu"] == OPTIONS_MENU_ID)
        {
            if (event["button"] == EMI_OPTIONS_CANCEL ||
                event["button"] == EMI_OPTIONS_OK)
            {
                for (auto &elem : optionsMenus)
                {
                    delete elem;
                    elem = nullptr;
                }

                // Go back to first menu.
                CreateFirstMenu();

                if (event["button"] == EMI_OPTIONS_CANCEL)
                {
                    // we didn't restart or save.
                    // so simply applying things that don't require a restart
                    // from original config will reset.
                    apply_temp_config(originalConfig);
                }
                else if (event["button"] == EMI_OPTIONS_OK)
                {
                    // Apply new settings.
                    // This may cause the application to restart.
                    apply_permanent_config(newConfig);
                }
            }
            else if (event["button"] == EMI_OPTIONS_FULLSCREEN)
            {
                newConfig["fullScreen"] =
                    newConfig["fullScreen"].To<bool>() ^ 1;
                ShowOptionsMenu(newConfig);
            }
            else if (event["button"] == EMI_OPTIONS_SCREEN_RES)
            {
                core::dimension2du currentSize(newConfig["screenWidth"],
                                               newConfig["screenHeight"]);
                core::dimension2du desktopSize = engine->GetDesktopResolution();

                if (currentSize ==
                    core::dimension2du(1024, 768)) // if currently on default
                {
                    NOTE << "Options: changing from default to desktop "
                            "resolution ("
                         << desktopSize.Width << "x" << desktopSize.Height
                         << ")";
                    // change to desktop res
                    newConfig["screenWidth"] = desktopSize.Width;
                    newConfig["screenHeight"] = desktopSize.Height;
                }
                else // we are either currently on desktop res, or are on
                     // "other".
                {
                    NOTE << "Options: on desktop or other res ("
                         << currentSize.Width << "x" << currentSize.Height
                         << "), changing to default.";
                    // In both cases, go to default res.
                    newConfig["screenWidth"] = 1024;
                    newConfig["screenHeight"] = 768;
                }

                ShowOptionsMenu(newConfig);
            }
            else if (event["button"] == EMI_OPTIONS_SHADERS)
            {
                newConfig["shadersEnabled"] =
                    newConfig["shadersEnabled"].To<bool>() ^ 1;
                ShowOptionsMenu(newConfig);
            }
            else if (event["button"] == EMI_OPTIONS_SKY_EFFECTS)
            {
                newConfig["skyEffects"] =
                    newConfig["skyEffects"].To<bool>() ^ 1;
                ShowOptionsMenu(newConfig);
            }
            else if (event["button"] == EMI_OPTIONS_BLOOM)
            {
                newConfig["postProcessingEnabled"] =
                    newConfig["postProcessingEnabled"].To<bool>() ^ 1;
                ShowOptionsMenu(newConfig);
            }
            else if (event["button"] == EMI_OPTIONS_WIND_VOLUME)
            {
                newConfig["windVolume"] += 1;

                if (newConfig["windVolume"] > 5)
                    newConfig["windVolume"] = 0;

                apply_temp_config(newConfig);
                ShowOptionsMenu(newConfig);
            }
            else if (event["button"] == EMI_OPTIONS_MUSIC_VOLUME)
            {
                newConfig["musicVolume"] += 1;

                if (newConfig["musicVolume"] > 1)
                    newConfig["musicVolume"] = 0;

                apply_temp_config(newConfig);
                ShowOptionsMenu(newConfig);
            }
            else if (event["button"] == EMI_OPTIONS_SFX_VOLUME)
            {
                newConfig["sfxVolume"] += 1;

                if (newConfig["sfxVolume"] > 5)
                    newConfig["sfxVolume"] = 0;

                apply_temp_config(newConfig);
                ShowOptionsMenu(newConfig);

                // Play some test SFX here.
                // positioned at camera pos + some offset

                ASSERT(levelPreview);

                ISound3D *sound = levelPreview->GetUtilSound();

                core::vector3df dist(0, 0, 4);
                core::vector3df offset;

                core::matrix4 mat;
                mat.setRotationDegrees(world->GetCamera()->GetRotation());
                mat.rotateVect(offset, dist);

                sound->SetPosition(world->GetCamera()->GetPosition() + offset);
                sound->Play(paths::get_sfx("balloonpush.ogg"));
            }
            else if (event["button"] == EMI_OPTIONS_MOVEMENT)
            {
                newConfig["gridBasedMovement"] =
                    newConfig["gridBasedMovement"].To<bool>() ^ 1;
                apply_temp_config(newConfig);
                ShowOptionsMenu(newConfig);
            }
            else if (event["button"] == EMI_OPTIONS_MOUSE_INVERT)
            {
                if (newConfig["invertMouseX"] && newConfig["invertMouseY"])
                {
                    newConfig["invertMouseX"] = false;
                    newConfig["invertMouseY"] = false;
                }
                else if (newConfig["invertMouseX"])
                {
                    newConfig["invertMouseX"] = false;
                    newConfig["invertMouseY"] = true;
                }
                else if (newConfig["invertMouseY"])
                {
                    newConfig["invertMouseX"] = true;
                    newConfig["invertMouseY"] = true;
                }
                else
                {
                    newConfig["invertMouseX"] = true;
                }

                apply_temp_config(newConfig);
                ShowOptionsMenu(newConfig);
            }
        }
        else if (event["menu"] == LEVEL_SELECT_MENU_ID)
        {
            // Either Previous or Next has been clicked.
            // event["button"] == EMI_LEVEL_PREV

            ASSERT(levelPreview);

            core::stringc oldLevelName = levelPreview->GetShortName();

            // (level select buttons should only have been shown if there are >=
            // 1 levels)
            ASSERT(levelFileNames.size());

            // Find where last level was in list.
            u32 i = 0;
            for (; i < levelFileNames.size(); i++)
            {
                if (levelFileNames[i] == oldLevelName)
                    break;
            }

            // not found?
            if (i >= levelFileNames.size())
            {
                // No change... Don't replace level.
            }
            else // was found
            {
                core::stringc newLevelName = oldLevelName;

                if (event["button"] == EMI_LEVEL_PREV)
                {
                    // Go to previous level, if one exists.
                    if (i > 0)
                    {
                        newLevelName = levelFileNames[i - 1];

                        // Let's save the selected one as the current.
                        if (i > 1)
                            file::put(get_full_save_path(),
                                      levelFileNames[i - 2]);

                        // ugliest of hacks ever
                        // since we need to save the level before, but that has
                        // been excluded from the levelFileNames list... :S
                        // so we just hard code it.
                        if (i == 1)
                            file::put(get_full_save_path(), "first.lev");
                    }
                }
                else if (event["button"] == EMI_LEVEL_NEXT)
                {
                    // Go to next level, if one exists.
                    if (i < levelFileNames.size() - 1)
                    {
                        newLevelName = levelFileNames[i + 1];

                        // Let's save the selected one as the current.
                        file::put(get_full_save_path(), levelFileNames[i]);
                    }
                }
                else
                {
                    WARN << "invalid button id";
                }

                levelPreview->ReplaceWith(level_path_rel_exe(newLevelName));
                UpdateLevelCamera();
            }

            // Re-show prev/next buttons
            delete menuLevelSelect;
            menuLevelSelect = nullptr;
            CreateLevelSelectButtons();
        }
    }
}
