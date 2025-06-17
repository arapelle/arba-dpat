#include <arba/dpat/state/state.hpp>
#include <arba/dpat/state/state_machine.hpp>
#include <arba/dpat/version.hpp>

#include <iostream>

namespace ex
{

class state_machine;

class abstract_state : public dpat::state<abstract_state, state_machine, int&, int&, const int&, int>
{
public:
    virtual ~abstract_state() override = default;
};

class state_machine : public dpat::state_machine<state_machine, abstract_state>
{
    using base_ = dpat::state_machine<state_machine, abstract_state>;

public:
    using base_::base_;
};

//---

class second_state;

class first_state : public abstract_state
{
public:
    first_state(bool& valid);

    virtual ~first_state() override { *pvalid_ = false; }

    virtual int& execute(state_machine& st_machine, int& dest, const int& src, int offset) override
    {
        offset += offset;
        dest = src + offset;
        st_machine.set_state(nullptr);
        return dest;
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

    virtual void invalidate() override { reset_shared_state_pointers(previous_state_); }

private:
    itru::shared_intrusive_ptr<first_state> previous_state_;
};

first_state::first_state(bool& valid)
    : pvalid_(&valid),
      next_state_(itru::make_shared_intrusive_ptr<second_state>(itru::shared_intrusive_ptr<first_state>(this)))
{
}

} // namespace ex

int main()
{
    using abstract_state_siptr = ex::abstract_state::abstract_state_siptr;

    bool valid = true;
    int dest = 0;
    int src = 42;
    int offset = 1000;

    abstract_state_siptr gen_int = itru::make_shared_intrusive_ptr<ex::first_state>(std::ref(valid));
    ex::state_machine stm(std::move(gen_int));

    while (stm.has_state())
    {
        stm.execute(dest, src, offset);
    }
    std::cout << "integer: " << dest << std::endl;

    std::cout << "EXIT SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
