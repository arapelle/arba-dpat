#pragma once

#include <cassert>
#include <utility>

inline namespace arba
{
namespace dpat
{

template <class AbstractStateType>
class basic_state_machine
{
public:
    using abstract_state = AbstractStateType;
    using abstract_state_siptr = typename AbstractStateType::abstract_state_siptr;

    explicit basic_state_machine(abstract_state_siptr state_siptr = abstract_state_siptr())
        : current_state_(state_siptr)
    {
    }

    basic_state_machine(const basic_state_machine&) = delete;
    basic_state_machine(basic_state_machine&&) = default;
    basic_state_machine& operator=(const basic_state_machine&) = delete;
    basic_state_machine& operator=(basic_state_machine&&) = default;

    ~basic_state_machine()
    {
        if (current_state_)
            invalidate_and_reset_state_();
    }

    void reset(abstract_state_siptr state_siptr = abstract_state_siptr())
    {
        if (current_state_)
            invalidate_and_reset_state_();
        current_state_ = state_siptr;
    }

    [[nodiscard]] inline bool has_state() const { return current_state_; }

    [[nodiscard]] inline const abstract_state_siptr& state() const { return current_state_; }

    void set_state(abstract_state_siptr state_siptr) { current_state_ = std::move(state_siptr); }

protected:
    template <typename CallbackType, typename... Args>
        requires requires(AbstractStateType* ptr, CallbackType callback, Args&&... args) {
            { (ptr->*callback)(std::forward<Args>(args)...) };
        } && (!std::is_void_v<std::invoke_result_t<CallbackType, AbstractStateType, Args && ...>>)
    decltype(auto) invoke(CallbackType callback, Args&&... args)
    {
        assert(has_state());
        abstract_state_siptr executing_state_siptr = current_state_;
        decltype(auto) result = (executing_state_siptr.get()->*callback)(std::forward<Args>(args)...);
        if (!current_state_) [[likely]]
            executing_state_siptr->invalidate();
        return result;
    }

    template <typename CallbackType, typename... Args>
        requires requires(AbstractStateType* ptr, CallbackType callback, Args&&... args) {
            { (ptr->*callback)(std::forward<Args>(args)...) } -> std::same_as<void>;
        }
    void invoke(CallbackType callback, Args&&... args)
    {
        assert(has_state());
        abstract_state_siptr executing_state_siptr = current_state_;
        (executing_state_siptr.get()->*callback)(std::forward<Args>(args)...);
        if (!current_state_) [[likely]]
            executing_state_siptr->invalidate();
    }

private:
    inline void invalidate_and_reset_state_()
    {
        current_state_->invalidate();
        current_state_.reset();
    }

private:
    abstract_state_siptr current_state_;
};

} // namespace dpat
} // namespace arba
