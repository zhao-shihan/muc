if(MUC_ENABLE_UBSAN_IN_DEBUG_BUILD)
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
    list(APPEND MUC_COMPILE_OPTIONS -Wall -Wextra -Wduplicated-cond -Wnon-virtual-dtor -pedantic -Wundef -Wunused-macros)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND MUC_COMPILE_OPTIONS -WCL4 -Wmove -Wnon-virtual-dtor -pedantic -Wundef -Wunused-macros
                                     -Wno-gnu-zero-variadic-macro-arguments)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND MUC_COMPILE_OPTIONS /W4)
endif()

macro(add_executable_with_feature prefix feature)
    add_executable(${prefix}_${feature} ${prefix}.c++)
    target_link_libraries(${prefix}_${feature} PRIVATE muc)
    target_compile_features(${prefix}_${feature} PRIVATE ${feature})
    target_compile_definitions(${prefix}_${feature} PRIVATE MUC_STATIC_TEST)
    target_compile_options(${prefix}_${feature} PRIVATE ${MUC_COMPILE_OPTIONS})

    # add_executable(${prefix}_${feature}_universe ${prefix}.c++)
    # target_include_directories(${prefix}_${feature}_universe PRIVATE ${CMAKE_HOME_DIRECTORY}/include)
    # target_compile_features(${prefix}_${feature}_universe PRIVATE ${feature})
    # target_compile_definitions(${prefix}_${feature}_universe PRIVATE MUC_STATIC_TEST MUC_TEST_UNIVERSE)
    # target_compile_options(${prefix}_${feature} PRIVATE ${MUC_COMPILE_OPTIONS})
endmacro(add_executable_with_feature)

add_executable_with_feature(find_root cxx_std_17)
add_executable_with_feature(math cxx_std_17)
add_executable_with_feature(ptr_span cxx_std_17)
add_executable_with_feature(ptr_vector cxx_std_17)
add_executable_with_feature(stopwatch cxx_std_17)

add_executable_with_feature(ceta_string cxx_std_20)
add_executable_with_feature(find_root cxx_std_20)
add_executable_with_feature(math cxx_std_20)
add_executable_with_feature(ptr_span cxx_std_20)
add_executable_with_feature(ptr_vector cxx_std_20)
add_executable_with_feature(stopwatch cxx_std_20)

# add_executable_with_feature(ceta_string cxx_std_23)
# add_executable_with_feature(find_root cxx_std_23)
# add_executable_with_feature(math cxx_std_23)
# add_executable_with_feature(ptr_span cxx_std_23)
# add_executable_with_feature(ptr_vector cxx_std_23)
# add_executable_with_feature(stopwatch cxx_std_23)
