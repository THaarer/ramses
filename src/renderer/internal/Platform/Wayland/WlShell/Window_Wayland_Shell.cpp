//  -------------------------------------------------------------------------
//  Copyright (C) 2017 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "internal/Platform/Wayland/WlShell/Window_Wayland_Shell.h"
#include "internal/Core/Utils/LogMacros.h"

namespace ramses::internal
{
    Window_Wayland_Shell::Window_Wayland_Shell(const DisplayConfig& displayConfig,
                                               IWindowEventHandler& windowEventHandler,
                                               uint32_t               id,
                                               std::chrono::microseconds frameCallbackMaxPollTime)
        : Window_Wayland(displayConfig, windowEventHandler, id, frameCallbackMaxPollTime)
    {
    }

    Window_Wayland_Shell::~Window_Wayland_Shell()
    {
        if (m_shellSurface)
        {
            wl_shell_surface_destroy(m_shellSurface);
        }

        if (m_shellRegistry)
        {
            wl_shell_destroy(m_shellRegistry);
        }
    }

    void Window_Wayland_Shell::registryGlobalCreated(wl_registry* wl_registry,
                                                     uint32_t     name,
                                                     const char*  interface,
                                                     uint32_t     version)
    {
        Window_Wayland::registryGlobalCreated(wl_registry, name, interface, version);

        if (0 == strcmp(interface, "wl_shell"))
        {
            m_shellRegistry = static_cast<wl_shell*>(wl_registry_bind(wl_registry, name, &wl_shell_interface, 1));
            LOG_DEBUG(CONTEXT_RENDERER, "Window_Wayland::registryGlobalCreated Bound wl_shell");
        }
    }

    bool Window_Wayland_Shell::createSurface()
    {
        if (nullptr != m_shellRegistry)
        {
            LOG_DEBUG(CONTEXT_RENDERER, "Window_Wayland::createShellSurface create wl_shell_surface");
            m_shellSurface = wl_shell_get_shell_surface(m_shellRegistry, m_wlContext.surface);
            wl_shell_surface_set_title(m_shellSurface, m_windowName.c_str());
            wl_shell_surface_set_toplevel(m_shellSurface);
            registerSurfaceListener();
        }
        else
        {
            LOG_ERROR(CONTEXT_RENDERER, "Window_Wayland::createShellSurface shellRegistry not available!");
            return false;
        }
        return true;
    }

    void Window_Wayland_Shell::setTitle(std::string_view title)
    {
        Window_Wayland::setTitle(title);
        if (nullptr != m_shellSurface)
        {
            wl_shell_surface_set_title(m_shellSurface, m_windowName.c_str());
        }
    }

    void Window_Wayland_Shell::configureCallback(void* userData, [[maybe_unused]] wl_shell_surface* surface, [[maybe_unused]] uint32_t edges, int32_t width, int32_t height)
    {
        auto* window = static_cast<Window_Wayland_Shell*>(userData);
        window->m_eventHandler.onResize(width, height);
    }

    void Window_Wayland_Shell::pingCallback([[maybe_unused]] void* userData, wl_shell_surface* surface, uint32_t serial)
    {
        wl_shell_surface_pong(surface, serial);
    }

    void Window_Wayland_Shell::popupDoneCallback([[maybe_unused]] void* userData, [[maybe_unused]] wl_shell_surface* surface)
    {
        LOG_DEBUG(CONTEXT_RENDERER, "Window_Wayland::popupDoneCallback called");
    }

    void Window_Wayland_Shell::registerSurfaceListener()
    {
        wl_shell_surface_add_listener(m_shellSurface, &m_shellSurfaceListener, this);
    }
}
