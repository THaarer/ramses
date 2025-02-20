//  -------------------------------------------------------------------------
//  Copyright (C) 2012 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "internal/RendererLib/RenderExecutorInternalState.h"
#include "internal/SceneGraph/SceneAPI/EDataType.h"
#include "internal/SceneGraph/SceneAPI/EFixedSemantics.h"

namespace ramses::internal
{
    class IDevice;
    struct RenderingContext;
    class FrameTimer;
    class IScene;

    class RenderExecutor
    {
    public:
        RenderExecutor(IDevice& device, RenderingContext& renderContext, const FrameTimer* frameTimer = nullptr);

        [[nodiscard]] SceneRenderExecutionIterator executeScene(const RendererCachedScene& scene) const;

        // This is exposed and can be modified but acts as a global parameter
        static constexpr const uint32_t DefaultNumRenderablesToRenderInBetweenTimeBudgetChecks = 10u;
        static uint32_t NumRenderablesToRenderInBetweenTimeBudgetChecks;

    protected:
        mutable RenderExecutorInternalState m_state;

        void executeRenderable      () const;
        void executeRenderTarget    (RenderTargetHandle renderTarget) const;
        void executeRenderStates    () const;
        void executeEffectAndInputs () const;
        void executeConstant        (EDataType dataType, uint32_t elementCount, DataInstanceHandle dataInstance, DataFieldHandle dataInstancefield, DataFieldHandle uniformInputField) const;
        void executeDrawCall        () const;

        void setGlobalInternalStates    (const RendererCachedScene& scene) const;
        void setRenderableInternalStates(RenderableHandle renderableHandle) const;

        void activateRenderTarget       (RenderTargetHandle renderTarget) const;

        void resolveAndSetSemanticDataField(EFixedSemantics semantics, DataInstanceHandle dataInstHandle, DataFieldHandle dataFieldHandle) const;
        void setSemanticDataFields  () const;
        void executeCamera(CameraHandle camera) const;

    private:
        [[nodiscard]] bool executeRenderPass(const RendererCachedScene& scene, const RenderPassHandle pass) const;
        void executeBlitPass(const RendererCachedScene& scene, const BlitPassHandle pass) const;
        [[nodiscard]] bool canDiscardDepthBuffer() const;

        static RenderBufferHandle FindDepthRenderBufferInRenderTarget(const IScene& scene, RenderTargetHandle renderTarget);
    };

}
