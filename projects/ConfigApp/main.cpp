#include "Litha.h"
#include "utils/paths.h"

#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// This should be changed to match the other Litha Engine project's hard coded
// default settings, those passed to CreateEngine.
VariantMap PROJECT_DEFAULT_SETTINGS()
{
    VariantMap settings;
    settings["appName"] = "Puzzle Moppet";
    settings["screenWidth"] = 800;
    settings["screenHeight"] = 600;
    settings["fullScreen"] = false;
    settings["softwareMode"] = false;
    settings["shadersEnabled"] = true;
    settings["postProcessingEnabled"] = true;
    settings["vsync"] = true;
    settings["maxRenderFPS"] = 60;
    return settings;
}

namespace gui_ids
{
enum
{
    BUTTON_CANCEL = 100,
    BUTTON_SAVE_AND_EXIT = 101,

    COMBOBOX_SCREEN_RES = 200,
    CHECKBOX_FULL_SCREEN = 201,
    CHECKBOX_SHADERS = 202,
    CHECKBOX_POST_PROCESSING = 203
};

// Should only be called after engine has been created.
gui::IGUIElement* getElementFromID(s32 id,
                                   gui::IGUIElement* startElement = nullptr)
{
    if (startElement == nullptr)
        startElement = GetEngine()
                           ->GetIrrlichtDevice()
                           ->getGUIEnvironment()
                           ->getRootGUIElement();

    if (startElement->getID() == id)
        return startElement;

    core::list<gui::IGUIElement*>::ConstIterator begin =
        startElement->getChildren().begin();
    core::list<gui::IGUIElement*>::ConstIterator end =
        startElement->getChildren().end();

    for (; begin != end; begin++)
    {
        if (gui::IGUIElement* found = getElementFromID(id, *begin))
            return found;
    }

    return nullptr;
}
} // namespace gui_ids

gui::IGUIComboBox* comboboxScreenRes;
gui::IGUICheckBox* checkboxFullScreen;
gui::IGUICheckBox* checkboxShaders;
gui::IGUICheckBox* checkboxPostProcessing;

// Current state
VariantMap projectSettings;

// List of all available video modes.
Set<core::dimension2du> videoModes; // using a set avoids duplicates (different
                                    // bit depths)

// Should only be called after InitGUI has set up the videoMode list.
bool selectVideoModeInComboBox(const core::dimension2du& dim)
{
    // Search for listed video mode in combo box that matches current settings.
    for (u32 i = 0; i < videoModes.size(); i++)
    {
        if (videoModes[i] == dim)
        {
            comboboxScreenRes->setSelected(i);
            return true;
        }
    }
    return false;
}

void InitSettings()
{
    // This is the same process used in Engine constructor
    // to determine the final settings for an app.

    // Find the other project's default settings.
    // It's the default engine settings overridden by the default project
    // settings.
    projectSettings = PROJECT_DEFAULT_SETTINGS();

    // And then override by the settings from the config file...
    VariantMap configFileSettings = file::loadsettings(
        os::path::concat(os::getcustomappdata(projectSettings["appName"]),
                         projectSettings["appName"] + ".ini"));

    if (configFileSettings.size())
        override_variantmap(projectSettings, configFileSettings);
}

void InitGUI(IrrlichtDevice* device)
{
    // Find available video modes.
    video::IVideoModeList* vmList = device->getVideoModeList();

    // We ignore the bit depth, as the maximum available is always chosen.
    // (32 bits, when creating the Irrlicht device).

    for (s32 i = 0; i < vmList->getVideoModeCount(); i++)
        videoModes.Insert(vmList->getVideoModeResolution(i));

    for (auto& videoMode : videoModes)
    {
        core::stringw resStr;
        resStr += videoMode.Width;
        resStr += "x";
        resStr += videoMode.Height;
        comboboxScreenRes->addItem(resStr.c_str());
    }

    // Now we set up the GUI depending on the previously saved settings.

    ASSERT(projectSettings["screenWidth"] > 0);
    ASSERT(projectSettings["screenHeight"] > 0);

    if (videoModes.size() == 0)
    {
        WARN << "No video modes were found...";
    }
    else
    {
        if (!selectVideoModeInComboBox(
                core::dimension2du(projectSettings["screenWidth"],
                                   projectSettings["screenHeight"])))
        {
            // Not found!? Fall back to the project default...
            // This would presumably only occur if the user changed their
            // monitor or something and a previously set video mode was no
            // longer supported.
            NOTE << "VideoMode [" << projectSettings["screenWidth"] << " by "
                 << projectSettings["screenHeight"]
                 << "] not found, falling back to project default...";

            // (still need to override the engine settings, to get the engine's
            // default, since the project might not have specified a video mode)
            VariantMap defaultProjectSettings =
                GetEngine()->GetDefaultSettings();
            override_variantmap(defaultProjectSettings,
                                PROJECT_DEFAULT_SETTINGS());

            ASSERT(defaultProjectSettings["screenWidth"] > 0);
            ASSERT(defaultProjectSettings["screenHeight"] > 0);

            // Attempt to set the default.
            if (!selectVideoModeInComboBox(
                    core::dimension2du(defaultProjectSettings["screenWidth"],
                                       defaultProjectSettings["screenHeight"])))
            {
                NOTE << "Default VideoMode [" << projectSettings["screenWidth"]
                     << " by " << projectSettings["screenHeight"]
                     << "] not found, will use the first in list... ["
                     << videoModes[0].Width << " by " << videoModes[0].Height
                     << "]";

                // Default is not present either!?
                // SO, just select the first in the list...
                comboboxScreenRes->setSelected(0);
            }
        }
    }

    checkboxFullScreen->setChecked(projectSettings["fullScreen"]);
    checkboxShaders->setChecked(projectSettings["shadersEnabled"]);
    checkboxPostProcessing->setChecked(
        projectSettings["postProcessingEnabled"]);
}

void setSkinTransparency(s32 alpha, irr::gui::IGUISkin* skin)
{
    for (s32 i = 0; i < irr::gui::EGDC_COUNT; ++i)
    {
        video::SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(alpha);
        skin->setColor((EGUI_DEFAULT_COLOR)i, col);
    }
}

void SaveSettings()
{
    // First, we must update the old state (projectSettings) with newly set GUI
    // state.

    if (comboboxScreenRes->getSelected() != -1)
    {
        s32 selected = comboboxScreenRes->getSelected();

        ASSERT((u32)selected < videoModes.size());

        projectSettings["screenWidth"] = videoModes[selected].Width;
        projectSettings["screenHeight"] = videoModes[selected].Height;
    }
    else
    {
        WARN << "no screen resolution selected...";
    }

    projectSettings["fullScreen"] = checkboxFullScreen->isChecked();
    projectSettings["shadersEnabled"] = checkboxShaders->isChecked();
    projectSettings["postProcessingEnabled"] =
        checkboxPostProcessing->isChecked();

    // Now write back the changes to .ini file.
    // Note that this writes *all* settings: project specific settings plus any
    // default engine settings that were not overridden by project.

    // Also, we don't want to store appName in the settings file.

    core::stringc savePath =
        os::path::concat(os::getcustomappdata(projectSettings["appName"]),
                         projectSettings["appName"] + ".ini");

    projectSettings.erase("appName");

    if (!file::savesettings(savePath, projectSettings))
    {
        WARN << "Could not save settings: " << savePath;
    }
}

class MyEventReceiver : public IEventReceiver
{
public:
    MyEventReceiver(IrrlichtDevice& device)
        : device(device)
    {
    }

    bool OnEvent(const SEvent& event) override
    {
        if (event.EventType == EET_GUI_EVENT)
        {
            s32 id = event.GUIEvent.Caller->getID();
            IGUIEnvironment* env = device.getGUIEnvironment();

            switch (event.GUIEvent.EventType)
            {
            case EGET_SCROLL_BAR_CHANGED:
                break;

            case EGET_BUTTON_CLICKED:
                switch (id)
                {
                case gui_ids::BUTTON_CANCEL:
                    device.closeDevice();
                    return true;
                case gui_ids::BUTTON_SAVE_AND_EXIT:
                    SaveSettings();
                    device.closeDevice();
                    return true;

                default:
                    return false;
                }
                break;

            case EGET_FILE_SELECTED:
            {
            }
            break;

            default:
                break;
            }
        }

        return false;
    }

private:
    IrrlichtDevice& device;
};

int main(int argc, const char** argv)
{
    VariantMap settings;
    settings["appName"] = "Puzzle Moppet";
    settings["windowTitle"] = "Puzzle Moppet Configuration";
    settings["screenWidth"] = 250;
    settings["screenHeight"] = 330;

    // create device and exit if creation failed
    IrrlichtDevice* device =
        createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(320, 480));

    if (device == nullptr)
        return 1; // could not create selected driver.

    device->setWindowCaption(L"Puzzle Moppet Configuration");
    device->setResizable(true);

    video::IVideoDriver* driver = device->getVideoDriver();
    IGUIEnvironment* env = device->getGUIEnvironment();

    IGUISkin* skin = env->getSkin();

    skin->setFont(env->getFont(paths::get_font("lucida.xml")));
    setSkinTransparency(255.f, skin);

    {
        // Screen mode
        env->addStaticText(L"Screen mode:", rect<s32>(20, 20, 130, 50));

        comboboxScreenRes = env->addComboBox(rect<s32>(60, 50, 200, 80));
        {
            // Find available video modes.
            video::IVideoModeList* vmList = device->getVideoModeList();

            // We ignore the bit depth, as the maximum available is always
            // chosen. (32 bits, when creating the Irrlicht device).
            for (s32 i = 0; i < vmList->getVideoModeCount(); i++)
                videoModes.Insert(vmList->getVideoModeResolution(i));

            for (auto& videoMode : videoModes)
            {
                core::stringw resStr;
                resStr += videoMode.Width;
                resStr += "x";
                resStr += videoMode.Height;
                comboboxScreenRes->addItem(resStr.c_str());
            }
        }

        // Full screen
        env->addStaticText(L"Full screen", rect<s32>(90, 100, 160, 130));
        checkboxFullScreen = env->addCheckBox(L"", rect<s32>(60, 90, 160, 130));
    }

    {
        // Graphics
        env->addStaticText(L"Graphics:", rect<s32>(20, 150, 140, 171));

        env->addStaticText(L"Shaders", rect<s32>(90, 180, 150, 210));
        checkboxShaders = env->addCheckBox(L"", rect<s32>(60, 170, 150, 209));

        env->addStaticText(L"Post processing effects",
                           rect<s32>(90, 220, 230, 246));
        checkboxPostProcessing =
            env->addCheckBox(L"", rect<s32>(60, 210, 220, 253));
    }

    { // Exit buttons
        constexpr size_t BUTTON_WIDTH = 130;
        constexpr size_t BUTTON_HEIGHT = 32;

        constexpr size_t BUTTON_LEFT_X0 = 25;
        constexpr size_t BUTTON_LEFT_X1 = BUTTON_LEFT_X0 + BUTTON_WIDTH;

        constexpr size_t BUTTON_Y0 = 280;
        constexpr size_t BUTTON_Y1 = BUTTON_Y0 + BUTTON_HEIGHT;

        constexpr size_t BUTTON_RIGHT_X0 = BUTTON_LEFT_X1 + 10;
        constexpr size_t BUTTON_RIGHT_X1 = BUTTON_RIGHT_X0 + BUTTON_WIDTH;

        env->addButton(rect<s32>(BUTTON_LEFT_X0, BUTTON_Y0, BUTTON_LEFT_X1,
                                 BUTTON_Y1),
                       nullptr, gui_ids::BUTTON_CANCEL, L"Exit without saving",
                       L"Exits Program without saving");
        env->addButton(rect<s32>(BUTTON_RIGHT_X0, BUTTON_Y0, BUTTON_RIGHT_X1,
                                 BUTTON_Y1),
                       nullptr, gui_ids::BUTTON_SAVE_AND_EXIT, L"Save & Exit",
                       L"Exits Program and saves");
    }

    // Then create the event receiver, giving it that context structure.
    MyEventReceiver receiver(*device);

    // And tell the device to use our custom event receiver.
    device->setEventReceiver(&receiver);

    InitSettings();
    InitGUI(device);

    while (device->run() && driver)
    {
        driver->beginScene(true, true, SColor(0, 200, 200, 200));
        env->drawAll();
        driver->endScene();
    }

    device->drop();

    return 0;
}
