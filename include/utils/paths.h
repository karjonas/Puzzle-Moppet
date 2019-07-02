#pragma once

#include "litha_internal.h"

namespace paths
{
core::stringc get_data_dir();
core::stringc get_media_dir();
core::stringc get_levels_list_file();
core::stringc get_levels_name_file();
core::stringc get_tutorial_texts_dir();
core::stringc get_save_score_file(const io::path settings_dir,
                                  const core::stringc &levelName);
core::stringc get_perfect_score_file(const core::stringc &levelName);
core::stringc get_level(const core::stringc &filename);
core::stringc get_sfx(const core::stringc &filename);
core::stringc get_font(const core::stringc &filename);
} // namespace paths
