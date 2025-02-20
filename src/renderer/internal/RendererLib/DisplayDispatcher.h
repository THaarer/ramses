//  -------------------------------------------------------------------------
//  Copyright (C) 2021 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "internal/RendererLib/DisplayBundle.h"
#include "internal/RendererLib/RendererConfig.h"
#include "internal/RendererLib/SceneDisplayTracker.h"
#include "internal/RendererLib/DisplayThread.h"
#include "internal/RendererLib/Enums/ELoopMode.h"
#include "internal/RendererLib/PlatformInterface/IPlatformFactory.h"
#include "internal/Watchdog/IThreadAliveNotifier.h"
#include <memory>

namespace ramses::internal
{
    class RendererCommandBuffer;
    class IRendererSceneEventSender;
    class Ramsh;
    class IEmbeddedCompositingManager;
    class IEmbeddedCompositor;
    class DisplayConfig;

    class DisplayDispatcher
    {
    public:
        DisplayDispatcher(
            std::unique_ptr<IPlatformFactory> platformFactory,
            RendererConfig config,
            IRendererSceneEventSender& rendererSceneSender,
            IThreadAliveNotifier& notifier);
        virtual ~DisplayDispatcher() = default;

        void dispatchCommands(RendererCommandBuffer& cmds);
        void dispatchCommands(RendererCommands& cmds);
        void doOneLoop(std::chrono::microseconds sleepTime = std::chrono::microseconds{0});

        void dispatchRendererEvents(RendererEventVector& events);
        void dispatchSceneControlEvents(RendererEventVector& events);
        void injectRendererEvent(RendererEvent&& event);
        void injectSceneControlEvent(RendererEvent&& event);
        void injectPlatformFactory(std::unique_ptr<IPlatformFactory> platformFactory);

        void startDisplayThreadsUpdating();
        void stopDisplayThreadsUpdating();
        void setLoopMode(ELoopMode loopMode);
        void setMinFrameDuration(std::chrono::microseconds minFrameDuration, DisplayHandle display);
        [[nodiscard]] std::chrono::microseconds getMinFrameDuration(DisplayHandle display) const;

        [[nodiscard]] const RendererConfig& getRendererConfig() const;

        // needed for EC tests...
        IEmbeddedCompositingManager& getECManager(DisplayHandle display);
        IEmbeddedCompositor& getEC(DisplayHandle display);

        // needed for Renderer lifecycle tests...
        [[nodiscard]] bool hasSystemCompositorController() const;

    protected:
        void preprocessCommand(RendererCommand::Variant& cmd);
        void dispatchCommand(RendererCommand::Variant&& cmd);
        [[nodiscard]] bool isSceneStateChangeEmittedFromOwningDisplay(SceneId sceneId, DisplayHandle emittingDisplay) const;

        struct Display
        {
            std::unique_ptr<IPlatform> platform;
            DisplayBundleShared displayBundle;
            std::unique_ptr<IDisplayThread> displayThread;
            RendererCommands pendingCommands;
            // TODO vaclav remove, debug only
            uint32_t lastFrameCounter = 0;
        };
        // virtual to allow mock of display thread
        virtual Display createDisplayBundle(DisplayHandle displayHandle, const DisplayConfig& dispConfig);

        std::unique_ptr<IPlatformFactory> m_platformFactory;
        const RendererConfig m_rendererConfig;
        IRendererSceneEventSender& m_rendererSceneSender;

        SceneDisplayTracker m_sceneDisplayTrackerForCommands;
        SceneDisplayTracker m_sceneDisplayTrackerForEvents;
        // use map to keep displays ordered
        std::map<DisplayHandle, Display> m_displays;

        std::unordered_map<DisplayHandle, RendererCommands> m_stashedCommandsForNewDisplays;
        RendererCommands m_stashedBroadcastCommandsForNewDisplays;

        // start/stop/framerate update or events collection are invoked from other thread,
        // they need access to displays hashmap which can change when create/destroy display executed
        std::mutex m_displaysAccessLock;

        std::mutex m_injectedEventsLock;
        RendererEventVector m_injectedRendererEvents;
        RendererEventVector m_injectedSceneControlEvents;

        bool m_threadedDisplays = false;
        bool m_displayThreadsUpdating = true;
        ELoopMode m_loopMode = ELoopMode::UpdateAndRender;
        std::unordered_map<DisplayHandle, std::chrono::microseconds> m_minFrameDurationsPerDisplay;

        IThreadAliveNotifier& m_notifier;

        // flag to force context enable for next doOneLoop (relevant only for non-threaded mode)
        bool m_forceContextEnableNextLoop = false;

        // to avoid re-allocs
        RendererCommands m_tmpCommands;
        RendererEventVector m_tmpEvents;

        int m_cmdDispatchLoopsSinceLastEventDispatch = 0;

        // TODO vaclav remove, debug only
        int m_loopCounter = 0;
    };
}
