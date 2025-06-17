#pragma once

#include <arba/itru/intrusive_ref_counter.hpp>
#include <arba/itru/shared_intrusive_ptr.hpp>
#include <arba/meta/type_traits/crtp.hpp>

#include <type_traits>

inline namespace arba
{
namespace dpat
{

template <class AbstractStateType>
class basic_state : public meta::crtp_base<AbstractStateType>, public itru::intrusive_ref_counter<>
{
public:
    using abstract_state_siptr = itru::shared_intrusive_ptr<AbstractStateType>;

    virtual ~basic_state() = default;
    virtual void invalidate() = 0;

protected:
    template <class StateType>
        requires std::is_base_of_v<AbstractStateType, StateType>
    void reset_shared_state_pointer(itru::shared_intrusive_ptr<StateType>& state_siptr)
    {
        if (state_siptr) [[likely]]
        {
            itru::shared_intrusive_ptr<StateType> aux = std::move(state_siptr);
            aux->invalidate();
        }
    }

    template <class StateType, class... States>
    void reset_shared_state_pointers(itru::shared_intrusive_ptr<StateType>& state_siptr, States&&... states)
    {
        reset_shared_state_pointer(state_siptr);
        if constexpr (sizeof...(states) >= 1)
        {
            reset_shared_state_pointers(std::forward<States>(states)...);
        }
    }
};

} // namespace dpat
} // namespace arba
