#include "utils/paths.h"

#include <cstdlib>

#ifndef DATA_DIR
#define DATA_DIR "../data"
#endif

#define MEDIA_DIR DATA_DIR "/media"
#define SFX_DIR DATA_DIR "/media/sfx"
#define FONT_DIR DATA_DIR "/media/font"
#define LEVEL_BASE_PATH DATA_DIR "/levels/levels"
#define LEVEL_LIST_FILE DATA_DIR "/levels/levels.list"
#define LEVEL_NAMES_FILE DATA_DIR "/levels/level_names.ini"

namespace paths
{
core::stringc getAppdirPrefix()
{
    core::stringc prefix;
    auto appDir = getenv("APPDIR");
    if (appDir != nullptr)
        prefix += appDir;
    return prefix;
}

core::stringc get_data_dir()
{
    return getAppdirPrefix() + DATA_DIR;
}
core::stringc get_media_dir()
{
    return getAppdirPrefix() + MEDIA_DIR;
}
core::stringc get_levels_list_file()
{
    return getAppdirPrefix() + LEVEL_LIST_FILE;
}
core::stringc get_levels_name_file()
{
    return getAppdirPrefix() + LEVEL_NAMES_FILE;
}
core::stringc get_tutorial_texts_dir()
{
    core::stringc folderName;
#ifdef __APPLE__
    folderName = "tutorialtexts_mac";
#else
    folderName = "tutorialtexts";
#endif
    return paths::get_data_dir() + "/" + folderName;
}

core::stringc get_save_score_file(const io::path settings_dir,
                                  const core::stringc &levelName)
{
    const io::path save_score_dir =
        os::path::concat(settings_dir, "levelscores");

    core::stringc shortScoreFile = os::path::splitext(levelName)[0] + ".ini";
    core::stringc saveScoreFile =
        os::path::concat(save_score_dir, shortScoreFile);
    return saveScoreFile;
}

core::stringc get_perfect_score_file(const core::stringc &levelName)
{
    core::stringc shortScoreFile = os::path::splitext(levelName)[0] + ".ini";
    core::stringc perfectScoreFile =
        paths::get_data_dir() + "/perfectscores/" + shortScoreFile;
    return perfectScoreFile;
}

core::stringc get_level(const core::stringc &filename)
{
    return getAppdirPrefix() + core::stringc(LEVEL_BASE_PATH) + "/" + filename;
}
core::stringc get_sfx(const core::stringc &filename)
{
    return getAppdirPrefix() + core::stringc(SFX_DIR) + "/" + filename;
}
core::stringc get_font(const core::stringc &filename)
{
    return getAppdirPrefix() + core::stringc(FONT_DIR) + "/" + filename;
}
} // namespace paths
