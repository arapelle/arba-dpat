#pragma once

#include "basic_state.hpp"

#include <type_traits>

inline namespace arba
{
namespace dpat
{

template <class SelfType, class StateMachineType, typename ReturnType, typename... Args>
class state : public basic_state<SelfType>
{
public:
    using state_type = state;
    using state_machine = StateMachineType;

public:
    virtual ~state() = default;
    virtual ReturnType execute(state_machine& st_machine, Args... args) = 0;
};

} // namespace dpat
} // namespace arba
