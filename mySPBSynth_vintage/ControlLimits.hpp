#ifndef CONTROLLIMITS_HPP_
#define CONTROLLIMITS_HPP_

#include <array>
#include "ControlPorts.hpp"

constexpr std::array<std::pair<float, float>, CONTROL_NR> controlLimits =
{{
    {0.0f, 4.0f},
    {0.001f, 4.0f},
    {0.001f, 4.0f},
    {0.0f, 1.0f},
    {0.001f, 4.0f},
    {0.0f, 1.0f}
}};

#endif
