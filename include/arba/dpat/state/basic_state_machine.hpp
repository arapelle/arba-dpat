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

    [[nodiscard]] inline abstract_state_siptr state() const { return current_state_; }

protected:
    template <typename CallbackType, typename... Args>
        requires requires(AbstractStateType* ptr, CallbackType callback, Args&&... args) {
            { (ptr->*callback)(std::forward<Args>(args)...) } -> std::convertible_to<abstract_state_siptr>;
        }
    void invoke(CallbackType callback, Args&&... args)
    {
        assert(has_state());

        abstract_state_siptr current_state = (current_state_.get()->*callback)(std::forward<Args>(args)...);
        if (current_state) [[likely]]
            current_state_ = std::move(current_state);
        else
            invalidate_and_reset_state_();
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
