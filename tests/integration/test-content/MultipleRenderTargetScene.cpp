//  -------------------------------------------------------------------------
//  Copyright (C) 2015 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "TestScenes/MultipleRenderTargetScene.h"
#include "ramses/client/Scene.h"
#include "ramses/client/Effect.h"
#include "ramses/client/PerspectiveCamera.h"
#include "ramses/client/RenderTarget.h"
#include "ramses/client/RenderGroup.h"
#include "ramses/client/RenderPass.h"
#include "ramses/client/Geometry.h"
#include "ramses/client/AttributeInput.h"
#include "ramses/client/MeshNode.h"
#include "ramses/client/Appearance.h"
#include "ramses/client/RenderTargetDescription.h"
#include <cassert>

namespace ramses::internal
{
    MultipleRenderTargetScene::MultipleRenderTargetScene(ramses::Scene& scene, uint32_t state, const glm::vec3& cameraPosition, uint32_t vpWidth, uint32_t vpHeight)
        : CommonRenderBufferTestScene(scene, cameraPosition, vpWidth, vpHeight)
        , m_renderBuffer1(*scene.createRenderBuffer(16u, 16u, ramses::ERenderBufferFormat::RGBA8, ramses::ERenderBufferAccessMode::ReadWrite))
        , m_renderBuffer2(InitRenderBuffer(scene, state))
        , m_depthBuffer(*scene.createRenderBuffer(16u, 16u, ramses::ERenderBufferFormat::Depth32, ramses::ERenderBufferAccessMode::ReadWrite))
    {
        initClearPass();

        if (state != CLEAR_MRT)
        {
            initMRTPass(state);
        }

        initFinalRenderPass(state);
    }

    const ramses::Effect& MultipleRenderTargetScene::getMRTEffect(uint32_t state)
    {
        switch (state)
        {
        case TWO_COLOR_BUFFERS:
        case TWO_COLOR_BUFFERS_RGBA8_AND_RGBA4:
        case SHADER_WRITES_TWO_COLOR_BUFFERS_RT_HAS_ONE:
            return getEffectRenderTwoBuffers();
        case SHADER_WRITES_ONE_COLOR_BUFFER_RT_HAS_TWO:
        case COLOR_WRITTEN_BY_TWO_DIFFERENT_RTS:
        case DEPTH_WRITTEN_AND_USED_BY_DIFFERENT_RT:
        case DEPTH_WRITTEN_AND_READ:
            return getEffectRenderOneBuffer();
        default:
            assert(false);
            return *getTestEffect("dummy");
        }
    }

    ramses::RenderBuffer& MultipleRenderTargetScene::InitRenderBuffer(ramses::Scene& scene, uint32_t state)
    {
        if (state == TWO_COLOR_BUFFERS_RGBA8_AND_RGBA4)
        {
            return *scene.createRenderBuffer(16u, 16u, ramses::ERenderBufferFormat::RGBA4, ramses::ERenderBufferAccessMode::ReadWrite);
        }
        return *scene.createRenderBuffer(16u, 16u, ramses::ERenderBufferFormat::RGBA8, ramses::ERenderBufferAccessMode::ReadWrite);
    }

    ramses::RenderTarget& MultipleRenderTargetScene::createMRTRenderTarget(uint32_t state)
    {
        ramses::RenderTargetDescription rtDesc;

        switch (state)
        {
        case TWO_COLOR_BUFFERS:
        case TWO_COLOR_BUFFERS_RGBA8_AND_RGBA4:
        case SHADER_WRITES_ONE_COLOR_BUFFER_RT_HAS_TWO:
            rtDesc.addRenderBuffer(m_renderBuffer1);
            rtDesc.addRenderBuffer(m_renderBuffer2);
            break;
        case SHADER_WRITES_TWO_COLOR_BUFFERS_RT_HAS_ONE:
        case COLOR_WRITTEN_BY_TWO_DIFFERENT_RTS:
            rtDesc.addRenderBuffer(m_renderBuffer1);
            break;
        case DEPTH_WRITTEN_AND_USED_BY_DIFFERENT_RT:
            rtDesc.addRenderBuffer(m_renderBuffer1);
            rtDesc.addRenderBuffer(m_depthBuffer);
            break;
        case DEPTH_WRITTEN_AND_READ:
            rtDesc.addRenderBuffer(m_depthBuffer);
            break;
        default:
            assert(false);
            break;
        }

        return *m_scene.createRenderTarget(rtDesc);
    }

    const ramses::MeshNode& MultipleRenderTargetScene::createQuadWithTexture(const ramses::RenderBuffer& renderBuffer, const glm::vec3& translation, const glm::vec4& modulateColor)
    {
        const ramses::Effect* effect = getTestEffect("ramses-test-client-texturedWithColor");

        const uint16_t indicesArray[] = { 0, 1, 2, 2, 1, 3 };
        const ramses::ArrayResource* indices = m_scene.createArrayResource(6u, indicesArray);

        const std::array<ramses::vec3f, 4u> vertexPositionsArray
        {
            ramses::vec3f{ -0.5f, -0.5f, 0.f },
            ramses::vec3f{ 0.5f, -0.5f, 0.f },
            ramses::vec3f{ -0.5f, 0.5f, 0.f },
            ramses::vec3f{ 0.5f, 0.5f, 0.f }
        };
        const ramses::ArrayResource* vertexPositions = m_scene.createArrayResource(4u, vertexPositionsArray.data());

        const std::array<ramses::vec2f, 4u> textureCoordsArray{ ramses::vec2f{0.f, 0.f}, ramses::vec2f{2.f, 0.f}, ramses::vec2f{0.f, 2.f}, ramses::vec2f{2.f, 2.f} };
        const ramses::ArrayResource* textureCoords = m_scene.createArrayResource(4u, textureCoordsArray.data());

        ramses::Appearance* appearance = m_scene.createAppearance(*effect, "appearance");
        appearance->setInputValue(*effect->findUniformInput("u_color"), modulateColor);

        // set vertex positions directly in geometry
        ramses::Geometry* geometry = m_scene.createGeometry(*effect, "triangle geometry");
        geometry->setIndices(*indices);
        geometry->setInputBuffer(*effect->findAttributeInput("a_position"), *vertexPositions);
        geometry->setInputBuffer(*effect->findAttributeInput("a_texcoord"), *textureCoords);

        ramses::TextureSampler* sampler = m_scene.createTextureSampler(
            ramses::ETextureAddressMode::Repeat,
            ramses::ETextureAddressMode::Repeat,
            ramses::ETextureSamplingMethod::Nearest,
            ramses::ETextureSamplingMethod::Nearest,
            renderBuffer);

        appearance->setInputTexture(*effect->findUniformInput("u_texture"), *sampler);

        ramses::MeshNode* meshNode = m_scene.createMeshNode("quad");
        meshNode->setAppearance(*appearance);
        meshNode->setGeometry(*geometry);

        ramses::Node* transNode = m_scene.createNode();
        transNode->setTranslation({translation.x, translation.y, translation.z});
        meshNode->setParent(*transNode);

        return *meshNode;
    }

    void MultipleRenderTargetScene::initClearPass()
    {
        ramses::RenderPass* renderPass = m_scene.createRenderPass();
        renderPass->setRenderOrder(-100);
        renderPass->setCamera(createCamera());

        ramses::RenderTargetDescription rtDesc;
        rtDesc.addRenderBuffer(m_renderBuffer1);
        rtDesc.addRenderBuffer(m_renderBuffer2);
        rtDesc.addRenderBuffer(m_depthBuffer);
        ramses::RenderTarget& renderTarget = *m_scene.createRenderTarget(rtDesc);

        renderPass->setRenderTarget(&renderTarget);
        renderPass->setClearColor({1.f, 0.f, 1.f, 0.5f});
        renderPass->setClearFlags(ramses::EClearFlag::All);
    }

    void MultipleRenderTargetScene::initMRTPass(uint32_t state)
    {
        ramses::MeshNode& meshNode = createMesh(getMRTEffect(state));

        ramses::Node& transNode = *m_scene.createNode();
        transNode.addChild(meshNode);
        transNode.translate({0.0f, -0.5f, -5.0f});
        if (state == DEPTH_WRITTEN_AND_READ)
        {
            transNode.setRotation({-30.0f, 0.f, 0.f}, ramses::ERotationType::Euler_XYZ);
        }

        ramses::RenderGroup& renderGroup = *m_scene.createRenderGroup();
        renderGroup.addMeshNode(meshNode);

        ramses::RenderPass& renderPass = *m_scene.createRenderPass();
        renderPass.setRenderOrder(0);
        renderPass.addRenderGroup(renderGroup);

        ramses::PerspectiveCamera& camera = createCamera(5.8f, 6.5f);
        renderPass.setCamera(camera);

        ramses::RenderTarget& renderTarget = createMRTRenderTarget(state);
        renderPass.setRenderTarget(&renderTarget);
        renderPass.setClearFlags(ramses::EClearFlag::None);

        if (state == COLOR_WRITTEN_BY_TWO_DIFFERENT_RTS ||
            state == DEPTH_WRITTEN_AND_USED_BY_DIFFERENT_RT)
        {
            camera.setViewport(0u, 0u, 8u, 16u);
            camera.setFrustum(camera.getLeftPlane() * 0.5f, camera.getRightPlane() * 0.5f, camera.getBottomPlane(), camera.getTopPlane(), camera.getNearPlane(), camera.getFarPlane());

            ramses::MeshNode& meshNode2 = createMesh(getMRTEffect(state), TriangleAppearance::EColor_Blue);
            transNode.addChild(meshNode2);

            meshNode2.getAppearance()->setDepthFunction(ramses::EDepthFunc::NotEqual);

            ramses::RenderGroup& renderGroup2 = *m_scene.createRenderGroup();
            renderGroup2.addMeshNode(meshNode2);

            ramses::RenderPass& renderPass2 = *m_scene.createRenderPass();
            renderPass2.setRenderOrder(1);
            renderPass2.addRenderGroup(renderGroup2);

            ramses::PerspectiveCamera& camera2 = createCamera(5.8f, 6.5f);
            camera2.setViewport(6u, 0u, 8u, 16u);
            camera2.setFrustum(camera2.getLeftPlane() * 0.5f, camera2.getRightPlane() * 0.5f, camera2.getBottomPlane(), camera2.getTopPlane(), camera2.getNearPlane(), camera2.getFarPlane());
            renderPass2.setCamera(camera2);

            ramses::RenderTarget& renderTarget2 = createMRTRenderTarget(state);
            renderPass2.setRenderTarget(&renderTarget2);
            renderPass2.setClearFlags(ramses::EClearFlag::None);
        }
    }

    void MultipleRenderTargetScene::initFinalRenderPass(uint32_t state)
    {
        const ramses::MeshNode* quad1 = nullptr;
        const ramses::MeshNode* quad2 = nullptr;

        if (state != DEPTH_WRITTEN_AND_READ)
        {
            quad1 = &createQuadWithTexture(m_renderBuffer1, glm::vec3(-0.6f, 0.f, -8.f));
            quad2 = &createQuadWithTexture(m_renderBuffer2, glm::vec3(0.6f, 0.f, -8.f));
        }
        else
        {
            // Modulate depth sampled as color with red color only, so that it can be tested consistently on different platforms.
            // Some platforms give the depth value in all RGBA channels, some only in R channel.
            quad1 = &createQuadWithTexture(m_depthBuffer, glm::vec3(-0.6f, 0.f, -8.f), glm::vec4(1.f, 0.f, 0.f, 1.f));
            quad2 = &createQuadWithTexture(m_depthBuffer, glm::vec3(0.6f, 0.f, -8.f), glm::vec4(1.f, 0.f, 0.f, 1.f));
        }

        ramses::Camera& camera = createCameraWithDefaultParameters();
        camera.setParent(getDefaultCameraTranslationNode());
        ramses::RenderPass* renderPass = m_scene.createRenderPass();
        renderPass->setRenderOrder(100);
        renderPass->setCamera(camera);
        ramses::RenderGroup* renderGroup = m_scene.createRenderGroup();
        renderPass->addRenderGroup(*renderGroup);
        renderGroup->addMeshNode(*quad1);
        renderGroup->addMeshNode(*quad2);
    }
}
