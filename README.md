# Concept #

A C++ library providing design pattern tools.

# Install #
## Requirements ##

Binaries:

- A C++20 compiler (ex: g++-14)
- CMake 3.26 or later

Testing Libraries (optional):

- [Google Test](https://github.com/google/googletest) 1.14 or later (optional)

## Clone

```
git clone https://github.com/arapelle/arba-dpat
```

## Use with `conan`

Create the conan package.
```
conan create . --build=missing -c
```
Add a requirement in your conanfile project file.
```python
    def requirements(self):
        self.requires("arba-dpat/0.1.0")
```

## Quick Install ##
There is a cmake script at the root of the project which builds the library in *Release* mode and install it (default options are used).
```
cd /path/to/arba-dpat
cmake -P cmake/scripts/quick_install.cmake
```
Use the following to quickly install a different mode.
```
cmake -P cmake/scripts/quick_install.cmake -- TESTS BUILD Debug DIR /tmp/local
```

## Uninstall ##
There is a uninstall cmake script created during installation. You can use it to uninstall properly this library.
```
cd /path/to/installed-arba-dpat/
cmake -P uninstall.cmake
```

# How to use
## Example
```c++
#include <arba/dpat/state/state.hpp>
#include <arba/dpat/state/state_machine.hpp>
#include <arba/dpat/version.hpp>

#include <iostream>

namespace ex
{

class abstract_state : public dpat::state<abstract_state, int&, const int&, int>
{
public:
    virtual ~abstract_state() override = default;
};

class second_state;

class first_state : public abstract_state
{
public:
    first_state(bool& valid);

    virtual ~first_state() override { *pvalid_ = false; }

    virtual abstract_state_siptr execute(int& dest, const int& src, int offset) override
    {
        offset += offset;
        dest = src + offset;
        return abstract_state_siptr();
    }

    virtual void invalidate() override { reset_shared_state_pointers(next_state_); }

private:
    bool* pvalid_ = nullptr;
    itru::shared_intrusive_ptr<second_state> next_state_;
};

class second_state : public abstract_state
{
public:
    second_state(itru::shared_intrusive_ptr<first_state> state_siptr) { previous_state_ = std::move(state_siptr); }

    virtual ~second_state() override = default;

    virtual abstract_state_siptr execute(int&, const int&, int) override { return abstract_state_siptr(); }

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
    dpat::state_machine<ex::abstract_state> stm(std::move(gen_int));

    while (stm.has_state())
    {
        stm.execute(dest, src, offset);
    }

    std::cout << "integer: " << dest << std::endl;
    std::cout << "EXIT SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
```

# License

[MIT License](./LICENSE.md) © arba-dpat
