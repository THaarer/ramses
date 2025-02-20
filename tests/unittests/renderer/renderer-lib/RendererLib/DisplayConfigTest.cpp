//  -------------------------------------------------------------------------
//  Copyright (C) 2015 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "internal/RendererLib/DisplayConfig.h"
#include "gtest/gtest.h"

namespace ramses::internal
{
    class AInternalDisplayConfig : public ::testing::Test
    {
    public:
        ramses::internal::DisplayConfig m_config;
    };

    TEST_F(AInternalDisplayConfig, hasDefaultValues)
    {
        EXPECT_EQ(ramses::EDeviceType::GLES_3_0, m_config.getDeviceType());
        EXPECT_FALSE(m_config.getFullscreenState());
        EXPECT_EQ(1u, m_config.getAntialiasingSampleCount());
        EXPECT_EQ(1280u, m_config.getDesiredWindowWidth());
        EXPECT_EQ(480u, m_config.getDesiredWindowHeight());
        EXPECT_EQ(0, m_config.getWindowPositionX());
        EXPECT_EQ(0, m_config.getWindowPositionY());
        EXPECT_TRUE(!m_config.getWaylandIviLayerID().isValid());
        EXPECT_FALSE(m_config.getStartVisibleIvi());
        EXPECT_FALSE(m_config.isResizable());
        EXPECT_EQ(0u, m_config.getGPUMemoryCacheSize());
        EXPECT_EQ(glm::vec4(0.f,0.f,0.f,1.f), m_config.getClearColor());
        EXPECT_EQ("", m_config.getWaylandDisplay());
        EXPECT_EQ(ramses::EDepthBufferType::DepthStencil, m_config.getDepthStencilBufferType());
        EXPECT_TRUE(m_config.isAsyncEffectUploadEnabled());
        EXPECT_EQ(std::string(""), m_config.getWaylandSocketEmbedded());
        EXPECT_EQ(std::string(""), m_config.getWaylandSocketEmbeddedGroup());
        EXPECT_EQ(-1, m_config.getWaylandSocketEmbeddedFD());
        EXPECT_EQ(std::string(""), m_config.getPlatformRenderNode());
        EXPECT_EQ(-1, m_config.getSwapInterval());
        EXPECT_EQ(0, m_config.getScenePriority(ramses::internal::SceneId()));
        EXPECT_EQ(0, m_config.getScenePriority(ramses::internal::SceneId(15562)));
        EXPECT_EQ(10u, m_config.getResourceUploadBatchSize());
    }

    TEST_F(AInternalDisplayConfig, setAndGetValues)
    {
        m_config.setDeviceType(ramses::EDeviceType::GL_4_2);
        EXPECT_EQ(ramses::EDeviceType::GL_4_2, m_config.getDeviceType());

        m_config.setWindowType(ramses::EWindowType::Android);
        EXPECT_EQ(ramses::EWindowType::Android, m_config.getWindowType());

        m_config.setFullscreenState(true);
        EXPECT_TRUE(m_config.getFullscreenState());

        m_config.setAntialiasingSampleCount(2u);
        EXPECT_EQ(2u, m_config.getAntialiasingSampleCount());

        m_config.setDesiredWindowWidth(100u);
        EXPECT_EQ(100u, m_config.getDesiredWindowWidth());

        m_config.setDesiredWindowHeight(200u);
        EXPECT_EQ(200u, m_config.getDesiredWindowHeight());

        m_config.setWindowPositionX(-10);
        EXPECT_EQ(-10, m_config.getWindowPositionX());

        m_config.setWindowPositionY(10);
        EXPECT_EQ(10, m_config.getWindowPositionY());

        m_config.setWaylandIviLayerID(ramses::internal::WaylandIviLayerId(102u));
        EXPECT_EQ(102u, m_config.getWaylandIviLayerID().getValue());

        m_config.setStartVisibleIvi(true);
        EXPECT_TRUE(m_config.getStartVisibleIvi());

        m_config.setGPUMemoryCacheSize(256u);
        EXPECT_EQ(256u, m_config.getGPUMemoryCacheSize());

        m_config.setResizable(false);
        EXPECT_FALSE(m_config.isResizable());

        const glm::vec4 clearColor(0.1f, 0.2f, 0.3f, 0.4f);
        m_config.setClearColor(clearColor);
        EXPECT_EQ(clearColor, m_config.getClearColor());

        m_config.setDepthStencilBufferType(ramses::EDepthBufferType::Depth);
        EXPECT_EQ(ramses::EDepthBufferType::Depth, m_config.getDepthStencilBufferType());

        m_config.setWaylandDisplay("ramses display");
        EXPECT_EQ("ramses display", m_config.getWaylandDisplay());

        m_config.setAsyncEffectUploadEnabled(false);
        EXPECT_FALSE(m_config.isAsyncEffectUploadEnabled());

        m_config.setWaylandEmbeddedCompositingSocketName("wayland-11");
        EXPECT_EQ(std::string("wayland-11"), m_config.getWaylandSocketEmbedded());

        m_config.setWaylandEmbeddedCompositingSocketFD(42);
        EXPECT_EQ(42, m_config.getWaylandSocketEmbeddedFD());

        m_config.setWaylandEmbeddedCompositingSocketGroup("groupname1");
        EXPECT_EQ(std::string("groupname1"), m_config.getWaylandSocketEmbeddedGroup());

        m_config.setWaylandEmbeddedCompositingSocketPermissions(0654);
        EXPECT_EQ(0654u, m_config.getWaylandSocketEmbeddedPermissions());

        m_config.setPlatformRenderNode("/some/render/node");
        EXPECT_EQ(std::string("/some/render/node"), m_config.getPlatformRenderNode());

        m_config.setSwapInterval(2);
        EXPECT_EQ(2, m_config.getSwapInterval());

        m_config.setResourceUploadBatchSize(3);
        EXPECT_EQ(3u, m_config.getResourceUploadBatchSize());

        m_config.setScenePriority(ramses::internal::SceneId(15562), -1);
        EXPECT_EQ(-1, m_config.getScenePriority(ramses::internal::SceneId(15562)));
        EXPECT_EQ(0, m_config.getScenePriority(ramses::internal::SceneId(15562 + 1)));
        EXPECT_EQ(1u, m_config.getScenePriorities().size());
        EXPECT_EQ(-1, m_config.getScenePriorities().at(ramses::internal::SceneId(15562)));
    }

    TEST_F(AInternalDisplayConfig, canBeCompared)
    {
        ramses::internal::DisplayConfig config1;
        ramses::internal::DisplayConfig config2;
        EXPECT_EQ(config1, config2);

        config1.setAntialiasingSampleCount(4u);
        EXPECT_NE(config1, config2);
    }
}
