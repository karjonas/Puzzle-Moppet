#include "Colors.h"

namespace Colors
{
using SColor = irr::video::SColor;

SColor text_col()
{
    return SColor(200, 255, 255, 255);
}
SColor text_col_mouseover()
{
    return SColor(255, 176, 196, 222); // LightSteelBlue
}
SColor text_col_disabled()
{
    return SColor(50, 255, 255, 255);
}

SColor text_level_title()
{
    return SColor(200, 255, 255, 255);
}
SColor text_level_default_rating()
{
    return SColor(200, 255, 255, 255);
}
SColor text_level_fraction()
{
    return SColor(200, 255, 255, 255);
}

SColor black()
{
    return SColor(0, 0, 0, 0);
}
SColor white()
{
    return SColor(255, 255, 255, 255);
}
} // namespace Colors
