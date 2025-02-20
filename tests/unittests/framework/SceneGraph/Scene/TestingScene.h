//  -------------------------------------------------------------------------
//  Copyright (C) 2012 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------


#pragma once

#include "internal/SceneGraph/Scene/Scene.h"
#include "internal/SceneGraph/SceneAPI/TextureEnums.h"
#include "internal/SceneGraph/SceneAPI/RenderBuffer.h"
#include "internal/SceneGraph/SceneAPI/RenderGroupUtils.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/matrix_transform_2d.hpp"

#include <cstdint>
#include <array>

namespace ramses::internal
{
    template <typename... Ts>
    std::array<std::byte, sizeof...(Ts)> make_byte_array(Ts&&... args) noexcept
    {
        return {std::byte(std::forward<Ts>(args))...};
    }

    template <class SCENE>
    class TestingScene
    {
    public:
    explicit TestingScene()
        {
            scene.allocateNode(0u, parent);
            scene.allocateNode(0u, child);
            scene.allocateNode(0u, childChild1);
            scene.allocateNode(0u, childChild2);
            scene.allocateNode(0u, childChild3);
            scene.allocateNode(0u, cameraNode);

            scene.addChildToNode(parent, child);
            scene.addChildToNode(child, childChild1);
            scene.addChildToNode(child, childChild2);
            scene.addChildToNode(child, childChild3);

            scene.allocateTransform(childChild1, t1);
            scene.allocateTransform(childChild2, t2);
            scene.allocateTransform(childChild3, t3);

            scene.setTranslation(t1, t1Translation);
            scene.setRotation   (t1, glm::vec4(t1Rotation, 1.f)   , ERotationType::Euler_ZYZ);
            scene.setScaling    (t1, t1Scaling    );

            scene.setTranslation(t2, t2Translation);
            scene.setRotation   (t2, glm::vec4(t2Rotation, 1.f)   , ERotationType::Euler_XYX);
            scene.setScaling    (t2, t2Scaling    );

            scene.setTranslation(t3, t3Translation);
            scene.setRotation   (t3, t3Rotation, ERotationType::Quaternion);
            scene.setScaling    (t3, t3Scaling    );

            scene.allocateRenderState(renderState);
            scene.setRenderStateBlendFactors(renderState, EBlendFactor::One, EBlendFactor::SrcAlpha, EBlendFactor::OneMinusSrcAlpha, EBlendFactor::DstAlpha);
            scene.setRenderStateBlendOperations(renderState, EBlendOperation::Add, EBlendOperation::Subtract);
            scene.setRenderStateBlendColor(renderState, { 0.1f, 0.2f, 0.3f, 0.4f });
            scene.setRenderStateCullMode(renderState, ECullMode::FrontFacing);
            scene.setRenderStateDrawMode(renderState, EDrawMode::Triangles);
            scene.setRenderStateDepthWrite(renderState, EDepthWrite::Disabled);
            scene.setRenderStateDepthFunc(renderState, EDepthFunc::GreaterEqual);
            scene.setRenderStateScissorTest(renderState, EScissorTest::Enabled, { 10, 20, 30, 40 });
            scene.setRenderStateStencilFunc(renderState, EStencilFunc::NotEqual, 12u, 0xAA);
            scene.setRenderStateStencilOps(renderState, EStencilOp::Increment, EStencilOp::IncrementWrap, EStencilOp::Decrement);
            const ColorWriteMask colorMask = EColorWriteFlag_Red | EColorWriteFlag_Blue;
            scene.setRenderStateColorWriteMask(renderState, colorMask);

            scene.allocateRenderable(childChild2, renderable);
            scene.setRenderableStartIndex(renderable, startIndex);
            scene.setRenderableIndexCount(renderable, indexCount);
            scene.setRenderableRenderState(renderable, renderState);
            scene.setRenderableVisibility(renderable, EVisibilityMode::Invisible);
            scene.setRenderableInstanceCount(renderable, renderableInstanceCount);
            scene.setRenderableStartVertex(renderable, startVertex);

            scene.allocateRenderable(child, renderable2);

            DataFieldInfoVector uniformLayoutDataFields{
                DataFieldInfo(EDataType::Float),
                DataFieldInfo(EDataType::Vector4F, 2u),
                DataFieldInfo(EDataType::Matrix33F, 1u, EFixedSemantics::ModelViewMatrix33),
                DataFieldInfo(EDataType::Matrix44F),
                DataFieldInfo(EDataType::TextureSampler2D),
                DataFieldInfo(EDataType::DataReference),
                DataFieldInfo(EDataType::TextureSamplerExternal),
                DataFieldInfo(EDataType::Bool),
                DataFieldInfo(EDataType::Bool)
            };
            scene.allocateDataLayout(uniformLayoutDataFields, effectHash, uniformLayout);

            DataFieldInfoVector geometryLayoutDataFields{
                DataFieldInfo(EDataType::Indices),
                DataFieldInfo(EDataType::Vector4Buffer),
                DataFieldInfo(EDataType::FloatBuffer),
                DataFieldInfo(EDataType::Vector2Buffer) //unused
            };
            scene.allocateDataLayout(geometryLayoutDataFields, effectHash, vertexLayout);

            scene.allocateDataInstance(scene.allocateDataLayout({ DataFieldInfo{ramses::internal::EDataType::Vector2I}, DataFieldInfo{ramses::internal::EDataType::Vector2I} },
                                                                ResourceContentHash::Invalid(), {}), cameraDataInstance);

            scene.allocateTextureSampler({ samplerStates, textureHash }, samplerWithTextureResource);
            scene.allocateTextureSampler({ samplerStates, renderBuffer }, samplerWithRenderBuffer);
            scene.allocateTextureSampler({ samplerStates, texture2DBuffer }, samplerWithTextureBuffer);
            scene.allocateTextureSampler({ samplerStates, TextureSampler::ContentType::ExternalTexture, ResourceContentHash::Invalid(), InvalidMemoryHandle }, samplerWithExternalTexture);

            scene.allocateDataInstance(uniformLayout, uniformData);
            scene.setDataSingleFloat(uniformData, DataFieldHandle(0u), 0.5f);
            const std::array dataVec4fArray = { glm::vec4(0.0f, 1.0f, 2.0f, 3.0f), glm::vec4(4.0f, 5.0f, 6.0f, 7.0f) };
            scene.setDataVector4fArray(uniformData, DataFieldHandle(1), 2, dataVec4fArray.data());
            scene.setDataSingleMatrix33f(uniformData, DataFieldHandle(2), glm::rotate(glm::mat3(1.f), glm::radians(5.0f)));
            scene.setDataSingleMatrix44f(uniformData, DataFieldHandle(3), glm::translate(glm::vec3{ 5.0f, 4.0f, 3.0f }));
            scene.setDataTextureSamplerHandle(uniformData, DataFieldHandle(4), samplerWithTextureResource);
            scene.setDataReference(uniformData, DataFieldHandle(5), dataRef);
            scene.setDataTextureSamplerHandle(uniformData, DataFieldHandle(6), samplerWithExternalTexture);
            scene.setDataSingleBoolean(uniformData, DataFieldHandle(7), true);
            scene.setDataSingleBoolean(uniformData, DataFieldHandle(8), false);

            scene.allocateDataInstance(vertexLayout, geometryData);
            scene.setDataResource(geometryData, DataFieldHandle(0u), indexArrayHash, DataBufferHandle::Invalid(), indexArrayDivisor, 0u, 0u);
            scene.setDataResource(geometryData, DataFieldHandle(1u), vertexArrayHash, DataBufferHandle::Invalid(), vertexArrayDivisor, 111u, 71u);
            scene.setDataResource(geometryData, DataFieldHandle(2u), ResourceContentHash::Invalid(), vertexDataBuffer, vertexArrayDivisor, 222u, 72u);

            scene.setRenderableDataInstance(renderable, ERenderableDataSlotType_Uniforms, uniformData);
            scene.setRenderableDataInstance(renderable, ERenderableDataSlotType_Geometry, geometryData);

            scene.allocateCamera(ECameraProjectionType::Perspective, cameraNode, cameraDataInstance, camera);
            scene.allocateCamera(ECameraProjectionType::Orthographic, cameraNode, cameraDataInstance, orthographicCamera);

            scene.allocateRenderTarget(renderTarget);
            scene.allocateRenderBuffer({ 1u, 2u, EPixelStorageFormat::RGBA8, ERenderBufferAccessMode::ReadWrite, 0u }, renderBuffer);
            scene.setRenderBufferProperties(renderBuffer, 23u, 42u, 3u);
            scene.addRenderTargetRenderBuffer(renderTarget, renderBuffer);

            scene.allocateRenderGroup(0u, 0u, renderGroup);
            scene.addRenderableToRenderGroup(renderGroup, renderable, 15);
            scene.addRenderableToRenderGroup(renderGroup, renderable2, 5);
            scene.removeRenderableFromRenderGroup(renderGroup, renderable2);
            scene.allocateRenderGroup(0u, 0u, renderGroup2);

            // nested render group
            scene.allocateRenderGroup(0u, 0u, nestedRenderGroupParent);
            scene.allocateRenderGroup(0u, 0u, nestedRenderGroupChild);
            scene.addRenderableToRenderGroup(nestedRenderGroupChild, renderable2, 21);
            scene.addRenderGroupToRenderGroup(nestedRenderGroupParent, nestedRenderGroupChild, 22);

            scene.allocateRenderPass(0u, renderPass);
            scene.setRenderPassClearFlag(renderPass, EClearFlag::None);
            scene.setRenderPassClearColor(renderPass, glm::vec4(0.5f, 0.0f, 1.f, 0.25f));
            scene.setRenderPassCamera(renderPass, camera);
            scene.setRenderPassRenderTarget(renderPass, renderTarget);
            scene.setRenderPassRenderOrder(renderPass, 1);
            scene.setRenderPassEnabled(renderPass, false);
            scene.setRenderPassRenderOnce(renderPass, true);

            scene.addRenderGroupToRenderPass(renderPass, renderGroup, 15);
            scene.addRenderGroupToRenderPass(renderPass, renderGroup2, 5);
            scene.removeRenderGroupFromRenderPass(renderPass, renderGroup2);

            scene.allocateBlitPass(renderBuffer, renderBuffer2, blitPass);
            scene.setBlitPassEnabled(blitPass, true);
            scene.setBlitPassRenderOrder(blitPass, 14u);
            scene.setBlitPassRegions(blitPass, blitPassSourceRectangle, blitPassDestinationRectangle);

            scene.allocateDataSlot({ EDataSlotType::TransformationConsumer, dataSlotId, parent, dataRef, textureHash, samplerWithTextureResource }, transformDataSlot);

            scene.allocateDataBuffer(EDataBufferType::IndexBuffer, EDataType::UInt32, 1024, indexDataBuffer);
            scene.updateDataBuffer(indexDataBuffer, 1u, 2u, make_byte_array(0x77, 0xAB).data());

            scene.allocateDataBuffer(EDataBufferType::VertexBuffer, EDataType::Float, 32, vertexDataBuffer);
            scene.updateDataBuffer(vertexDataBuffer, 0u, 3u, make_byte_array(0x0A, 0x1B, 0x2C).data());

            scene.allocatePickableObject(vertexDataBuffer, child, PickableObjectId{ 0u }, pickableHandle);
            scene.setPickableObjectId(pickableHandle, pickableId);
            scene.setPickableObjectCamera(pickableHandle, camera);
            scene.setPickableObjectEnabled(pickableHandle, false);

            scene.allocateTextureBuffer(EPixelStorageFormat::R8, { {8u, 8u}, {4u, 4u}, {2u, 2u} }, texture2DBuffer);
            scene.updateTextureBuffer(texture2DBuffer, 0u, 3u, 4u, 1u, 3u, make_byte_array(34u, 35u, 36u).data()); //partial update level 0
            scene.updateTextureBuffer(texture2DBuffer, 0u, 3u, 4u, 1u, 2u, make_byte_array(134u, 135u).data()); //override partial update level 0
            scene.updateTextureBuffer(texture2DBuffer, 2u, 0u, 0u, 2u, 2u, make_byte_array(00u, 10u, 01u, 11u).data()); //full update level 2

            scene.allocateSceneReference(sceneRefSceneId, sceneRef);
            scene.requestSceneReferenceState(sceneRef, RendererSceneState::Ready);
            scene.requestSceneReferenceFlushNotifications(sceneRef, true);
            scene.setSceneReferenceRenderOrder(sceneRef, -13);
        }

        const SCENE& getScene() const
        {
            return scene;
        }

        template <typename OTHERSCENE>
        void CheckEquivalentTo(const OTHERSCENE& otherScene) const
        {
            CheckNodesEquivalentTo<OTHERSCENE>(otherScene);
            CheckTransformsEquivalentTo<OTHERSCENE>(otherScene);
            CheckRenderablesEquivalentTo<OTHERSCENE>(otherScene);
            CheckStatesEquivalentTo<OTHERSCENE>(otherScene);
            CheckDataLayoutsEquivalentTo<OTHERSCENE>(otherScene);
            CheckDataInstancesEquivalentTo<OTHERSCENE>(otherScene);
            CheckTextureSamplersEquivalentTo<OTHERSCENE>(otherScene);
            CheckCamerasEquivalentTo<OTHERSCENE>(otherScene);
            CheckRenderGroupsEquivalentTo<OTHERSCENE>(otherScene);
            CheckRenderPassesEquivalentTo<OTHERSCENE>(otherScene);
            CheckBlitPassesEquivalentTo<OTHERSCENE>(otherScene);
            CheckRenderBuffersAndTargetsEquivalentTo<OTHERSCENE>(otherScene);
            CheckDataBuffersEquivalentTo<OTHERSCENE>(otherScene);
            CheckTextureBuffersEquivalentTo<OTHERSCENE>(otherScene);
            CheckDataSlotsEquivalentTo<OTHERSCENE>(otherScene);
            CheckPickableObjectsEquivalentTo<OTHERSCENE>(otherScene);
            CheckSceneReferencesEquivalentTo<OTHERSCENE>(otherScene);
        }

        template <typename OTHERSCENE>
        void CheckNodesEquivalentTo(const OTHERSCENE& otherScene) const
        {
            EXPECT_TRUE(otherScene.isNodeAllocated(parent));
            EXPECT_TRUE(otherScene.isNodeAllocated(child));
            EXPECT_TRUE(otherScene.isNodeAllocated(childChild1));
            EXPECT_TRUE(otherScene.isNodeAllocated(childChild2));
            EXPECT_TRUE(otherScene.isNodeAllocated(childChild3));

            EXPECT_FALSE(otherScene.getParent(parent).isValid());
            EXPECT_TRUE(otherScene.getParent(child).isValid());
            EXPECT_TRUE(otherScene.getParent(childChild1).isValid());
            EXPECT_TRUE(otherScene.getParent(childChild2).isValid());
            EXPECT_TRUE(otherScene.getParent(childChild3).isValid());

            EXPECT_EQ(parent, otherScene.getParent(child));
            EXPECT_EQ(child, otherScene.getParent(childChild1));
            EXPECT_EQ(child, otherScene.getParent(childChild2));
            EXPECT_EQ(child, otherScene.getParent(childChild3));
        }

        template <typename OTHERSCENE>
        void CheckTransformsEquivalentTo(const OTHERSCENE& otherScene) const
        {
            EXPECT_TRUE(otherScene.isTransformAllocated(t1));
            EXPECT_TRUE(otherScene.isTransformAllocated(t2));
            EXPECT_TRUE(otherScene.isTransformAllocated(t3));

            EXPECT_TRUE(otherScene.getTransformNode(t1).isValid());
            EXPECT_TRUE(otherScene.getTransformNode(t2).isValid());
            EXPECT_TRUE(otherScene.getTransformNode(t3).isValid());

            EXPECT_EQ(childChild1, otherScene.getTransformNode(t1));
            EXPECT_EQ(childChild2, otherScene.getTransformNode(t2));
            EXPECT_EQ(childChild3, otherScene.getTransformNode(t3));

            EXPECT_EQ(t1Translation, otherScene.getTranslation (t1));
            EXPECT_EQ(t1Scaling    , otherScene.getScaling     (t1));
            EXPECT_EQ(ERotationType::Euler_ZYZ, otherScene.getRotationType(t1));

            EXPECT_EQ(t2Translation, otherScene.getTranslation (t2));
            EXPECT_EQ(t2Scaling    , otherScene.getScaling     (t2));
            EXPECT_EQ(glm::vec4(t2Rotation, 1.f), otherScene.getRotation    (t2));
            EXPECT_EQ(ERotationType::Euler_XYX, otherScene.getRotationType(t2));

            EXPECT_EQ(t3Translation, otherScene.getTranslation (t3));
            EXPECT_EQ(t3Scaling    , otherScene.getScaling     (t3));
            EXPECT_EQ(t3Rotation, otherScene.getRotation    (t3));
            EXPECT_EQ(ERotationType::Quaternion, otherScene.getRotationType(t3));
        }

        template <typename OTHERSCENE>
        void CheckRenderablesEquivalentTo(const OTHERSCENE& otherScene) const
        {
            ASSERT_TRUE(otherScene.isRenderableAllocated(renderable));
            const Renderable& renderableData = otherScene.getRenderable(renderable);
            EXPECT_EQ(childChild2, renderableData.node);
            EXPECT_EQ(startIndex, renderableData.startIndex);
            EXPECT_EQ(indexCount, renderableData.indexCount);
            EXPECT_EQ(uniformData, renderableData.dataInstances[ERenderableDataSlotType_Uniforms]);
            EXPECT_EQ(geometryData, renderableData.dataInstances[ERenderableDataSlotType_Geometry]);
            EXPECT_EQ(renderState, renderableData.renderState);
            EXPECT_EQ(EVisibilityMode::Invisible, renderableData.visibilityMode);
            EXPECT_EQ(renderableInstanceCount, renderableData.instanceCount);
            EXPECT_EQ(startVertex, renderableData.startVertex);
        }

        template <typename OTHERSCENE>
        void CheckStatesEquivalentTo(const OTHERSCENE& otherScene) const
        {
            ASSERT_TRUE(otherScene.isRenderStateAllocated(renderState));
            const RenderState& rs = otherScene.getRenderState(renderState);

            EXPECT_EQ(EBlendFactor::One                  , rs.blendFactorSrcColor);
            EXPECT_EQ(EBlendFactor::SrcAlpha             , rs.blendFactorDstColor);
            EXPECT_EQ(EBlendFactor::OneMinusSrcAlpha     , rs.blendFactorSrcAlpha);
            EXPECT_EQ(EBlendFactor::DstAlpha             , rs.blendFactorDstAlpha);
            EXPECT_EQ(EBlendOperation::Add               , rs.blendOperationColor);
            EXPECT_EQ(EBlendOperation::Subtract          , rs.blendOperationAlpha);
            EXPECT_EQ(0.1f                               , rs.blendColor.r);
            EXPECT_EQ(0.2f                               , rs.blendColor.g);
            EXPECT_EQ(0.3f                               , rs.blendColor.b);
            EXPECT_EQ(0.4f                               , rs.blendColor.a);
            EXPECT_EQ(ECullMode::FrontFacing             , rs.cullMode);
            EXPECT_EQ(EDrawMode::Triangles               , rs.drawMode);
            EXPECT_EQ(EDepthFunc::GreaterEqual           , rs.depthFunc);
            EXPECT_EQ(EDepthWrite::Disabled              , rs.depthWrite);
            EXPECT_EQ(EScissorTest::Enabled              , rs.scissorTest);
            const RenderState::ScissorRegion scissorRegion{ 10, 20, 30u, 40u };
            EXPECT_EQ(scissorRegion                      , rs.scissorRegion);
            EXPECT_EQ(EStencilFunc::NotEqual             , rs.stencilFunc);
            EXPECT_EQ(12u                                , rs.stencilRefValue);
            EXPECT_EQ(0xAA                               , rs.stencilMask);
            EXPECT_EQ(EStencilOp::Increment              , rs.stencilOpFail);
            EXPECT_EQ(EStencilOp::IncrementWrap          , rs.stencilOpDepthFail);
            EXPECT_EQ(EStencilOp::Decrement              , rs.stencilOpDepthPass);
        }

        template <typename OTHERSCENE>
        void CheckDataLayoutsEquivalentTo(const OTHERSCENE& otherScene) const
        {
            // check counts
            EXPECT_TRUE(otherScene.isDataLayoutAllocated(uniformLayout));
            EXPECT_TRUE(otherScene.isDataLayoutAllocated(vertexLayout));
            EXPECT_EQ(9u, otherScene.getDataLayout(uniformLayout).getFieldCount());
            EXPECT_EQ(4u, otherScene.getDataLayout(vertexLayout).getFieldCount());
            // check types
            EXPECT_EQ(EDataType::Float, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(0)).dataType);
            EXPECT_EQ(EDataType::Vector4F, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(1)).dataType);
            EXPECT_EQ(EDataType::Matrix33F, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(2)).dataType);
            EXPECT_EQ(EDataType::Matrix44F, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(3)).dataType);
            EXPECT_EQ(EDataType::TextureSampler2D, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(4)).dataType);
            EXPECT_EQ(EDataType::DataReference, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(5)).dataType);
            EXPECT_EQ(EDataType::TextureSamplerExternal, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(6)).dataType);
            EXPECT_EQ(EDataType::Bool, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(7)).dataType);
            EXPECT_EQ(EDataType::Bool, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(8)).dataType);
            EXPECT_EQ(effectHash, otherScene.getDataLayout(uniformLayout).getEffectHash());
            EXPECT_EQ(EDataType::Indices, otherScene.getDataLayout(vertexLayout).getField(DataFieldHandle(0)).dataType);
            EXPECT_EQ(EDataType::Vector4Buffer, otherScene.getDataLayout(vertexLayout).getField(DataFieldHandle(1)).dataType);
            EXPECT_EQ(EDataType::FloatBuffer, otherScene.getDataLayout(vertexLayout).getField(DataFieldHandle(2)).dataType);
            EXPECT_EQ(EDataType::Vector2Buffer, otherScene.getDataLayout(vertexLayout).getField(DataFieldHandle(3)).dataType);
            EXPECT_EQ(effectHash, otherScene.getDataLayout(vertexLayout).getEffectHash());
            // check element counts
            EXPECT_EQ(1u, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(0)).elementCount);
            EXPECT_EQ(2u, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(1)).elementCount);
            EXPECT_EQ(1u, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(2)).elementCount);
            EXPECT_EQ(1u, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(3)).elementCount);
            EXPECT_EQ(1u, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(4)).elementCount);
            EXPECT_EQ(1u, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(5)).elementCount);
            EXPECT_EQ(1u, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(6)).elementCount);
            EXPECT_EQ(1u, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(7)).elementCount);
            EXPECT_EQ(1u, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(8)).elementCount);
            EXPECT_EQ(1u, otherScene.getDataLayout(vertexLayout).getField(DataFieldHandle(0)).elementCount);
            EXPECT_EQ(1u, otherScene.getDataLayout(vertexLayout).getField(DataFieldHandle(1)).elementCount);
            EXPECT_EQ(1u, otherScene.getDataLayout(vertexLayout).getField(DataFieldHandle(2)).elementCount);
            // check semantics
            EXPECT_EQ(EFixedSemantics::ModelViewMatrix33, otherScene.getDataLayout(uniformLayout).getField(DataFieldHandle(2)).semantics);
        }

        template <typename OTHERSCENE>
        void CheckDataInstancesEquivalentTo(const OTHERSCENE& otherScene) const
        {
            EXPECT_TRUE(otherScene.isDataInstanceAllocated(geometryData));
            EXPECT_TRUE(otherScene.isDataInstanceAllocated(uniformData));

            {
                const ResourceField& dataResource = otherScene.getDataResource(geometryData, DataFieldHandle(0u));
                EXPECT_EQ(indexArrayHash, dataResource.hash);
                EXPECT_FALSE(dataResource.dataBuffer.isValid());
                EXPECT_EQ(indexArrayDivisor, dataResource.instancingDivisor);
            }

            {
                const ResourceField& dataResource = otherScene.getDataResource(geometryData, DataFieldHandle(1u));
                EXPECT_EQ(vertexArrayHash, dataResource.hash);
                EXPECT_FALSE(dataResource.dataBuffer.isValid());
                EXPECT_EQ(vertexArrayDivisor, dataResource.instancingDivisor);
                EXPECT_EQ(111u, dataResource.offsetWithinElementInBytes);
                EXPECT_EQ(71u, dataResource.stride);
            }

            {
                const ResourceField& dataResource = otherScene.getDataResource(geometryData, DataFieldHandle(2u));
                EXPECT_EQ(ResourceContentHash::Invalid(), dataResource.hash);
                EXPECT_EQ(vertexDataBuffer, dataResource.dataBuffer);
                EXPECT_EQ(vertexArrayDivisor, dataResource.instancingDivisor);
                EXPECT_EQ(222u, dataResource.offsetWithinElementInBytes);
                EXPECT_EQ(72u, dataResource.stride);
            }

            {
                const ResourceField& dataResource = otherScene.getDataResource(geometryData, DataFieldHandle(3u));
                EXPECT_EQ(ResourceContentHash::Invalid(), dataResource.hash);
                EXPECT_FALSE(dataResource.dataBuffer.isValid());
                EXPECT_EQ(0u, dataResource.instancingDivisor);
            }

            EXPECT_EQ(0.5f, otherScene.getDataSingleFloat(uniformData, DataFieldHandle(0u)));
            EXPECT_EQ(true, otherScene.getDataSingleBoolean(uniformData, DataFieldHandle(7u)));
            EXPECT_EQ(false, otherScene.getDataSingleBoolean(uniformData, DataFieldHandle(8u)));

            // array check
            const glm::vec4* dataVec4fArray = otherScene.getDataVector4fArray(uniformData, DataFieldHandle(1u));
            EXPECT_EQ(glm::vec4(0.0f, 1.0f, 2.0f, 3.0f), dataVec4fArray[0]);
            EXPECT_EQ(glm::vec4(4.0f, 5.0f, 6.0f, 7.0f), dataVec4fArray[1]);

            EXPECT_EQ(glm::rotate(glm::mat3(1.f), glm::radians(5.0f)), otherScene.getDataSingleMatrix33f(uniformData, DataFieldHandle(2u)));
            EXPECT_EQ(glm::translate(glm::vec3{ 5.0f, 4.0f, 3.0f }), otherScene.getDataSingleMatrix44f(uniformData, DataFieldHandle(3u)));
            EXPECT_EQ(samplerWithTextureResource, otherScene.getDataTextureSamplerHandle(uniformData, DataFieldHandle(4u)));
            EXPECT_EQ(dataRef, otherScene.getDataReference(uniformData, DataFieldHandle(5u)));
            EXPECT_EQ(samplerWithExternalTexture, otherScene.getDataTextureSamplerHandle(uniformData, DataFieldHandle(6u)));
            EXPECT_EQ(TextureSampler::ContentType::ExternalTexture, otherScene.getTextureSampler(samplerWithExternalTexture).contentType);
        }

        template <typename OTHERSCENE>
        void CheckTextureSamplersEquivalentTo(const OTHERSCENE& otherScene) const
        {
            EXPECT_TRUE(otherScene.isTextureSamplerAllocated(samplerWithTextureResource));
            EXPECT_TRUE(otherScene.isTextureSamplerAllocated(samplerWithRenderBuffer));
            EXPECT_TRUE(otherScene.isTextureSamplerAllocated(samplerWithTextureBuffer));
            EXPECT_EQ(samplerStates.m_addressModeU, otherScene.getTextureSampler(samplerWithTextureResource).states.m_addressModeU);
            EXPECT_EQ(samplerStates.m_addressModeV, otherScene.getTextureSampler(samplerWithTextureResource).states.m_addressModeV);
            EXPECT_EQ(samplerStates.m_addressModeR, otherScene.getTextureSampler(samplerWithTextureResource).states.m_addressModeR);
            EXPECT_EQ(samplerStates.m_minSamplingMode, otherScene.getTextureSampler(samplerWithTextureResource).states.m_minSamplingMode);
            EXPECT_EQ(samplerStates.m_magSamplingMode, otherScene.getTextureSampler(samplerWithTextureResource).states.m_magSamplingMode);
            EXPECT_EQ(samplerStates.m_anisotropyLevel, otherScene.getTextureSampler(samplerWithTextureResource).states.m_anisotropyLevel);

            EXPECT_EQ(textureHash, otherScene.getTextureSampler(samplerWithTextureResource).textureResource);
            EXPECT_EQ(renderBuffer.asMemoryHandle(), otherScene.getTextureSampler(samplerWithRenderBuffer).contentHandle);
            EXPECT_EQ(texture2DBuffer.asMemoryHandle(), otherScene.getTextureSampler(samplerWithTextureBuffer).contentHandle);
            EXPECT_EQ(TextureSampler::ContentType::ClientTexture, otherScene.getTextureSampler(samplerWithTextureResource).contentType);
            EXPECT_EQ(TextureSampler::ContentType::RenderBuffer, otherScene.getTextureSampler(samplerWithRenderBuffer).contentType);
            EXPECT_EQ(TextureSampler::ContentType::TextureBuffer, otherScene.getTextureSampler(samplerWithTextureBuffer).contentType);
        }

        template <typename OTHERSCENE>
        void CheckCamerasEquivalentTo(const OTHERSCENE& otherScene) const
        {
            const Camera& camData = otherScene.getCamera(camera);
            EXPECT_EQ(ECameraProjectionType::Perspective, camData.projectionType);
            EXPECT_EQ(cameraNode, camData.node);
            EXPECT_EQ(cameraDataInstance, camData.dataInstance);

            const Camera& orthoCamData = otherScene.getCamera(orthographicCamera);
            EXPECT_EQ(ECameraProjectionType::Orthographic, orthoCamData.projectionType);
            EXPECT_EQ(cameraNode, orthoCamData.node);
        }

        template <typename OTHERSCENE>
        void CheckRenderGroupsEquivalentTo(const OTHERSCENE& otherScene) const
        {
            ASSERT_TRUE(otherScene.isRenderGroupAllocated(renderGroup));
            const RenderGroup& rg = otherScene.getRenderGroup(renderGroup);
            EXPECT_TRUE(RenderGroupUtils::ContainsRenderable(renderable, rg));
            EXPECT_FALSE(RenderGroupUtils::ContainsRenderable(renderable2, rg));
            ASSERT_EQ(1u, rg.renderables.size());
            EXPECT_EQ(renderable, rg.renderables[0].renderable);
            EXPECT_EQ(15, rg.renderables[0].order);

            ASSERT_TRUE(otherScene.isRenderGroupAllocated(nestedRenderGroupParent));
            ASSERT_TRUE(otherScene.isRenderGroupAllocated(nestedRenderGroupChild));
            const RenderGroup& rgParent = otherScene.getRenderGroup(nestedRenderGroupParent);
            const RenderGroup& rgChild = otherScene.getRenderGroup(nestedRenderGroupChild);

            EXPECT_TRUE(RenderGroupUtils::ContainsRenderGroup(nestedRenderGroupChild, rgParent));
            EXPECT_TRUE(RenderGroupUtils::ContainsRenderable(renderable2, rgChild));
            ASSERT_EQ(1u, rgChild.renderables.size());
            EXPECT_EQ(renderable2, rgChild.renderables[0].renderable);
            EXPECT_EQ(21, rgChild.renderables[0].order);
            ASSERT_EQ(1u, rgParent.renderGroups.size());
            EXPECT_EQ(nestedRenderGroupChild, rgParent.renderGroups[0].renderGroup);
            EXPECT_EQ(22, rgParent.renderGroups[0].order);
        }

        template <typename OTHERSCENE>
        void CheckRenderPassesEquivalentTo(const OTHERSCENE& otherScene) const
        {
            ASSERT_TRUE(otherScene.isRenderPassAllocated(renderPass));
            const RenderPass& rp = otherScene.getRenderPass(renderPass);
            EXPECT_EQ(camera, rp.camera);
            EXPECT_EQ(renderTarget, rp.renderTarget);
            EXPECT_EQ(1, rp.renderOrder);
            EXPECT_EQ(glm::vec4(0.5f, 0.0f, 1.f, 0.25f), rp.clearColor);
            EXPECT_EQ(EClearFlag::None, rp.clearFlags);
            EXPECT_FALSE(rp.isEnabled);
            EXPECT_TRUE(rp.isRenderOnce);

            ASSERT_TRUE(RenderGroupUtils::ContainsRenderGroup(renderGroup, rp));
            EXPECT_FALSE(RenderGroupUtils::ContainsRenderGroup(renderGroup2, rp));
            EXPECT_EQ(renderGroup, rp.renderGroups[0].renderGroup);
            EXPECT_EQ(15, rp.renderGroups[0].order);
        }

        template <typename OTHERSCENE>
        void CheckBlitPassesEquivalentTo(const OTHERSCENE& otherScene) const
        {
            ASSERT_TRUE(otherScene.isBlitPassAllocated(blitPass));
            const BlitPass& blitPassData = otherScene.getBlitPass(blitPass);
            EXPECT_EQ(renderBuffer,  blitPassData.sourceRenderBuffer);
            EXPECT_EQ(renderBuffer2, blitPassData.destinationRenderBuffer);

            EXPECT_EQ(blitPassSourceRectangle.x,      blitPassData.sourceRegion.x);
            EXPECT_EQ(blitPassSourceRectangle.y,      blitPassData.sourceRegion.y);
            EXPECT_EQ(blitPassSourceRectangle.width,  blitPassData.sourceRegion.width);
            EXPECT_EQ(blitPassSourceRectangle.height, blitPassData.sourceRegion.height);

            EXPECT_EQ(blitPassDestinationRectangle.x,      blitPassData.destinationRegion.x);
            EXPECT_EQ(blitPassDestinationRectangle.y,      blitPassData.destinationRegion.y);
            EXPECT_EQ(blitPassDestinationRectangle.width,  blitPassData.destinationRegion.width);
            EXPECT_EQ(blitPassDestinationRectangle.height, blitPassData.destinationRegion.height);
        }

        template <typename OTHERSCENE>
        void CheckRenderBuffersAndTargetsEquivalentTo(const OTHERSCENE& otherScene) const
        {
            ASSERT_TRUE(otherScene.isRenderTargetAllocated(renderTarget));
            ASSERT_TRUE(otherScene.isRenderBufferAllocated(renderBuffer));

            const RenderBuffer& renderBufferData = otherScene.getRenderBuffer(renderBuffer);
            EXPECT_EQ(23u, renderBufferData.width);
            EXPECT_EQ(42u, renderBufferData.height);
            EXPECT_EQ(EPixelStorageFormat::RGBA8, renderBufferData.format);
            EXPECT_EQ(ERenderBufferAccessMode::ReadWrite, renderBufferData.accessMode);
            EXPECT_EQ(3u, renderBufferData.sampleCount);

            EXPECT_EQ(1u, otherScene.getRenderTargetRenderBufferCount(renderTarget));
            EXPECT_EQ(renderBuffer, otherScene.getRenderTargetRenderBuffer(renderTarget, 0u));
        }

        template <typename OTHERSCENE>
        void CheckDataBuffersEquivalentTo(const OTHERSCENE& otherScene) const
        {
            ASSERT_TRUE(otherScene.isDataBufferAllocated(indexDataBuffer));
            EXPECT_TRUE(otherScene.isDataBufferAllocated(vertexDataBuffer));
            const GeometryDataBuffer& indexData = otherScene.getDataBuffer(indexDataBuffer);
            const GeometryDataBuffer& vertexData = otherScene.getDataBuffer(vertexDataBuffer);

            EXPECT_EQ(1024u, indexData.data.size());
            EXPECT_EQ(3u, indexData.usedSize);
            EXPECT_EQ(EDataBufferType::IndexBuffer, indexData.bufferType);
            EXPECT_EQ(EDataType::UInt32, indexData.dataType);
            EXPECT_EQ(std::byte{0x77}, indexData.data[1]);
            EXPECT_EQ(std::byte{0xAB}, indexData.data[2]);

            EXPECT_EQ(32u, vertexData.data.size());
            EXPECT_EQ(3u, vertexData.usedSize);
            EXPECT_EQ(EDataBufferType::VertexBuffer, vertexData.bufferType);
            EXPECT_EQ(EDataType::Float, vertexData.dataType);
            EXPECT_EQ(std::byte{0x0A}, vertexData.data[0]);
            EXPECT_EQ(std::byte{0x1B}, vertexData.data[1]);
            EXPECT_EQ(std::byte{0x2C}, vertexData.data[2]);
        }

        template <typename OTHERSCENE>
        void CheckTextureBuffersEquivalentTo(const OTHERSCENE& otherScene) const
        {
            ASSERT_TRUE(otherScene.isTextureBufferAllocated(texture2DBuffer));
            const TextureBuffer& texBuffer = otherScene.getTextureBuffer(texture2DBuffer);
            EXPECT_EQ(EPixelStorageFormat::R8, texBuffer.textureFormat);

            ASSERT_EQ(3u, texBuffer.mipMaps.size());
            EXPECT_EQ(8u, texBuffer.mipMaps[0].width);
            EXPECT_EQ(8u, texBuffer.mipMaps[0].height);
            EXPECT_EQ(4u, texBuffer.mipMaps[1].width);
            EXPECT_EQ(4u, texBuffer.mipMaps[1].height);
            EXPECT_EQ(2u, texBuffer.mipMaps[2].width);
            EXPECT_EQ(2u, texBuffer.mipMaps[2].height);
            EXPECT_EQ(84u, TextureBuffer::GetMipMapDataSizeInBytes(texBuffer));

            const auto& mipLevel0Data = texBuffer.mipMaps[0u].data;
            const auto& mipLevel2Data = texBuffer.mipMaps[2u].data;
            EXPECT_EQ(std::byte{134u}, mipLevel0Data[4 * 8 + 3]);
            EXPECT_EQ(std::byte{135u}, mipLevel0Data[5 * 8 + 3]);
            EXPECT_EQ(std::byte{36u},  mipLevel0Data[6 * 8 + 3]);

            EXPECT_EQ(std::byte{00u}, mipLevel2Data[0 * 2 + 0]);
            EXPECT_EQ(std::byte{10u}, mipLevel2Data[0 * 2 + 1]);
            EXPECT_EQ(std::byte{01u}, mipLevel2Data[1 * 2 + 0]);
            EXPECT_EQ(std::byte{11u}, mipLevel2Data[1 * 2 + 1]);

            EXPECT_EQ(Quad(3, 4, 1, 3), texBuffer.mipMaps[0].usedRegion);
            EXPECT_EQ(Quad(0, 0, 0, 0), texBuffer.mipMaps[1].usedRegion);
            EXPECT_EQ(Quad(0, 0, 2, 2), texBuffer.mipMaps[2].usedRegion);
        }

        template <typename OTHERSCENE>
        void CheckDataSlotsEquivalentTo(const OTHERSCENE& otherScene) const
        {
            EXPECT_TRUE(otherScene.isDataSlotAllocated(transformDataSlot));
            const DataSlot& dataSlot = otherScene.getDataSlot(transformDataSlot);
            EXPECT_EQ(EDataSlotType::TransformationConsumer, dataSlot.type);
            EXPECT_EQ(dataSlotId, dataSlot.id);
            EXPECT_EQ(parent, dataSlot.attachedNode);
            EXPECT_EQ(dataRef, dataSlot.attachedDataReference);
            EXPECT_EQ(textureHash, dataSlot.attachedTexture);
            EXPECT_EQ(samplerWithTextureResource, dataSlot.attachedTextureSampler);
        }

        template <typename OTHERSCENE>
        void CheckPickableObjectsEquivalentTo(const OTHERSCENE& otherScene) const
        {
            EXPECT_TRUE(otherScene.isPickableObjectAllocated(pickableHandle));
            const PickableObject& pickableObject = otherScene.getPickableObject(pickableHandle);
            EXPECT_EQ(vertexDataBuffer, pickableObject.geometryHandle);
            EXPECT_EQ(child, pickableObject.nodeHandle);
            EXPECT_EQ(camera, pickableObject.cameraHandle);
            EXPECT_EQ(pickableId, pickableObject.id);
            EXPECT_FALSE(pickableObject.isEnabled);
        }

        template <typename OTHERSCENE>
        void CheckSceneReferencesEquivalentTo(const OTHERSCENE& otherScene) const
        {
            EXPECT_TRUE(otherScene.isSceneReferenceAllocated(sceneRef));
            const auto& sr = otherScene.getSceneReference(sceneRef);
            EXPECT_EQ(sceneRefSceneId, sr.sceneId);
            EXPECT_EQ(RendererSceneState::Ready, sr.requestedState);
            EXPECT_EQ(-13, sr.renderOrder);
            EXPECT_TRUE(sr.flushNotifications);
        }

        SCENE                       scene;

        const ResourceContentHash   indexArrayHash                  {111u, 0};
        const ResourceContentHash   vertexArrayHash                 {222u, 0};
        const ResourceContentHash   effectHash                      {333u, 0};
        const ResourceContentHash   textureHash                     {444u, 0};
        const uint32_t                indexArrayDivisor               = 5u;
        const uint32_t                vertexArrayDivisor              = 6u;
        const uint32_t                startIndex                      = 12u;
        const uint32_t                indexCount                      = 13u;
        const uint32_t                startVertex                     = 14u;
        const glm::vec3               t1Translation                   {1, 2, 3};
        const glm::vec3               t1Rotation                      {4, 5, 6};
        const glm::vec3               t1Scaling                       {7,8, 9};
        const glm::vec3               t2Translation                   {0.5f, 1.5f, 2.0f};
        const glm::vec3               t2Rotation                      {1.3f, 1.6f, 2.1f};
        const glm::vec3               t2Scaling                       {2.3f, 3.6f, 4.1f};
        const glm::vec3               t3Translation                   {3.5f, 4.5f, 5.0f};
        const glm::vec4               t3Rotation                      {0.5f, -0.5f, 0.5f, 0.5f};
        const glm::vec3               t3Scaling                       {5.0f, 4.0f, 3.0f};
        const DataSlotId            dataSlotId                      {15};
        const DataInstanceHandle    dataRef                         {124u};
        // Bounding geometry data
        const glm::vec3               obbPosition                     {5.1f, 5.2f, 5.3f};
        const glm::vec3               obbExtent                       {7.1f, 7.2f, 7.3f};

        const CameraHandle           camera                         {20u};
        const CameraHandle           orthographicCamera             {24u};
        const NodeHandle             parent                         {10u};
        const NodeHandle             child                          {15u};
        const NodeHandle             childChild1                    {12u};
        const NodeHandle             childChild2                    {21u};
        const NodeHandle             childChild3                    {31u};
        const NodeHandle             cameraNode                     {23u};
        const DataInstanceHandle     cameraDataInstance             {23u};
        const TransformHandle        t1                             {35u};
        const TransformHandle        t2                             {39u};
        const TransformHandle        t3                             {41u};
        const RenderableHandle       renderable                     {43u};
        const RenderableHandle       renderable2                    {44u};
        const RenderTargetHandle     renderTarget                   {45u};
        const RenderBufferHandle     renderBuffer                   {46u};
        const RenderBufferHandle     renderBuffer2                  {47u};
        const RenderPassHandle       renderPass                     {49u};
        const BlitPassHandle         blitPass                       {50u};
        const RenderStateHandle      renderState                    {51u};
        const DataLayoutHandle       uniformLayout                  {53u};
        const DataLayoutHandle       vertexLayout                   {54u};
        const DataInstanceHandle     uniformData                    {55u};
        const DataInstanceHandle     geometryData                   {56u};
        const TextureSamplerHandle   samplerWithTextureResource     {57u};
        const TextureSamplerHandle   samplerWithRenderBuffer        {58u};
        const TextureSamplerHandle   samplerWithTextureBuffer       {60u};
        const TextureSamplerHandle   samplerWithExternalTexture     {61u};
        const DataSlotHandle         transformDataSlot              {58u};
        const RenderGroupHandle      renderGroup                    {59u};
        const RenderGroupHandle      renderGroup2                   {60u};
        const RenderGroupHandle      nestedRenderGroupParent        {61u};
        const RenderGroupHandle      nestedRenderGroupChild         {62u};
        const uint32_t                 renderableInstanceCount        = 64u;
        const TextureSamplerStates   samplerStates                  { ETextureAddressMode::Repeat, ETextureAddressMode::Clamp, ETextureAddressMode::Mirror, ETextureSamplingMethod::Nearest, ETextureSamplingMethod::Linear, 32u };
        const PixelRectangle         blitPassSourceRectangle        = PixelRectangle({1u, 2u, 300u, 400u});
        const PixelRectangle         blitPassDestinationRectangle   = PixelRectangle({5u, 6u, 700u, 800u});
        const DataBufferHandle       indexDataBuffer                { 65u };
        const DataBufferHandle       vertexDataBuffer               { 66u };
        const TextureBufferHandle    texture2DBuffer                { 67u };
        const PickableObjectHandle   pickableHandle                 { 68u };
        const PickableObjectId       pickableId                     { 69u };
        const SceneReferenceHandle   sceneRef                       { 70u };
        const SceneId                sceneRefSceneId                { 123 };
    };
}
