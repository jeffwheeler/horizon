#pragma once
#include "core.hpp"
#include <forward_list>

namespace horizon {

class ToolCopyPlacement : public ToolBase {
public:
    ToolCopyPlacement(Core *c, ToolID tid);
    ToolResponse begin(const ToolArgs &args) override;
    ToolResponse update(const ToolArgs &args) override;
    bool can_begin() override;
    bool is_specific() override
    {
        return true;
    }

private:
};
} // namespace horizon
