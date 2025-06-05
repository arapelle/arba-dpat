#include <arba/dpat/state/basic_state.hpp>

#include <gtest/gtest.h>

namespace ut
{

class abstract_state : public dpat::basic_state<abstract_state>
{
public:
    virtual ~abstract_state() override = default;
    virtual abstract_state_siptr run(int& dest, const int& src) = 0;
};

class sub_state;

class main_state : public abstract_state
{
public:
    virtual ~main_state() override = default;

    virtual abstract_state_siptr run(int& dest, const int& src) override
    {
        dest = src;
        return abstract_state_siptr();
    }

    virtual void invalidate() override
    {
        reset_shared_state_pointers(first_child_state, first_child_state, second_child_state);
    }

    itru::shared_intrusive_ptr<sub_state> first_child_state;
    itru::shared_intrusive_ptr<sub_state> second_child_state;
};

class sub_state : public abstract_state
{
public:
    virtual ~sub_state() override = default;

    virtual abstract_state_siptr run(int&, const int&) override { return abstract_state_siptr(); }

    virtual void invalidate() override { reset_shared_state_pointers(parent_state); }

    itru::shared_intrusive_ptr<main_state> parent_state;
};

} // namespace ut

TEST(basic_state_tests, invalidate__three_states_inter_connected__all_states_disconnected)
{
    using abstract_state_siptr = ut::abstract_state::abstract_state_siptr;
    int dest = 0;
    const int src = 42;
    itru::shared_intrusive_ptr<ut::main_state> main_state = itru::make_shared_intrusive_ptr<ut::main_state>();
    itru::shared_intrusive_ptr<ut::sub_state> first_child_state = itru::make_shared_intrusive_ptr<ut::sub_state>();
    itru::shared_intrusive_ptr<ut::sub_state> second_child_state = itru::make_shared_intrusive_ptr<ut::sub_state>();
    main_state->first_child_state = first_child_state;
    main_state->second_child_state = second_child_state;
    first_child_state->parent_state = main_state;
    second_child_state->parent_state = main_state;
    abstract_state_siptr state = main_state->run(dest, src);
    ASSERT_EQ(state, nullptr);
    ASSERT_EQ(dest, src);
    ASSERT_NE(main_state->first_child_state, nullptr);
    ASSERT_NE(main_state->second_child_state, nullptr);
    ASSERT_NE(first_child_state->parent_state, nullptr);
    ASSERT_NE(second_child_state->parent_state, nullptr);
    ASSERT_EQ(main_state->use_count(), 3);
    ASSERT_EQ(first_child_state->use_count(), 2);
    ASSERT_EQ(second_child_state->use_count(), 2);
    main_state->invalidate();
    ASSERT_EQ(main_state->first_child_state, nullptr);
    ASSERT_EQ(main_state->second_child_state, nullptr);
    ASSERT_EQ(first_child_state->parent_state, nullptr);
    ASSERT_EQ(second_child_state->parent_state, nullptr);
    ASSERT_EQ(main_state->use_count(), 1);
    ASSERT_EQ(first_child_state->use_count(), 1);
    ASSERT_EQ(second_child_state->use_count(), 1);
}
