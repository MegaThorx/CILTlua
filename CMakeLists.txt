cmake_minimum_required(VERSION 3.1)
project(ciltlua)

# Show more warnings
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
	# For MSVC
	add_definitions(/W2)
else()
	# For GCC and compilers that are compatible with GCC options (like CLang)
	add_definitions(-Wall)
endif()

# Require c++11 support
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Disable compiler specific extensions
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ciltlua src/main.cpp)
