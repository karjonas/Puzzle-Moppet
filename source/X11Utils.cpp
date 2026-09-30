#include "X11Utils.h"

#include <IrrCompileConfig.h>

#ifdef _IRR_COMPILE_WITH_X11_DEVICE_

#include <GL/glx.h>
#include <X11/Xutil.h>
#include <cstring>

namespace
{
bool xErrorOccurred = false;

int IgnoreXError(Display *, XErrorEvent *)
{
    xErrorOccurred = true;
    return 0;
}

bool HasExtension(const char *extensions, const char *name)
{
    size_t len = strlen(name);

    for (const char *p = extensions; p && (p = strstr(p, name)); p += len)
    {
        if ((p == extensions || p[-1] == ' ') &&
            (p[len] == ' ' || p[len] == '\0'))
            return true;
    }

    return false;
}
} // namespace

bool SetGlxSwapInterval(int interval)
{
    Display *display = glXGetCurrentDisplay();
    GLXDrawable drawable = glXGetCurrentDrawable();

    if (!display || !drawable)
        return false;

    const char *extensions =
        glXQueryExtensionsString(display, DefaultScreen(display));

    typedef void (*SwapIntervalEXT)(Display *, GLXDrawable, int);
    typedef int (*SwapIntervalMESA)(unsigned int);
    typedef int (*SwapIntervalSGI)(int);

    XSync(display, False);
    xErrorOccurred = false;
    int (*oldHandler)(Display *, XErrorEvent *) =
        XSetErrorHandler(IgnoreXError);

    bool called = true;

    if (HasExtension(extensions, "GLX_EXT_swap_control"))
    {
        ((SwapIntervalEXT)glXGetProcAddressARB(
            (const GLubyte *)"glXSwapIntervalEXT"))(display, drawable,
                                                    interval);
    }
    else if (HasExtension(extensions, "GLX_MESA_swap_control"))
    {
        ((SwapIntervalMESA)glXGetProcAddressARB(
            (const GLubyte *)"glXSwapIntervalMESA"))(interval);
    }
    else if (interval > 0 && HasExtension(extensions, "GLX_SGI_swap_control"))
    {
        // SGI does not allow an interval of 0
        ((SwapIntervalSGI)glXGetProcAddressARB(
            (const GLubyte *)"glXSwapIntervalSGI"))(interval);
    }
    else
        called = false;

    XSync(display, False);
    XSetErrorHandler(oldHandler);

    return called && !xErrorOccurred;
}

bool SetX11WindowClass(void *display, unsigned long window, const char *name,
                       const char *className)
{
    if (!display || !window)
        return false;

    XClassHint hint;
    hint.res_name = const_cast<char *>(name);
    hint.res_class = const_cast<char *>(className);
    XSetClassHint((Display *)display, (Window)window, &hint);

    return true;
}

#else

bool SetGlxSwapInterval(int) { return false; }

bool SetX11WindowClass(void *, unsigned long, const char *, const char *)
{
    return false;
}

#endif
