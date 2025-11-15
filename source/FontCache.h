namespace irr
{
class IrrlichtDevice;
namespace gui
{
class CGUITTFont;
}
} // namespace irr

class FontCache
{
public:
    static irr::gui::CGUITTFont* getFont(irr::IrrlichtDevice* device,
                                         const char* fontPath,
                                         unsigned int size, float outline);

    static void shutdown();
};
