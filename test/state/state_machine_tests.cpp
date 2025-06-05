#include <arba/dpat/state/state.hpp>
#include <arba/dpat/state/state_machine.hpp>

#include <gtest/gtest.h>

#include <functional>

namespace ut
{

class state_machine;

class abstract_state : public dpat::state<abstract_state, state_machine, int&, int&, const int&, int>
{
public:
    virtual ~abstract_state() override = default;

    virtual void manage_event(state_machine& st_machine, char pressed_char) = 0;
};

class state_machine : public dpat::state_machine<state_machine, abstract_state>
{
    using base_ = dpat::state_machine<state_machine, abstract_state>;

public:
    using base_::base_;

    inline void manage_event(char pressed_char) { this->base_::invoke(&abstract_state::manage_event, *this, pressed_char); }
};

//---

class second_state;

class first_state : public abstract_state
{
public:
    first_state(bool& valid, bool& a_pressed);

    virtual ~first_state() override { *pvalid_ = false; }

    virtual int& execute(state_machine& st_machine, int& dest, const int& src, int offset) override
    {
        offset += offset;
        dest = src + offset;
        st_machine.set_state(nullptr);
        return dest;
    }

    virtual void manage_event(state_machine& , char pressed_char) override
    {
        if (pressed_char == 'A')
            *p_a_pressed_ = true;
    }

    virtual void invalidate() override { reset_shared_state_pointers(next_state_); }

private:
    bool* pvalid_ = nullptr;
    bool* p_a_pressed_ = nullptr;
    itru::shared_intrusive_ptr<second_state> next_state_;
};

class second_state : public abstract_state
{
public:
    second_state(itru::shared_intrusive_ptr<first_state> state_siptr = nullptr)
    {
        previous_state_ = std::move(state_siptr);
    }

    virtual ~second_state() override = default;

    virtual int& execute(state_machine& st_machine, int& dest, const int&, int) override
    {
        st_machine.set_state(nullptr);
        return dest;
    }

    virtual void manage_event(state_machine& , char ) override
    {
    }

    virtual void invalidate() override { reset_shared_state_pointers(previous_state_); }

private:
    itru::shared_intrusive_ptr<first_state> previous_state_;
};

first_state::first_state(bool& valid, bool& a_pressed)
    : pvalid_(&valid), p_a_pressed_(&a_pressed),
    next_state_(itru::make_shared_intrusive_ptr<second_state>(itru::shared_intrusive_ptr<first_state>(this)))
{
}

} // namespace ut

TEST(state_machine_tests, use_case_ok)
{
    using abstract_state_siptr = ut::abstract_state::abstract_state_siptr;

    bool valid = true;
    bool a_pressed = false;
    int dest = 0;
    int src = 42;
    int offset = 1000;

    abstract_state_siptr gen_int =
        itru::make_shared_intrusive_ptr<ut::first_state>(std::ref(valid), std::ref(a_pressed));
    ut::state_machine stm(std::move(gen_int));

    ASSERT_TRUE(valid);
    ASSERT_FALSE(a_pressed);
    while (stm.has_state())
    {
        stm.manage_event('A');
        stm.execute(dest, src, offset);
    }

    ASSERT_TRUE(a_pressed);
    ASSERT_EQ(dest, 2042);
    ASSERT_EQ(src, 42);
    ASSERT_EQ(offset, 1000);
    ASSERT_FALSE(valid);
}
