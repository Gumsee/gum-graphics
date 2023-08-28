#pragma once
#include <Desktop/Window.h>
#include <string>
#include "Framebuffer.h"
#include "System/MemoryManagement.h"
#include "System/Output.h"

namespace Gum {
namespace Graphics
{
    extern void enableFeature(const unsigned int& feature);
    extern void disableFeature(const unsigned int& feature);
    extern void renderWireframe(const bool wireframe);
    extern void cullBackside(const bool cullback);

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
}}