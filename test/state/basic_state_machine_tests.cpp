#include <arba/dpat/state/basic_state.hpp>
#include <arba/dpat/state/basic_state_machine.hpp>

#include <gtest/gtest.h>

#include <functional>

namespace ut
{

class state_machine;

class abstract_state : public dpat::basic_state<abstract_state>
{
public:
    virtual ~abstract_state() override = default;
    virtual int& run(state_machine& st_machine, int& dest, const int& src, int offset) = 0;
};

class state_machine : public dpat::basic_state_machine<abstract_state>
{
    using base_ = dpat::basic_state_machine<abstract_state>;

public:
    using base_::base_;

    inline int& run(int& dest, const int& src, int offset)
    {
        return this->base_::invoke(&abstract_state::run, *this, dest, src, offset);
    }

private:
};

//---

class second_state;

class first_state : public abstract_state
{
public:
    first_state(bool& valid);

    virtual ~first_state() override { *pvalid_ = false; }

    virtual int& run(state_machine& st_machine, int& dest, const int& src, int offset) override
    {
        offset += offset;
        dest = src + offset;
        st_machine.set_state(nullptr);
        return dest;
    }

    virtual void invalidate() override { reset_shared_state_pointers(next_state_); }

private:
    bool* pvalid_ = nullptr;
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

    virtual int& run(state_machine& st_machine, int& dest, const int&, int) override
    {
        st_machine.set_state(nullptr);
        return dest;
    }

    virtual void invalidate() override { reset_shared_state_pointers(previous_state_); }

private:
    itru::shared_intrusive_ptr<first_state> previous_state_;
};

first_state::first_state(bool& valid)
    : pvalid_(&valid),
      next_state_(itru::make_shared_intrusive_ptr<second_state>(itru::shared_intrusive_ptr<first_state>(this)))
{
}

} // namespace ut

TEST(basic_state_machine_tests, constructor__null_siptr__ok)
{
    ut::state_machine stm;
    ASSERT_FALSE(stm.has_state());
    ASSERT_EQ(stm.state(), nullptr);
}

TEST(basic_state_machine_tests, constructor__not_null_siptr__ok)
{
    using abstract_state_siptr = ut::abstract_state::abstract_state_siptr;

    bool valid = true;
    abstract_state_siptr state = itru::make_shared_intrusive_ptr<ut::first_state>(std::ref(valid));
    ut::state_machine stm(state);
    ASSERT_TRUE(stm.has_state());
    ASSERT_EQ(stm.state(), state);
    ASSERT_EQ(state->use_count(), 3);
}

TEST(basic_state_machine_tests, reset__null_siptr_and_set_null_siptr__ok)
{
    using abstract_state_siptr = ut::abstract_state::abstract_state_siptr;

    bool valid = true;
    abstract_state_siptr state = itru::make_shared_intrusive_ptr<ut::first_state>(std::ref(valid));
    ut::state_machine stm;
    stm.reset();
    ASSERT_FALSE(stm.has_state());
    ASSERT_EQ(stm.state(), nullptr);
}

TEST(basic_state_machine_tests, reset__null_siptr_and_set_not_null_siptr__ok)
{
    using abstract_state_siptr = ut::abstract_state::abstract_state_siptr;

    bool valid = true;
    abstract_state_siptr state = itru::make_shared_intrusive_ptr<ut::first_state>(std::ref(valid));
    ut::state_machine stm;
    stm.reset(state);
    ASSERT_TRUE(stm.has_state());
    ASSERT_EQ(stm.state(), state);
    ASSERT_EQ(state->use_count(), 3);
}

TEST(basic_state_machine_tests, reset__not_null_siptr_and_set_null_siptr__ok)
{
    using abstract_state_siptr = ut::abstract_state::abstract_state_siptr;

    bool valid = true;
    abstract_state_siptr state = itru::make_shared_intrusive_ptr<ut::first_state>(std::ref(valid));
    ut::state_machine stm(state);
    stm.reset();
    ASSERT_FALSE(stm.has_state());
    ASSERT_EQ(stm.state(), nullptr);
    ASSERT_TRUE(valid);
    ASSERT_EQ(state->use_count(), 1);
}

TEST(basic_state_machine_tests, reset__not_null_siptr_and_set_not_null_siptr__ok)
{
    using abstract_state_siptr = ut::abstract_state::abstract_state_siptr;

    bool valid = true;
    bool other_valid = true;
    abstract_state_siptr state = itru::make_shared_intrusive_ptr<ut::first_state>(std::ref(valid));
    abstract_state_siptr other_state = itru::make_shared_intrusive_ptr<ut::first_state>(std::ref(other_valid));
    ut::state_machine stm(state);
    stm.reset(other_state);
    ASSERT_TRUE(stm.has_state());
    ASSERT_EQ(stm.state(), other_state);
    ASSERT_TRUE(valid);
    ASSERT_EQ(state->use_count(), 1);
    ASSERT_EQ(other_state->use_count(), 3);
}

TEST(basic_state_machine_tests, destructor__state_invalidated)
{
    using abstract_state_siptr = ut::abstract_state::abstract_state_siptr;

    bool valid = true;
    abstract_state_siptr state = itru::make_shared_intrusive_ptr<ut::first_state>(std::ref(valid));
    ASSERT_TRUE(valid);
    ASSERT_EQ(state->use_count(), 2);
    {
        ut::state_machine stm(state);
    }
    ASSERT_TRUE(valid);
    ASSERT_EQ(state->use_count(), 1);
}

TEST(basic_state_machine_tests, use_case__ok)
{
    using abstract_state_siptr = ut::abstract_state::abstract_state_siptr;

    bool valid = true;
    int dest = 0;
    int src = 42;
    int offset = 1000;

    abstract_state_siptr gen_int = itru::make_shared_intrusive_ptr<ut::first_state>(std::ref(valid));
    ut::state_machine stm(std::move(gen_int));

    ASSERT_TRUE(valid);
    while (stm.has_state())
    {
        stm.run(dest, src, offset);
    }

    ASSERT_EQ(dest, 2042);
    ASSERT_EQ(src, 42);
    ASSERT_EQ(offset, 1000);
    ASSERT_FALSE(valid);
}
