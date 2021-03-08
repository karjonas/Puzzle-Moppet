#pragma once

#include <irrlicht/SColor.h>

namespace Colors
{
using SColor = irr::video::SColor;

SColor text_col();
SColor text_col_mouseover();

SColor text_level_title();
SColor text_level_default_rating();
SColor text_level_fraction();

SColor black();
SColor white();
} // namespace Colors
