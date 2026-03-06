#pragma once
#ifdef GUMDESKTOP_FOUND
#include <Desktop/Window.h>
#endif
#include <string>
#include "Framebuffer.h"
#include "System/MemoryManagement.h"
#include "System/Output.h"

namespace Gum {
namespace Graphics
{
    struct VARS
    {
        inline static int MAX_TEXTURE_SIZE = 0;
        inline static std::string RENDERER = "";
        inline static std::string VERSION = "";
        inline static std::string VENDOR = "";
        inline static int SHADING_LANGUAGE_MAJOR_VERSION = 0;
        inline static int SHADING_LANGUAGE_MINOR_VERSION = 0;
        inline static int MAJOR_VERSION = 0;
        inline static int MINOR_VERSION = 0;
        inline static std::string sGLExtentions = "";
    };

    extern void enableFeature(const unsigned int& feature);
    extern void disableFeature(const unsigned int& feature);
    extern void renderWireframe(const bool wireframe);
    extern void cullBackside(const bool cullback);
    extern void printInfo();
    extern void init();
    extern void loadDefaults();

    // Helper to check for extension string presence.  Adapted from:
    //   http://www.opengl.org/resources/features/OGLextensions/
    extern bool isExtensionSupported(const char *extension);

    #ifdef GUMDESKTOP_FOUND
    static void __attribute__ ((unused)) addFramebufferToWindow(Gum::Window* window)
    {
        Framebuffer* framebuffer = new Framebuffer(window->getSize(), true);
        framebuffer->bind();
        framebuffer->resetViewport();

        window->setFramebufferCallback([framebuffer](ivec2 size, int mode) {
            switch(mode)
            {
                case 0:
                    framebuffer->setSize(size);
                    framebuffer->resetViewport();
                    break;

                case 1:
                    framebuffer->bind();
                    break;

                case 2:
                    delete framebuffer;
                    break;
            };
        });
    }
    #endif
}}