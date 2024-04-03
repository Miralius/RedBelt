cmake_minimum_required(VERSION 3.27)
project(RedBelt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_subdirectory(MaxElementIf)
add_subdirectory(MacrosLog)
add_subdirectory(ComparatorGenerator)
add_subdirectory(UpdateFieldMacro)
add_subdirectory(BadMacro)
add_subdirectory(UniqId)