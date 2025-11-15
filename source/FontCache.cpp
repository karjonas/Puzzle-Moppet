#include "FontCache.h"

#include "Litha.h"

#include <map>
#include <tuple>
#include <string>

namespace
{
// Map to store the cached font pointers.
// Each font pointer stored here holds ONE reference count from the cache
// itself.
std::map<std::tuple<std::string, u32, float>, irr::gui::CGUITTFont*> m_cachedFonts;

void clearCache()
{
    if (m_cachedFonts.empty())
        return;

    auto it = m_cachedFonts.begin();
    while (it != m_cachedFonts.end())
    {
        // Only remove if the font is ONLY referenced by the cache (RC == 1)
        if (it->second->getReferenceCount() == 1)
        {
            // 1. Drop the reference held by the cache. Since RC was 1, this
            // deletes the object.
            it->second->drop();

            // 2. Erase the element from the map and advance the iterator to the
            // next valid element.
            it = m_cachedFonts.erase(it);
        }
        else
        {
            // Font is still in use elsewhere (RC > 1), keep it in the cache and
            // advance iterator.
            ++it;
        }
    }
}
} // namespace

irr::gui::CGUITTFont* FontCache::getFont(irr::IrrlichtDevice* device,
                                         const char* fontPath,
                                         unsigned int size, float outline)
{
    if (!device)
        return nullptr;

    irr::video::IVideoDriver* driver = device->getVideoDriver();
    irr::io::IFileSystem* fs = device->getFileSystem();

    auto key = std::make_tuple(fontPath, size, outline);

    // Check if the font is already cached
    auto it = m_cachedFonts.find(key);
    if (it != m_cachedFonts.end())
    {
        irr::gui::CGUITTFont* font = it->second;
        font->grab();
        clearCache();
        return font;
    }

    // Font not found in cache, create it
    bool fontAntiAlias = true;
    bool fontTransparency = true;

    // CGUITTFont::createTTFont returns a font with a reference count of 1.
    irr::gui::CGUITTFont* newFont =
        irr::gui::CGUITTFont::createTTFont(driver, fs, fontPath, size,
                                           fontAntiAlias, fontTransparency,
                                           outline);

    if (newFont)
    {
        // Store the font in the map (retains the ref count of 1 for the cache)
        newFont->grab();
        m_cachedFonts[key] = newFont;
    }

    clearCache();
    return newFont;
}

void FontCache::shutdown()
{
    // MUST drop all references BEFORE Irrlicht destroys textures/glyph pages.
    for (auto& pair : m_cachedFonts)
        pair.second->drop();
    m_cachedFonts.clear();
}
