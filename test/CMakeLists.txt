include_directories(${CMAKE_HOME_DIRECTORY})

if(MACE_ENABLE_UBSAN_IN_DEBUG_BUILD)
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options($<$<CONFIG:Debug>:-fsanitize=undefined>
                            $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
                            $<$<CONFIG:RelWithDebInfo>:-fsanitize=undefined>
                            $<$<CONFIG:RelWithDebInfo>:-fno-omit-frame-pointer>)
        add_link_options($<$<CONFIG:Debug>:-fsanitize=undefined>
                         $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
                         $<$<CONFIG:RelWithDebInfo>:-fsanitize=undefined>
                         $<$<CONFIG:RelWithDebInfo>:-fno-omit-frame-pointer>)
        if(DEFINED CMAKE_CONFIGURATION_TYPES)
            # multi-config generator
            message(STATUS "UndefinedBehaviorSanitizer will be enabled in debug build (-fsanitize=undefined)")
        else()
            # single-config generator
            if(LOWERCASED_CMAKE_BUILD_TYPE STREQUAL "debug")
                message(STATUS "UndefinedBehaviorSanitizer enabled (-fsanitize=undefined)")
            endif()
        endif()
    endif()
endif()

# More warnings
if(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND MACE_COMPILE_OPTIONS -Wall -Wextra -Wduplicated-cond -Wnon-virtual-dtor -pedantic -Wundef -Wunused-macros)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND MACE_COMPILE_OPTIONS -WCL4 -Wmove -Wnon-virtual-dtor -pedantic -Wundef -Wunused-macros
                                     -Wno-gnu-zero-variadic-macro-arguments)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND MACE_COMPILE_OPTIONS /W4)
endif()

set(CMAKE_CXX_STANDARD 17)
add_executable(ptr_vector_c++17 ptr_vector.c++)
add_executable(stopwatch_c++17 stopwatch.c++)

set(CMAKE_CXX_STANDARD 20)
add_executable(ceta_string_c++20 ceta_string.c++)
add_executable(ptr_vector_c++20 ptr_vector.c++)
add_executable(stopwatch_c++20 stopwatch.c++)
