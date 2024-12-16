#include <arba/uuid/uuid.hpp>
#include <arba/uuid/version.hpp>
#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << arba::uuid::project_name() << std::endl;
    std::cout << "EXIT SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
