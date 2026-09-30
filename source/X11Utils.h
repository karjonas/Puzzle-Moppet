#ifndef X11_UTILS_H
#define X11_UTILS_H

// Workarounds for Irrlicht 1.8's X11 device. Kept in their own file since the
// X11 headers define macros such as None and Bool that clash with the rest of
// the engine. They do nothing (and return false) if built without X11.

// Set the swap interval (vsync) of the current GLX context, using whichever
// swap control extension the GLX implementation advertises.
// Irrlicht 1.8 calls glXSwapIntervalSGI without checking for
// GLX_SGI_swap_control, which is a fatal X error on servers that lack it
// (e.g. Xvfb). This checks first and traps X errors instead.
// Returns false if no extension is available or setting it failed.
bool SetGlxSwapInterval(int interval);

// Set the WM_CLASS of a window, which Irrlicht 1.8 leaves empty. Desktops use
// it to match the window with its .desktop file (for the icon in the task bar).
bool SetX11WindowClass(void *display, unsigned long window, const char *name,
                       const char *className);

#endif
