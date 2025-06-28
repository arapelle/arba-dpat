#pragma once

#include "basic_state_machine.hpp"

#include <arba/meta/type_traits/crtp.hpp>

inline namespace arba
{
namespace dpat
{

template <class AbstractStateType, class SelfType = meta::default_t>
class state_machine : public meta::crtp_base<meta::type_or_default_t<SelfType, state_machine<AbstractStateType>>>,
                      public basic_state_machine<AbstractStateType>
{
    using base_ = basic_state_machine<AbstractStateType>;

public:
    template <class OtherSelf>
    using rebind_t = state_machine<AbstractStateType, OtherSelf>;

public:
    using typename base_::abstract_state_siptr;

    explicit state_machine(abstract_state_siptr state_siptr = abstract_state_siptr()) : base_(std::move(state_siptr)) {}

    template <typename... Args>
    inline decltype(auto) execute(Args&&... args)
    {
        using state_type = typename AbstractStateType::state_type;
        return this->base_::invoke(&state_type::execute, this->self(), std::forward<Args>(args)...);
    }
};

} // namespace dpat
} // namespace arba
