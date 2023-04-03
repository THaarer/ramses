//  -------------------------------------------------------------------------
//  Copyright (C) 2014 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef RAMSES_RENDERERCONFIG_H
#define RAMSES_RENDERERCONFIG_H

#include "RendererAPI/Types.h"
#include "Collections/String.h"
#include <chrono>

namespace ramses_internal
{
    class RendererConfig
    {
    public:
        RendererConfig() {}

        void setWaylandDisplayForSystemCompositorController(std::string_view wd);
        [[nodiscard]] std::string_view getWaylandDisplayForSystemCompositorController() const;

        void enableSystemCompositorControl();
        [[nodiscard]] Bool getSystemCompositorControlEnabled() const;

        [[nodiscard]] std::chrono::microseconds getFrameCallbackMaxPollTime() const;
        void setFrameCallbackMaxPollTime(std::chrono::microseconds pollTime);
        void setRenderthreadLooptimingReportingPeriod(std::chrono::milliseconds period);
        [[nodiscard]] std::chrono::milliseconds getRenderThreadLoopTimingReportingPeriod() const;

    private:
        String m_waylandDisplayForSystemCompositorController;
        Bool m_systemCompositorEnabled = false;
        std::chrono::microseconds m_frameCallbackMaxPollTime{10000u};
        std::chrono::milliseconds m_renderThreadLoopTimingReportingPeriod { 0 }; // zero deactivates reporting
    };
}

#endif
