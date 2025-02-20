//  -------------------------------------------------------------------------
//  Copyright (C) 2012 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "internal/RendererLib/RendererSceneUpdater.h"
#include "RendererResourceManagerMock.h"
#include "gmock/gmock.h"

namespace ramses::internal
{
    class RendererSceneUpdaterPartialMock : public RendererSceneUpdater
    {
    public:
        RendererSceneUpdaterPartialMock(DisplayHandle display, IPlatform& platform, const Renderer& renderer, const RendererScenes& rendererScenes, const SceneStateExecutor& sceneStateExecutor,
            const RendererEventCollector& rendererEventCollector, const FrameTimer& frameTimer, const SceneExpirationMonitor& expirationMonitor, IThreadAliveNotifier& notifier);
        ~RendererSceneUpdaterPartialMock() override;

        MOCK_METHOD(void, handleSceneUpdate, (SceneId sceneId, SceneUpdate&& update), (override));
        MOCK_METHOD(void, handlePickEvent, (SceneId sceneId, glm::vec2 coords), (override));
        MOCK_METHOD(std::unique_ptr<IRendererResourceManager>, createResourceManager, (IRenderBackend&, IEmbeddedCompositingManager&, const DisplayConfig&, IBinaryShaderCache*), (override));
    };

    class RendererSceneUpdaterFacade : public RendererSceneUpdaterPartialMock
    {
    public:
        RendererSceneUpdaterFacade(DisplayHandle display, IPlatform& platform, const Renderer& renderer, const RendererScenes& rendererScenes, const SceneStateExecutor& sceneStateExecutor,
            const RendererEventCollector& rendererEventCollector, const FrameTimer& frameTimer, const SceneExpirationMonitor& expirationMonitor, IThreadAliveNotifier& notifier);
        ~RendererSceneUpdaterFacade() override;

        void handleSceneUpdate(SceneId sceneId, SceneUpdate&& update) override;
        void handlePickEvent(SceneId sceneId, glm::vec2 coords) override;

        void setForceMapTimeout(std::chrono::milliseconds timeout);

        testing::StrictMock<RendererResourceManagerRefCountMock>* m_resourceManagerMock = nullptr;

        [[nodiscard]] bool hasResourceManager() const
        {
            return RendererSceneUpdaterPartialMock::hasResourceManager();
        }

    protected:
        std::unique_ptr<IRendererResourceManager> createResourceManager(
            IRenderBackend& renderBackend,
            IEmbeddedCompositingManager& embeddedCompositingManager,
            const DisplayConfig& displayConfig,
            IBinaryShaderCache* binaryShaderCache) override;
    };
}
