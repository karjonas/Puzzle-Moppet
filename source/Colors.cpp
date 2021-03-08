#include "Colors.h"

namespace Colors
{
using SColor = irr::video::SColor;

SColor text_col()
{
    return SColor(150, 255, 255, 255);
}
SColor text_col_mouseover()
{
    return SColor(100, 200, 200, 200);
}

SColor text_level_title()
{
    return SColor(115, 255, 255, 255);
}
SColor text_level_default_rating()
{
    return SColor(100, 255, 255, 255);
}
SColor text_level_fraction()
{
    return SColor(75, 255, 255, 255);
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
