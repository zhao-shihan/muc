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

set(CMAKE_CXX_STANDARD 17)
add_executable(ptr_vector_c++17 ptr_vector.c++)

set(CMAKE_CXX_STANDARD 20)
add_executable(ptr_vector_c++20 ptr_vector.c++)
add_executable(ceta_string_c++20 ceta_string.c++)
