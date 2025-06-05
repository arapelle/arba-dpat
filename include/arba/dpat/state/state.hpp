#pragma once

#include "basic_state.hpp"

#include <type_traits>

inline namespace arba
{
namespace dpat
{

template <class AbstractStateType, typename... Args>
class state : public basic_state<AbstractStateType>
{
public:
    using state_type = state;

public:
    using typename basic_state<AbstractStateType>::abstract_state_siptr;

public:
    virtual ~state() = default;
    virtual abstract_state_siptr execute(Args... args) = 0;
};

} // namespace dpat
} // namespace arba
