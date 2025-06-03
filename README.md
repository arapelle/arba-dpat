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
#include <arba/dpat/dpat.hpp>
#include <arba/dpat/version.hpp>
#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << arba::dpat::project_name() << std::endl;
    std::cout << "EXIT SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
```

# License

[MIT License](./LICENSE.md) © arba-dpat
