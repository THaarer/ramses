//  -------------------------------------------------------------------------
//  Copyright (C) 2012 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "internal/SceneGraph/SceneAPI/IScene.h"
#include "internal/SceneGraph/SceneAPI/Renderable.h"
#include "internal/SceneGraph/SceneAPI/SceneTypes.h"
#include "internal/SceneGraph/SceneAPI/TextureSampler.h"
#include "internal/RendererLib/RendererResourceManager.h"
#include "RendererResourceManagerMock.h"
#include "EmbeddedCompositingManagerMock.h"
#include "DeviceMock.h"
#include "SceneAllocateHelper.h"
#include "internal/SceneGraph/Scene/DataLayout.h"
#include "MockResourceHash.h"

namespace ramses::internal
{
    class TestSceneHelper
    {
    public:
        explicit TestSceneHelper(IScene& scene, bool indexArrayAvailable = true)
            : m_scene(scene)
            , m_sceneAllocator(m_scene)
            , m_sceneID(scene.getSceneId())
            , m_indexArrayAvailable(indexArrayAvailable)
        {
            if (m_indexArrayAvailable)
            {
                ON_CALL(resourceManager, getResourceDeviceHandle(MockResourceHash::IndexArrayHash)).WillByDefault(Return(DeviceMock::FakeIndexBufferDeviceHandle));
            }
            else
            {
                ON_CALL(resourceManager, getResourceDeviceHandle(MockResourceHash::IndexArrayHash)).WillByDefault(Return(DeviceResourceHandle::Invalid()));
            }

            DataFieldInfoVector geometryDataFields(2u);
            geometryDataFields[indicesField.asMemoryHandle()] = DataFieldInfo(EDataType::Indices, 1u, EFixedSemantics::Indices);
            geometryDataFields[vertAttribField.asMemoryHandle()] = DataFieldInfo(EDataType::Vector3Buffer, 1u, EFixedSemantics::Invalid);
            m_sceneAllocator.allocateDataLayout(geometryDataFields, MockResourceHash::EffectHash, testGeometryLayout);

            DataFieldInfoVector uniformDataFields(2u);
            uniformDataFields[dataField.asMemoryHandle()] = DataFieldInfo(EDataType::Float);
            uniformDataFields[samplerField.asMemoryHandle()] = DataFieldInfo(EDataType::TextureSampler2D);
            m_sceneAllocator.allocateDataLayout(uniformDataFields, MockResourceHash::EffectHash, testUniformLayout);
        }

        RenderGroupHandle createRenderGroup(RenderPassHandle pass1 = RenderPassHandle::Invalid(), RenderPassHandle pass2 = RenderPassHandle::Invalid())
        {
            const RenderGroupHandle renderGroupHandle = m_sceneAllocator.allocateRenderGroup();
            if (pass1.isValid())
            {
                m_scene.addRenderGroupToRenderPass(pass1, renderGroupHandle, 0);
            }
            if (pass2.isValid())
            {
                m_scene.addRenderGroupToRenderPass(pass2, renderGroupHandle, 0);
            }

            return renderGroupHandle;
        }

        RenderableHandle createRenderable(RenderGroupHandle group1 = RenderGroupHandle::Invalid(), RenderGroupHandle group2 = RenderGroupHandle::Invalid())
        {
            const NodeHandle nodeHandle = m_sceneAllocator.allocateNode();
            const RenderableHandle renderableHandle = m_sceneAllocator.allocateRenderable(nodeHandle);

            if (group1.isValid())
            {
                m_scene.addRenderableToRenderGroup(group1, renderableHandle, 0);
            }
            if (group2.isValid())
            {
                m_scene.addRenderableToRenderGroup(group2, renderableHandle, 0);
            }

            return renderableHandle;
        }

        void removeRenderable(RenderableHandle renderable, RenderGroupHandle group1 = RenderGroupHandle::Invalid(), RenderGroupHandle group2 = RenderGroupHandle::Invalid())
        {
            m_scene.releaseRenderable(renderable);
            if (group1.isValid())
            {
                m_scene.removeRenderableFromRenderGroup(group1, renderable);
            }
            if (group2.isValid())
            {
                m_scene.removeRenderableFromRenderGroup(group2, renderable);
            }
        }

        RenderPassHandle createRenderPassWithCamera()
        {
            const RenderPassHandle pass = m_sceneAllocator.allocateRenderPass();
            const auto dataLayout = m_sceneAllocator.allocateDataLayout({DataFieldInfo{EDataType::Vector2I}, DataFieldInfo{EDataType::Vector2I}}, ResourceContentHash::Invalid());
            const CameraHandle camera = m_sceneAllocator.allocateCamera(ECameraProjectionType::Perspective, m_sceneAllocator.allocateNode(), m_sceneAllocator.allocateDataInstance(dataLayout));
            m_scene.setRenderPassCamera(pass, camera);
            return pass;
        }

        BlitPassHandle createBlitPassWithDummyRenderBuffers()
        {
            const RenderBufferHandle sourceRenderBuffer = m_sceneAllocator.allocateRenderBuffer({ 16u, 12u, EPixelStorageFormat::R8, ERenderBufferAccessMode::ReadWrite, 0u });
            const RenderBufferHandle destinationRenderBuffer = m_sceneAllocator.allocateRenderBuffer({ 16u, 12u, EPixelStorageFormat::R8, ERenderBufferAccessMode::ReadWrite, 0u });
            const BlitPassHandle pass = m_sceneAllocator.allocateBlitPass(sourceRenderBuffer, destinationRenderBuffer);

            return pass;
        }

        void createRenderTarget()
        {
            m_sceneAllocator.allocateRenderTarget(renderTarget);
            m_sceneAllocator.allocateRenderBuffer({ 16u, 12u, EPixelStorageFormat::R8, ERenderBufferAccessMode::ReadWrite, 0u }, renderTargetColorBuffer);
            m_scene.addRenderTargetRenderBuffer(renderTarget, renderTargetColorBuffer);
        }

        template <typename TextureContentHandle>
        TextureSamplerHandle createTextureSampler(TextureContentHandle handleOrHash)
        {
            return m_sceneAllocator.allocateTextureSampler({ {}, handleOrHash });
        }

        TextureSamplerHandle createTextureSamplerWithFakeTexture()
        {
            return createTextureSampler(MockResourceHash::TextureHash);
        }

        DataInstanceHandle createAndAssignUniformDataInstance(RenderableHandle renderable, TextureSamplerHandle sampler)
        {
            const DataInstanceHandle uniformData = m_sceneAllocator.allocateDataInstance(testUniformLayout);
            m_scene.setRenderableDataInstance(renderable, ERenderableDataSlotType_Uniforms, uniformData);
            m_scene.setDataTextureSamplerHandle(uniformData, samplerField, sampler);

            return uniformData;
        }

        DataInstanceHandle createAndAssignVertexDataInstance(RenderableHandle renderable)
        {
            const DataInstanceHandle geometryData = m_sceneAllocator.allocateDataInstance(testGeometryLayout);
            m_scene.setRenderableDataInstance(renderable, ERenderableDataSlotType_Geometry, geometryData);

            return geometryData;
        }

        void setResourcesToRenderable(
            RenderableHandle renderable,
            bool setVertices = true,
            bool setIndices = true)
        {
            auto vertexData = m_scene.getRenderable(renderable).dataInstances[ERenderableDataSlotType_Geometry];
            if (setVertices)
                m_scene.setDataResource(vertexData, vertAttribField, MockResourceHash::VertArrayHash, DataBufferHandle::Invalid(), 0u, 0u, 0u);
            if (setIndices)
                m_scene.setDataResource(vertexData, indicesField, MockResourceHash::IndexArrayHash, DataBufferHandle::Invalid(), 0u, 0u, 0u);
        }

        template <typename TextureContentHandle>
        void recreateSamplerWithDifferentContent(TextureSamplerHandle handle, TextureContentHandle contentHandleOrHash)
        {
            const auto& samplerStates = m_scene.getTextureSampler(handle).states;
            m_scene.releaseTextureSampler(handle);
            m_scene.allocateTextureSampler({ samplerStates, contentHandleOrHash }, handle);
            [[maybe_unused]] bool hasDataSlot = false;
            for (DataSlotHandle d(0u); d < m_scene.getDataSlotCount(); ++d)
                hasDataSlot |= (m_scene.isDataSlotAllocated(d) && m_scene.getDataSlot(d).attachedTextureSampler == handle);
            assert(!hasDataSlot && "Recreating sampler that had data slot assigned, data slot must be recreated as well");
        }

    public:
        IScene& m_scene;
        SceneAllocateHelper m_sceneAllocator;
        const SceneId m_sceneID;
        StrictMock<EmbeddedCompositingManagerMock> embeddedCompositingManager;
        StrictMock<RendererResourceManagerMock>    resourceManager;

        const DataLayoutHandle testUniformLayout            { 0u };
        const DataLayoutHandle testGeometryLayout           { 2u };
        const DataFieldHandle indicesField                  { 0u };
        const DataFieldHandle vertAttribField               { 1u };
        const DataFieldHandle dataField                     { 0u };
        const DataFieldHandle samplerField                  { 1u };
        const RenderTargetHandle renderTarget               { 13u };
        const RenderBufferHandle renderTargetColorBuffer    { 5u };

    private:
        bool m_indexArrayAvailable;
    };
}
