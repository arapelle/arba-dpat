#pragma once

#include "basic_state_machine.hpp"
#include "state.hpp"

inline namespace arba
{
namespace dpat
{

template <class AbstractStateType>
class state_machine : public basic_state_machine<AbstractStateType>
{
    using base_ = basic_state_machine<AbstractStateType>;

public:
    using typename base_::abstract_state_siptr;

    explicit state_machine(abstract_state_siptr state_siptr = abstract_state_siptr()) : base_(std::move(state_siptr)) {}

    template <typename... Args>
    inline void execute(Args&&... args)
    {
        using state_t = typename AbstractStateType::state_type;
        this->base_::invoke(&state_t::execute, std::forward<Args>(args)...);
    }
};

} // namespace dpat
} // namespace arba
