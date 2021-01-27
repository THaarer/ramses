//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef RAMSES_COMPONENT_CATEGORYINFO_H
#define RAMSES_COMPONENT_CATEGORYINFO_H

#include "Utils/LoggingUtils.h"
#include "Utils/StringOutputSpecialWrapper.h"
#include "ramses-framework-api/CategoryInfoUpdate.h"
#include "absl/types/span.h"
#include <vector>

namespace ramses_internal
{
    class CategoryInfo
    {
    public:
        CategoryInfo();
        CategoryInfo(uint32_t width, uint32_t height);
        explicit CategoryInfo(absl::Span<const Byte> data);

        void setCategoryRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        uint32_t getCategoryX() const;
        uint32_t getCategoryY() const;
        uint32_t getCategoryWidth() const;
        uint32_t getCategoryHeight() const;
        bool hasCategoryRectChange() const;

        void setRenderSize(uint32_t width, uint32_t height);
        uint32_t getRenderSizeWidth() const;
        uint32_t getRenderSizeHeight() const;
        bool hasRenderSizeChange() const;

        void setSafeRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        uint32_t getSafeRectX() const;
        uint32_t getSafeRectY() const;
        uint32_t getSafeRectWidth() const;
        uint32_t getSafeRectHeight() const;
        bool hasSafeRectChange() const;

        std::vector<Byte> toBinary() const;

        bool operator==(const CategoryInfo& rhs) const;
        bool operator!=(const CategoryInfo& rhs) const;
    private:
        void fromBinary(absl::Span<const Byte> data);

        uint32_t m_categoryRectX;
        uint32_t m_categoryRectY;
        uint32_t m_categoryRectWidth;
        uint32_t m_categoryRectHeight;
        uint32_t m_renderSizeWidth;
        uint32_t m_renderSizeHeight;
        uint32_t m_safeRectX;
        uint32_t m_safeRectY;
        uint32_t m_safeRectWidth;
        uint32_t m_safeRectHeight;
        bool m_categoryRectChanged;
        bool m_renderSizeChanged;
        bool m_safeRectChanged;
    };
}

template <>
struct fmt::formatter<ramses_internal::CategoryInfo> : public ramses_internal::SimpleFormatterBase
{
    template <typename FormatContext>
    auto format(const ramses_internal::CategoryInfo& categoryInfo, FormatContext& ctx)
    {
        fmt::format_to(ctx.out(), "[categoryRect:");
        if (categoryInfo.hasCategoryRectChange())
            fmt::format_to(ctx.out(), "{}x{}", categoryInfo.getCategoryWidth(), categoryInfo.getCategoryHeight());
        return fmt::format_to(ctx.out(), "]");
    }
};

#endif
