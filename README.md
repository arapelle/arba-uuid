# Concept #

A C++ library providing Universal Unique ID features.

# Install #
## Requirements ##

Binaries:

- A C++20 compiler (ex: g++-14)
- CMake 3.26 or later

Testing Libraries (optional):

- [Google Test](https://github.com/google/googletest) 1.14 or later (optional)

## Clone

```
git clone https://github.com/arapelle/arba-uuid
```

## Use with `conan`

Create the conan package.
```
conan create . --build=missing -c
```
Add a requirement in your conanfile project file.
```python
    def requirements(self):
        self.requires("arba-uuid/0.2.0")
```

## Quick Install ##
There is a cmake script at the root of the project which builds the library in *Release* mode and install it (default options are used).
```
cd /path/to/arba-rand
cmake -P cmake/scripts/quick_install.cmake
```
Use the following to quickly install a different mode.
```
cmake -P cmake/scripts/quick_install.cmake -- TESTS BUILD Debug DIR /tmp/local
```

## Uninstall ##
There is a uninstall cmake script created during installation. You can use it to uninstall properly this library.
```
cd /path/to/installed-arba-uuid/
cmake -P uninstall.cmake
```

# How to use
## Example - Creating a UUID from a string.
```c++
#include <arba/uuid/uuid.hpp>
#include <arba/uuid/version.hpp>
#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << std::format("arba-uuid-{}", arba::uuid::version) << std::endl;
    arba::uuid::uuid id("03b0d57c-a6bd-40de-bbb7-d26534801bb0");
    std::cout << "uuid: " << id << std::endl;
    uuid::uuid id2("78fdfce9-76f0-4088-ba95-713cf711781a");
    std::cout << "uuid: " << id2 << std::endl;
    return EXIT_SUCCESS;
}

```

# License

[MIT License](./LICENSE.md) © arba-uuid
