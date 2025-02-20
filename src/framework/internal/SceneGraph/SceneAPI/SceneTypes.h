//  -------------------------------------------------------------------------
//  Copyright (C) 2012 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "ramses/framework/StronglyTypedValue.h"
#include "internal/SceneGraph/SceneAPI/Handles.h"
#include "internal/SceneGraph/SceneAPI/RenderState.h"
#include <vector>

namespace ramses::internal
{
    struct RenderGroupOrderEntry
    {
        RenderGroupHandle renderGroup;
        int32_t           order{0};

        bool operator<(const RenderGroupOrderEntry& other) const
        {
            return order < other.order;
        }
    };

    using RenderPassVector           =  std::vector<RenderPassHandle>;
    using RenderableVector           =  std::vector<RenderableHandle>;
    using RenderPassVector           =  std::vector<RenderPassHandle>;
    using NodeHandleVector           =  std::vector<NodeHandle>;
    using TransformHandleVector      =  std::vector<TransformHandle>;
    using RenderBufferHandleVector   =  std::vector<RenderBufferHandle>;
    using RenderGroupOrderVector     =  std::vector<RenderGroupOrderEntry>;
    using BlitPassHandleVector       =  std::vector<BlitPassHandle>;
    using PickableObjectHandleVector =  std::vector<PickableObjectHandle>;
    using DataBufferHandleVector     =  std::vector<DataBufferHandle>;
    using TextureBufferHandleVector  =  std::vector<TextureBufferHandle>;
    using TextureSamplerHandleVector =  std::vector<TextureSamplerHandle>;

    struct PickableObjectIdTag {};
    using PickableObjectId = StronglyTypedValue<uint32_t, std::numeric_limits<uint32_t>::max(), PickableObjectIdTag>;
    using PickableObjectIds = std::vector<PickableObjectId>;
}
