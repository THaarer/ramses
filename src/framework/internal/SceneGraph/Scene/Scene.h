//  -------------------------------------------------------------------------
//  Copyright (C) 2012 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "internal/SceneGraph/SceneAPI/IScene.h"
#include "internal/SceneGraph/SceneAPI/Handles.h"
#include "internal/SceneGraph/SceneAPI/ResourceField.h"
#include "internal/SceneGraph/SceneAPI/Camera.h"
#include "internal/SceneGraph/SceneAPI/Renderable.h"
#include "internal/SceneGraph/SceneAPI/RenderPass.h"
#include "internal/SceneGraph/SceneAPI/RenderGroup.h"
#include "internal/SceneGraph/SceneAPI/TextureSampler.h"
#include "internal/SceneGraph/SceneAPI/TextureBuffer.h"
#include "internal/SceneGraph/SceneAPI/GeometryDataBuffer.h"
#include "internal/SceneGraph/SceneAPI/RenderBuffer.h"
#include "internal/SceneGraph/SceneAPI/RenderTarget.h"
#include "internal/SceneGraph/SceneAPI/BlitPass.h"
#include "internal/SceneGraph/SceneAPI/PickableObject.h"
#include "internal/SceneGraph/SceneAPI/SceneReference.h"

#include "internal/SceneGraph/Scene/TopologyNode.h"
#include "internal/SceneGraph/Scene/TopologyTransform.h"
#include "internal/SceneGraph/Scene/DataLayout.h"
#include "internal/SceneGraph/Scene/DataInstance.h"

#include "internal/Core/Utils/MemoryPool.h"
#include "internal/Core/Utils/MemoryPoolExplicit.h"

namespace ramses::internal
{
    template <template<typename, typename> class MEMORYPOOL>
    class SceneT;

    using Scene = SceneT<MemoryPool>;
    using SceneWithExplicitMemory = SceneT<MemoryPoolExplicit>;

    template <template<typename, typename> class MEMORYPOOL>
    class SceneT : public IScene
    {
    public:
        using RenderableMemoryPool      = MEMORYPOOL<Renderable         , RenderableHandle>;
        using NodeMemoryPool            = MEMORYPOOL<TopologyNode       , NodeHandle>;
        using CameraMemoryPool          = MEMORYPOOL<Camera             , CameraHandle>;
        using RenderStateMemoryPool     = MEMORYPOOL<RenderState        , RenderStateHandle>;
        using TransformMemoryPool       = MEMORYPOOL<TopologyTransform  , TransformHandle>;
        using DataLayoutMemoryPool      = MEMORYPOOL<DataLayout         , DataLayoutHandle>;
        using DataInstanceMemoryPool    = MEMORYPOOL<DataInstance       , DataInstanceHandle>;
        using RenderGroupMemoryPool     = MEMORYPOOL<RenderGroup        , RenderGroupHandle>;
        using RenderPassMemoryPool      = MEMORYPOOL<RenderPass         , RenderPassHandle>;
        using BlitPassMemoryPool        = MEMORYPOOL<BlitPass           , BlitPassHandle>;
        using PickableObjectMemoryPool  = MEMORYPOOL<PickableObject     , PickableObjectHandle>;
        using RenderTargetMemoryPool    = MEMORYPOOL<RenderTarget       , RenderTargetHandle>;
        using RenderBufferMemoryPool    = MEMORYPOOL<RenderBuffer       , RenderBufferHandle>;
        using TextureSamplerMemoryPool  = MEMORYPOOL<TextureSampler     , TextureSamplerHandle>;
        using DataBufferMemoryPool      = MEMORYPOOL<GeometryDataBuffer , DataBufferHandle>;
        using TextureBufferMemoryPool   = MEMORYPOOL<TextureBuffer      , TextureBufferHandle>;
        using DataSlotMemoryPool        = MEMORYPOOL<DataSlot           , DataSlotHandle>;
        using SceneReferenceMemoryPool  = MEMORYPOOL<SceneReference     , SceneReferenceHandle>;

        explicit SceneT(const SceneInfo& sceneInfo = SceneInfo());

        void preallocateSceneSize (const SceneSizeInformation& sizeInfo) override;

        [[nodiscard]] SceneId            getSceneId                 () const final override;
        [[nodiscard]] const std::string& getName                    () const final override;

        void setEffectTimeSync(FlushTime::Clock::time_point t) override;
        [[nodiscard]] FlushTime::Clock::time_point getEffectTimeSync() const override;

        // Renderables
        RenderableHandle            allocateRenderable              (NodeHandle nodeHandle, RenderableHandle handle) override;
        void                        releaseRenderable               (RenderableHandle renderableHandle) override;
        [[nodiscard]] bool          isRenderableAllocated           (RenderableHandle renderableHandle) const final override;
        [[nodiscard]] uint32_t      getRenderableCount              () const final override;
        void                        setRenderableDataInstance       (RenderableHandle renderableHandle, ERenderableDataSlotType slot, DataInstanceHandle newDataInstance) override;
        void                        setRenderableStartIndex         (RenderableHandle renderableHandle, uint32_t startIndex) override;
        void                        setRenderableIndexCount         (RenderableHandle renderableHandle, uint32_t indexCount) override;
        void                        setRenderableRenderState        (RenderableHandle renderableHandle, RenderStateHandle stateHandle) override;
        void                        setRenderableVisibility         (RenderableHandle renderableHandle, EVisibilityMode visibility) override;
        void                        setRenderableInstanceCount      (RenderableHandle renderableHandle, uint32_t instanceCount) override;
        void                        setRenderableStartVertex        (RenderableHandle renderableHandle, uint32_t startVertex) override;
        [[nodiscard]] const Renderable& getRenderable               (RenderableHandle renderableHandle) const final override;
        [[nodiscard]] const RenderableMemoryPool& getRenderables    () const;

        // Render state
        RenderStateHandle           allocateRenderState             (RenderStateHandle stateHandle) override;
        void                        releaseRenderState              (RenderStateHandle stateHandle) override;
        [[nodiscard]] bool          isRenderStateAllocated          (RenderStateHandle stateHandle) const final override;
        [[nodiscard]] uint32_t      getRenderStateCount             () const final override;
        void                        setRenderStateBlendFactors      (RenderStateHandle stateHandle, EBlendFactor srcColor, EBlendFactor destColor, EBlendFactor srcAlpha, EBlendFactor destAlpha) override;
        void                        setRenderStateBlendOperations   (RenderStateHandle stateHandle, EBlendOperation operationColor, EBlendOperation operationAlpha) override;
        void                        setRenderStateBlendColor        (RenderStateHandle stateHandle, const glm::vec4& color) override;
        void                        setRenderStateCullMode          (RenderStateHandle stateHandle, ECullMode cullMode) override;
        void                        setRenderStateDrawMode          (RenderStateHandle stateHandle, EDrawMode drawMode) override;
        void                        setRenderStateDepthFunc         (RenderStateHandle stateHandle, EDepthFunc func) override;
        void                        setRenderStateDepthWrite        (RenderStateHandle stateHandle, EDepthWrite flag) override;
        void                        setRenderStateScissorTest       (RenderStateHandle stateHandle, EScissorTest flag, const RenderState::ScissorRegion& region) override;
        void                        setRenderStateStencilFunc       (RenderStateHandle stateHandle, EStencilFunc func, uint8_t ref, uint8_t mask) override;
        void                        setRenderStateStencilOps        (RenderStateHandle stateHandle, EStencilOp sfail, EStencilOp dpfail, EStencilOp dppass) override;
        void                        setRenderStateColorWriteMask    (RenderStateHandle stateHandle, ColorWriteMask colorMask) override;
        [[nodiscard]] const RenderState& getRenderState             (RenderStateHandle stateHandle) const final override;
        [[nodiscard]] const RenderStateMemoryPool& getRenderStates  () const;

        // Camera
        CameraHandle                allocateCamera                  (ECameraProjectionType type, NodeHandle nodeHandle, DataInstanceHandle dataInstance, CameraHandle handle) override;
        void                        releaseCamera                   (CameraHandle cameraHandle) override;
        [[nodiscard]] bool          isCameraAllocated               (CameraHandle handle) const final override;
        [[nodiscard]] uint32_t      getCameraCount                  () const final override;
        [[nodiscard]] const Camera& getCamera                       (CameraHandle cameraHandle) const final override;
        [[nodiscard]] const CameraMemoryPool& getCameras            () const;

        // Nodes
        NodeHandle                  allocateNode                    (uint32_t childrenCount, NodeHandle handle) override;
        void                        releaseNode                     (NodeHandle nodeHandle) override;
        [[nodiscard]] bool          isNodeAllocated                 (NodeHandle node) const final override;
        [[nodiscard]] uint32_t      getNodeCount                    () const final override;
        [[nodiscard]] NodeHandle    getParent                       (NodeHandle nodeHandle) const final override;
        void                        addChildToNode                  (NodeHandle parent, NodeHandle child) override;
        void                        removeChildFromNode             (NodeHandle parent, NodeHandle child) override;
        [[nodiscard]] uint32_t      getChildCount                   (NodeHandle parent) const final override;
        [[nodiscard]] NodeHandle    getChild                        (NodeHandle parent, uint32_t childNumber) const final override;
        [[nodiscard]] const NodeMemoryPool& getNodes                () const;

        // Transformation
        TransformHandle             allocateTransform               (NodeHandle nodeHandle, TransformHandle handle) override;
        void                        releaseTransform                (TransformHandle transform) override;
        [[nodiscard]] uint32_t      getTransformCount               () const final override;
        [[nodiscard]] bool          isTransformAllocated            (TransformHandle transformHandle) const final override;
        [[nodiscard]] NodeHandle    getTransformNode                (TransformHandle handle) const final override;
        [[nodiscard]] const glm::vec3& getTranslation               (TransformHandle handle) const final override;
        [[nodiscard]] const glm::vec4& getRotation                  (TransformHandle handle) const final override;
        [[nodiscard]] ERotationType    getRotationType              (TransformHandle handle) const final override;
        [[nodiscard]] const glm::vec3& getScaling                   (TransformHandle handle) const final override;
        void                        setTranslation                  (TransformHandle handle, const glm::vec3& translation) override;
        void                        setRotation                     (TransformHandle handle, const glm::vec4& rotation, ERotationType rotationType) override;
        void                        setScaling                      (TransformHandle handle, const glm::vec3& scaling) override;
        [[nodiscard]] const TransformMemoryPool& getTransforms      () const;

        DataLayoutHandle            allocateDataLayout              (const DataFieldInfoVector& dataFields, const ResourceContentHash& effectHash, DataLayoutHandle handle) override;
        void                        releaseDataLayout               (DataLayoutHandle layoutHandle) override;
        [[nodiscard]] bool          isDataLayoutAllocated           (DataLayoutHandle layoutHandle) const final override;
        [[nodiscard]] uint32_t      getDataLayoutCount              () const final override;
        [[nodiscard]] const DataLayout& getDataLayout               (DataLayoutHandle layoutHandle) const final override;
        [[nodiscard]] const DataLayoutMemoryPool& getDataLayouts    () const;

        DataInstanceHandle          allocateDataInstance            (DataLayoutHandle finishedLayoutHandle, DataInstanceHandle instanceHandle) override;
        void                        releaseDataInstance             (DataInstanceHandle containerHandle) override;
        [[nodiscard]] bool          isDataInstanceAllocated         (DataInstanceHandle containerHandle) const final override;
        [[nodiscard]] uint32_t      getDataInstanceCount            () const final override;
        [[nodiscard]] DataLayoutHandle getLayoutOfDataInstance      (DataInstanceHandle containerHandle) const final override;
        [[nodiscard]] const DataInstanceMemoryPool& getDataInstances() const;

        [[nodiscard]] const float*         getDataFloatArray            (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::vec2*     getDataVector2fArray         (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::vec3*     getDataVector3fArray         (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::vec4*     getDataVector4fArray         (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const bool*          getDataBooleanArray          (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const int32_t*       getDataIntegerArray          (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::mat2*     getDataMatrix22fArray        (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::mat3*     getDataMatrix33fArray        (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::mat4*     getDataMatrix44fArray        (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::ivec2*    getDataVector2iArray         (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::ivec4*    getDataVector4iArray         (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::ivec3*    getDataVector3iArray         (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const ResourceField& getDataResource              (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] TextureSamplerHandle getDataTextureSamplerHandle  (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] DataInstanceHandle   getDataReference             (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;

        void setDataFloatArray           (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const float* data) override;
        void setDataVector2fArray        (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const glm::vec2* data) override;
        void setDataVector3fArray        (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const glm::vec3* data) override;
        void setDataVector4fArray        (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const glm::vec4* data) override;
        void setDataBooleanArray         (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const bool* data) override;
        void setDataIntegerArray         (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const int32_t* data) override;
        void setDataVector2iArray        (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const glm::ivec2* data) override;
        void setDataVector3iArray        (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const glm::ivec3* data) override;
        void setDataVector4iArray        (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const glm::ivec4* data) override;
        void setDataMatrix22fArray       (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const glm::mat2* data) override;
        void setDataMatrix33fArray       (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const glm::mat3* data) override;
        void setDataMatrix44fArray       (DataInstanceHandle containerHandle, DataFieldHandle field, uint32_t elementCount, const glm::mat4* data) override;
        void setDataResource             (DataInstanceHandle containerHandle, DataFieldHandle field, const ResourceContentHash& hash, DataBufferHandle dataBuffer, uint32_t instancingDivisor, uint16_t offsetWithinElementInBytes, uint16_t stride) override;
        void setDataTextureSamplerHandle (DataInstanceHandle containerHandle, DataFieldHandle field, TextureSamplerHandle samplerHandle) override;
        void setDataReference            (DataInstanceHandle containerHandle, DataFieldHandle field, DataInstanceHandle dataRef) override;

        // get/setData*Array wrappers for elementCount == 1
        [[nodiscard]] float             getDataSingleFloat              (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::vec2&  getDataSingleVector2f           (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::vec3&  getDataSingleVector3f           (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::vec4&  getDataSingleVector4f           (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] bool              getDataSingleBoolean            (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] int32_t           getDataSingleInteger            (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::mat2&  getDataSingleMatrix22f          (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::mat3&  getDataSingleMatrix33f          (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::mat4&  getDataSingleMatrix44f          (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::ivec2& getDataSingleVector2i           (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::ivec3& getDataSingleVector3i           (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;
        [[nodiscard]] const glm::ivec4& getDataSingleVector4i           (DataInstanceHandle containerHandle, DataFieldHandle field) const final override;

        void setDataSingleFloat     (DataInstanceHandle containerHandle, DataFieldHandle field, float data) override;
        void setDataSingleVector2f  (DataInstanceHandle containerHandle, DataFieldHandle field, const glm::vec2& data) override;
        void setDataSingleVector3f  (DataInstanceHandle containerHandle, DataFieldHandle field, const glm::vec3& data) override;
        void setDataSingleVector4f  (DataInstanceHandle containerHandle, DataFieldHandle field, const glm::vec4& data) override;
        void setDataSingleBoolean   (DataInstanceHandle containerHandle, DataFieldHandle field, bool data) override;
        void setDataSingleInteger   (DataInstanceHandle containerHandle, DataFieldHandle field, int32_t data) override;
        void setDataSingleVector2i  (DataInstanceHandle containerHandle, DataFieldHandle field, const glm::ivec2& data) override;
        void setDataSingleVector3i  (DataInstanceHandle containerHandle, DataFieldHandle field, const glm::ivec3& data) override;
        void setDataSingleVector4i  (DataInstanceHandle containerHandle, DataFieldHandle field, const glm::ivec4& data) override;
        void setDataSingleMatrix22f (DataInstanceHandle containerHandle, DataFieldHandle field, const glm::mat2& data) override;
        void setDataSingleMatrix33f (DataInstanceHandle containerHandle, DataFieldHandle field, const glm::mat3& data) override;
        void setDataSingleMatrix44f (DataInstanceHandle containerHandle, DataFieldHandle field, const glm::mat4& data) override;

        // Texture sampler
        TextureSamplerHandle        allocateTextureSampler          (const TextureSampler& sampler, TextureSamplerHandle handle) override;
        void                        releaseTextureSampler           (TextureSamplerHandle handle) override;
        [[nodiscard]] bool          isTextureSamplerAllocated       (TextureSamplerHandle handle) const final override;
        [[nodiscard]] uint32_t      getTextureSamplerCount          () const final override;
        [[nodiscard]] const TextureSampler& getTextureSampler       (TextureSamplerHandle handle) const final override;
        [[nodiscard]] const TextureSamplerMemoryPool& getTextureSamplers() const;

        // Render groups
        RenderGroupHandle       allocateRenderGroup             (uint32_t renderableCount, uint32_t nestedGroupCount, RenderGroupHandle groupHandle) override;
        void                    releaseRenderGroup              (RenderGroupHandle groupHandle) override;
        [[nodiscard]] bool      isRenderGroupAllocated          (RenderGroupHandle groupHandle) const final override;
        [[nodiscard]] uint32_t  getRenderGroupCount             () const final override;
        void                    addRenderableToRenderGroup      (RenderGroupHandle groupHandle, RenderableHandle renderableHandle, int32_t order) override;
        void                    removeRenderableFromRenderGroup (RenderGroupHandle groupHandle, RenderableHandle renderableHandle) override;
        void                    addRenderGroupToRenderGroup     (RenderGroupHandle groupHandleParent, RenderGroupHandle groupHandleChild, int32_t order) override;
        void                    removeRenderGroupFromRenderGroup(RenderGroupHandle groupHandleParent, RenderGroupHandle groupHandleChild) override;
        [[nodiscard]] const RenderGroup& getRenderGroup         (RenderGroupHandle groupHandle) const final override;
        [[nodiscard]] const RenderGroupMemoryPool& getRenderGroups() const;

        //Render pass
        RenderPassHandle        allocateRenderPass              (uint32_t renderGroupCount, RenderPassHandle passHandle) override;
        void                    releaseRenderPass               (RenderPassHandle passHandle) override;
        [[nodiscard]] bool      isRenderPassAllocated           (RenderPassHandle pass) const final override;
        [[nodiscard]] uint32_t  getRenderPassCount              () const final override;
        void                    setRenderPassClearColor         (RenderPassHandle passHandle, const glm::vec4& clearColor) override;
        void                    setRenderPassClearFlag          (RenderPassHandle passHandle, ClearFlags clearFlag) override;
        void                    setRenderPassCamera             (RenderPassHandle passHandle, CameraHandle cameraHandle) override;
        void                    setRenderPassRenderTarget       (RenderPassHandle passHandle, RenderTargetHandle targetHandle) override;
        void                    setRenderPassRenderOrder        (RenderPassHandle passHandle, int32_t renderOrder) override;
        void                    setRenderPassEnabled            (RenderPassHandle passHandle, bool isEnabled) override;
        void                    setRenderPassRenderOnce         (RenderPassHandle passHandle, bool enable) override;
        void                    retriggerRenderPassRenderOnce   (RenderPassHandle passHandle) override;
        void                    addRenderGroupToRenderPass      (RenderPassHandle passHandle, RenderGroupHandle groupHandle, int32_t order) override;
        void                    removeRenderGroupFromRenderPass (RenderPassHandle passHandle, RenderGroupHandle groupHandle) override;
        [[nodiscard]] const RenderPass& getRenderPass           (RenderPassHandle passHandle) const final override;
        [[nodiscard]] const RenderPassMemoryPool& getRenderPasses() const;

        //Blit pass
        BlitPassHandle          allocateBlitPass                (RenderBufferHandle sourceRenderBufferHandle, RenderBufferHandle destinationRenderBufferHandle, BlitPassHandle passHandle) override;
        void                    releaseBlitPass                 (BlitPassHandle passHandle) override;
        [[nodiscard]] bool      isBlitPassAllocated             (BlitPassHandle passHandle) const final override;
        [[nodiscard]] uint32_t  getBlitPassCount                () const final override;
        void                    setBlitPassRenderOrder          (BlitPassHandle passHandle, int32_t renderOrder) override;
        void                    setBlitPassEnabled              (BlitPassHandle passHandle, bool isEnabled) override;
        void                    setBlitPassRegions              (BlitPassHandle passHandle, const PixelRectangle& sourceRegion, const PixelRectangle& destinationRegion) override;
        [[nodiscard]] const BlitPass& getBlitPass               (BlitPassHandle passHandle) const final override;
        [[nodiscard]] const BlitPassMemoryPool& getBlitPasses   () const;

        //Pickable object
        PickableObjectHandle    allocatePickableObject          (DataBufferHandle geometryHandle, NodeHandle nodeHandle, PickableObjectId id, PickableObjectHandle pickableHandle) override;
        void                    releasePickableObject           (PickableObjectHandle pickableHandle) override;
        [[nodiscard]] bool      isPickableObjectAllocated       (PickableObjectHandle pickableHandle) const final override;
        [[nodiscard]] uint32_t  getPickableObjectCount          () const final override;
        void                    setPickableObjectId             (PickableObjectHandle pickableHandle, PickableObjectId id) override;
        void                    setPickableObjectCamera         (PickableObjectHandle pickableHandle, CameraHandle cameraHandle) override;
        void                    setPickableObjectEnabled        (PickableObjectHandle pickableHandle, bool isEnabled) override;
        [[nodiscard]] const PickableObject& getPickableObject   (PickableObjectHandle pickableHandle) const final override;
        [[nodiscard]] const PickableObjectMemoryPool& getPickableObjects() const;

        // Render targets
        RenderTargetHandle      allocateRenderTarget            (RenderTargetHandle targetHandle) override;
        void                    releaseRenderTarget             (RenderTargetHandle targetHandle) override;
        [[nodiscard]] bool      isRenderTargetAllocated         (RenderTargetHandle targetHandle) const final override;
        [[nodiscard]] uint32_t  getRenderTargetCount            () const final override;
        void                    addRenderTargetRenderBuffer     (RenderTargetHandle targetHandle, RenderBufferHandle bufferHandle) override;
        [[nodiscard]] uint32_t  getRenderTargetRenderBufferCount(RenderTargetHandle targetHandle) const final override;
        [[nodiscard]] RenderBufferHandle getRenderTargetRenderBuffer(RenderTargetHandle targetHandle, uint32_t bufferIndex) const final override;
        [[nodiscard]] const RenderTargetMemoryPool& getRenderTargets() const;

        // Render buffers
        RenderBufferHandle      allocateRenderBuffer            (const RenderBuffer& renderBuffer, RenderBufferHandle handle) override;
        void                    releaseRenderBuffer             (RenderBufferHandle handle) override;
        void                    setRenderBufferProperties       (RenderBufferHandle handle, uint32_t width, uint32_t height, uint32_t sampleCount) override;
        [[nodiscard]] bool      isRenderBufferAllocated         (RenderBufferHandle handle) const final override;
        [[nodiscard]] uint32_t  getRenderBufferCount            () const final override;
        [[nodiscard]] const RenderBuffer& getRenderBuffer       (RenderBufferHandle handle) const final override;
        [[nodiscard]] const RenderBufferMemoryPool& getRenderBuffers() const;

        // Data buffers
        DataBufferHandle        allocateDataBuffer              (EDataBufferType dataBufferType, EDataType dataType, uint32_t maximumSizeInBytes, DataBufferHandle handle) override;
        void                    releaseDataBuffer               (DataBufferHandle handle) override;
        [[nodiscard]] uint32_t  getDataBufferCount              () const final override;
        void                    updateDataBuffer                (DataBufferHandle handle, uint32_t offsetInBytes, uint32_t dataSizeInBytes, const std::byte* data) override;
        [[nodiscard]] bool      isDataBufferAllocated           (DataBufferHandle handle) const final override;
        [[nodiscard]] const GeometryDataBuffer& getDataBuffer   (DataBufferHandle handle) const final override;
        [[nodiscard]] const DataBufferMemoryPool& getDataBuffers() const;

        //Texture buffers
        TextureBufferHandle     allocateTextureBuffer           (EPixelStorageFormat textureFormat, const MipMapDimensions& mipMapDimensions, TextureBufferHandle handle) override;
        void                    releaseTextureBuffer            (TextureBufferHandle handle) override;
        [[nodiscard]] bool      isTextureBufferAllocated        (TextureBufferHandle handle) const final override;
        [[nodiscard]] uint32_t  getTextureBufferCount           () const final override;
        void                    updateTextureBuffer             (TextureBufferHandle handle, uint32_t mipLevel, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const std::byte* data) override;
        [[nodiscard]] const TextureBuffer& getTextureBuffer     (TextureBufferHandle handle) const final override;
        [[nodiscard]] const TextureBufferMemoryPool& getTextureBuffers() const;

        DataSlotHandle          allocateDataSlot                (const DataSlot& dataSlot, DataSlotHandle handle) override;
        void                    releaseDataSlot                 (DataSlotHandle handle) override;
        void                    setDataSlotTexture              (DataSlotHandle handle, const ResourceContentHash& texture) override;
        [[nodiscard]] bool      isDataSlotAllocated             (DataSlotHandle handle) const final override;
        [[nodiscard]] uint32_t  getDataSlotCount                () const final override;
        [[nodiscard]] const DataSlot& getDataSlot               (DataSlotHandle handle) const final override;
        [[nodiscard]] const DataSlotMemoryPool& getDataSlots    () const;

        SceneReferenceHandle    allocateSceneReference          (SceneId sceneId, SceneReferenceHandle handle) override;
        void                    releaseSceneReference           (SceneReferenceHandle handle) override;
        void                    requestSceneReferenceState      (SceneReferenceHandle handle, RendererSceneState state) override;
        void                    requestSceneReferenceFlushNotifications(SceneReferenceHandle handle, bool enable) override;
        void                    setSceneReferenceRenderOrder    (SceneReferenceHandle handle, int32_t renderOrder) override;
        [[nodiscard]] bool      isSceneReferenceAllocated       (SceneReferenceHandle handle) const final override;
        [[nodiscard]] uint32_t  getSceneReferenceCount          () const final override;
        [[nodiscard]] const SceneReference& getSceneReference   (SceneReferenceHandle handle) const final override;
        [[nodiscard]] const SceneReferenceMemoryPool& getSceneReferences() const;

        [[nodiscard]] SceneSizeInformation getSceneSizeInformation() const final  override;

    protected:
        [[nodiscard]] const TopologyNode& getNode                         (NodeHandle handle) const;
        TextureSampler&                   getTextureSamplerInternal       (TextureSamplerHandle handle);
        RenderPass&                       getRenderPassInternal           (RenderPassHandle handle);
        RenderGroup&                      getRenderGroupInternal          (RenderGroupHandle handle);
        [[nodiscard]] const TopologyTransform& getTransform               (TransformHandle handle) const;

    private:
        template <typename TYPE>
        const TYPE* getInstanceDataInternal(DataInstanceHandle dataInstanceHandle, DataFieldHandle fieldId) const;
        template <typename TYPE>
        void setInstanceDataInternal(DataInstanceHandle dataInstanceHandle, DataFieldHandle fieldId, uint32_t elementCount, const TYPE* newValue);

        NodeMemoryPool              m_nodes;
        CameraMemoryPool            m_cameras;
        RenderableMemoryPool        m_renderables;
        RenderStateMemoryPool       m_states;
        TransformMemoryPool         m_transforms;
        DataLayoutMemoryPool        m_dataLayoutMemory;
        DataInstanceMemoryPool      m_dataInstanceMemory;
        RenderGroupMemoryPool       m_renderGroups;
        RenderPassMemoryPool        m_renderPasses;
        BlitPassMemoryPool          m_blitPasses;
        PickableObjectMemoryPool    m_pickableObjects;
        RenderTargetMemoryPool      m_renderTargets;
        RenderBufferMemoryPool      m_renderBuffers;
        TextureSamplerMemoryPool    m_textureSamplers;
        DataBufferMemoryPool        m_dataBuffers;
        TextureBufferMemoryPool     m_textureBuffers;
        DataSlotMemoryPool          m_dataSlots;
        SceneReferenceMemoryPool    m_sceneReferences;

        const std::string           m_name;
        const SceneId               m_sceneId;

        FlushTime::Clock::time_point m_effectTimeSync;
    };

    template <template<typename, typename> class MEMORYPOOL>
    RenderPass& SceneT<MEMORYPOOL>::getRenderPassInternal(RenderPassHandle handle)
    {
        return *m_renderPasses.getMemory(handle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    RenderGroup& SceneT<MEMORYPOOL>::getRenderGroupInternal(RenderGroupHandle handle)
    {
        return *m_renderGroups.getMemory(handle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    template <typename TYPE>
    [[nodiscard]] const TYPE* SceneT<MEMORYPOOL>::getInstanceDataInternal(DataInstanceHandle dataInstanceHandle, DataFieldHandle fieldId) const
    {
        const DataInstance* dataInstance = m_dataInstanceMemory.getMemory(dataInstanceHandle);
        const DataLayout* dataLayout = m_dataLayoutMemory.getMemory(dataInstance->getLayoutHandle());
        assert(TypeMatchesEDataType<TYPE>(dataLayout->getField(fieldId).dataType));
        return dataInstance->getTypedDataPointer<TYPE>(dataLayout->getFieldOffset(fieldId));
    }

    template <template<typename, typename> class MEMORYPOOL>
    template <typename TYPE>
    void SceneT<MEMORYPOOL>::setInstanceDataInternal(DataInstanceHandle dataInstanceHandle, DataFieldHandle fieldId, uint32_t elementCount, const TYPE* newValue)
    {
        DataInstance* dataInstance = m_dataInstanceMemory.getMemory(dataInstanceHandle);
        const DataLayout* dataLayout = m_dataLayoutMemory.getMemory(dataInstance->getLayoutHandle());
        assert(TypeMatchesEDataType<TYPE>(dataLayout->getField(fieldId).dataType));
        assert(elementCount == dataLayout->getField(fieldId).elementCount);
        dataInstance->setTypedData<TYPE>(dataLayout->getFieldOffset(fieldId), elementCount, newValue);
    }

    // SceneT::is*Allocated calls are inlined because heavily used. Allow devirtualization and inlining together with final
    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isCameraAllocated(CameraHandle handle) const
    {
        return m_cameras.isAllocated(handle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isRenderPassAllocated(RenderPassHandle pass) const
    {
        return m_renderPasses.isAllocated(pass);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isRenderTargetAllocated(RenderTargetHandle targetHandle) const
    {
        return m_renderTargets.isAllocated(targetHandle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isRenderBufferAllocated(RenderBufferHandle handle) const
    {
        return m_renderBuffers.isAllocated(handle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isDataBufferAllocated(DataBufferHandle handle) const
    {
        return m_dataBuffers.isAllocated(handle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isTextureBufferAllocated(TextureBufferHandle handle) const
    {
        return m_textureBuffers.isAllocated(handle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isDataSlotAllocated(DataSlotHandle handle) const
    {
        return m_dataSlots.isAllocated(handle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isBlitPassAllocated(BlitPassHandle passHandle) const
    {
        return m_blitPasses.isAllocated(passHandle);
    }

    template <template <typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isPickableObjectAllocated(PickableObjectHandle pickableHandle) const
    {
        return m_pickableObjects.isAllocated(pickableHandle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isRenderStateAllocated(RenderStateHandle stateHandle) const
    {
        return m_states.isAllocated(stateHandle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isTextureSamplerAllocated(TextureSamplerHandle handle) const
    {
        return m_textureSamplers.isAllocated(handle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isRenderGroupAllocated(RenderGroupHandle groupHandle) const
    {
        return m_renderGroups.isAllocated(groupHandle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isDataInstanceAllocated(DataInstanceHandle containerHandle) const
    {
        return m_dataInstanceMemory.isAllocated(containerHandle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isDataLayoutAllocated(DataLayoutHandle layoutHandle) const
    {
        return m_dataLayoutMemory.isAllocated(layoutHandle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isRenderableAllocated(RenderableHandle renderableHandle) const
    {
        return m_renderables.isAllocated(renderableHandle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isTransformAllocated(TransformHandle transformHandle) const
    {
        return m_transforms.isAllocated(transformHandle);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline bool SceneT<MEMORYPOOL>::isNodeAllocated(NodeHandle node) const
    {
        return m_nodes.isAllocated(node);
    }

    template <template<typename, typename> class MEMORYPOOL>
    bool SceneT<MEMORYPOOL>::isSceneReferenceAllocated(SceneReferenceHandle handle) const
    {
        return m_sceneReferences.isAllocated(handle);
    }

    // inline often called getters
    template <template<typename, typename> class MEMORYPOOL>
    inline SceneId SceneT<MEMORYPOOL>::getSceneId() const
    {
        return m_sceneId;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline FlushTime::Clock::time_point SceneT<MEMORYPOOL>::getEffectTimeSync() const
    {
        return m_effectTimeSync;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const DataLayout& SceneT<MEMORYPOOL>::getDataLayout(DataLayoutHandle layoutHandle) const
    {
        return *m_dataLayoutMemory.getMemory(layoutHandle);
    }


    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::RenderableMemoryPool& SceneT<MEMORYPOOL>::getRenderables() const
    {
        return m_renderables;
    }


    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::RenderStateMemoryPool& SceneT<MEMORYPOOL>::getRenderStates() const
    {
        return m_states;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::CameraMemoryPool& SceneT<MEMORYPOOL>::getCameras() const
    {
        return m_cameras;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::NodeMemoryPool& SceneT<MEMORYPOOL>::getNodes() const
    {
        return m_nodes;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::TransformMemoryPool& SceneT<MEMORYPOOL>::getTransforms() const
    {
        return m_transforms;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::DataLayoutMemoryPool& SceneT<MEMORYPOOL>::getDataLayouts() const
    {
        return m_dataLayoutMemory;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::DataInstanceMemoryPool& SceneT<MEMORYPOOL>::getDataInstances() const
    {
        return m_dataInstanceMemory;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::TextureSamplerMemoryPool& SceneT<MEMORYPOOL>::getTextureSamplers() const
    {
        return m_textureSamplers;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::RenderGroupMemoryPool& SceneT<MEMORYPOOL>::getRenderGroups() const
    {
        return m_renderGroups;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::RenderPassMemoryPool& SceneT<MEMORYPOOL>::getRenderPasses() const
    {
        return m_renderPasses;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::BlitPassMemoryPool& SceneT<MEMORYPOOL>::getBlitPasses() const
    {
        return m_blitPasses;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::PickableObjectMemoryPool& SceneT<MEMORYPOOL>::getPickableObjects() const
    {
        return m_pickableObjects;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::RenderTargetMemoryPool& SceneT<MEMORYPOOL>::getRenderTargets() const
    {
        return m_renderTargets;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::RenderBufferMemoryPool& SceneT<MEMORYPOOL>::getRenderBuffers() const
    {
        return m_renderBuffers;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::DataBufferMemoryPool& SceneT<MEMORYPOOL>::getDataBuffers() const
    {
        return m_dataBuffers;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::TextureBufferMemoryPool& SceneT<MEMORYPOOL>::getTextureBuffers() const
    {
        return m_textureBuffers;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::DataSlotMemoryPool& SceneT<MEMORYPOOL>::getDataSlots() const
    {
        return m_dataSlots;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline
    const typename SceneT<MEMORYPOOL>::SceneReferenceMemoryPool& SceneT<MEMORYPOOL>::getSceneReferences() const
    {
        return m_sceneReferences;
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const float* SceneT<MEMORYPOOL>::getDataFloatArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<float>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const glm::mat2* SceneT<MEMORYPOOL>::getDataMatrix22fArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<glm::mat2>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const glm::mat3* SceneT<MEMORYPOOL>::getDataMatrix33fArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<glm::mat3>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const glm::mat4* SceneT<MEMORYPOOL>::getDataMatrix44fArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<glm::mat4>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const glm::vec2* SceneT<MEMORYPOOL>::getDataVector2fArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<glm::vec2>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const glm::vec3* SceneT<MEMORYPOOL>::getDataVector3fArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<glm::vec3>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const glm::vec4* SceneT<MEMORYPOOL>::getDataVector4fArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<glm::vec4>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const bool* SceneT<MEMORYPOOL>::getDataBooleanArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<bool>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const int32_t* SceneT<MEMORYPOOL>::getDataIntegerArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<int32_t>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const glm::ivec2* SceneT<MEMORYPOOL>::getDataVector2iArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<glm::ivec2>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const glm::ivec3* SceneT<MEMORYPOOL>::getDataVector3iArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<glm::ivec3>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const glm::ivec4* SceneT<MEMORYPOOL>::getDataVector4iArray(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return getInstanceDataInternal<glm::ivec4>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline TextureSamplerHandle SceneT<MEMORYPOOL>::getDataTextureSamplerHandle(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return *getInstanceDataInternal<TextureSamplerHandle>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline DataInstanceHandle SceneT<MEMORYPOOL>::getDataReference(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return *getInstanceDataInternal<DataInstanceHandle>(containerHandle, fieldId);
    }

    template <template<typename, typename> class MEMORYPOOL>
    inline const ResourceField& SceneT<MEMORYPOOL>::getDataResource(DataInstanceHandle containerHandle, DataFieldHandle fieldId) const
    {
        return *getInstanceDataInternal<ResourceField>(containerHandle, fieldId);
    }
}
