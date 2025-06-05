#pragma once

#include "basic_state_machine.hpp"

#include <arba/meta/type_traits/crtp.hpp>

inline namespace arba
{
namespace dpat
{

template <class SelfType, class AbstractStateType>
class state_machine : public meta::crtp_base<SelfType>, public basic_state_machine<AbstractStateType>
{
    using base_ = basic_state_machine<AbstractStateType>;

public:
    using typename base_::abstract_state_siptr;

    explicit state_machine(abstract_state_siptr state_siptr = abstract_state_siptr()) : base_(std::move(state_siptr)) {}

    template <typename... Args>
    inline decltype(auto) execute(Args&&... args)
    {
        using state_type = typename AbstractStateType::state_type;
        return this->base_::invoke(&state_type::execute, static_cast<SelfType&>(*this), std::forward<Args>(args)...);
    }
};

} // namespace dpat
} // namespace arba
