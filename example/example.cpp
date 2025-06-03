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
