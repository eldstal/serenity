if (NOT ENABLE_OSS_FUZZ)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option -Wno-literal-suffix -O2 -Wall -Wextra -std=c++2a -fPIC -g -Wno-deprecated-copy")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fPIC -g -Wno-deprecated-copy")
endif()

if (ENABLE_ADDRESS_SANITIZER)
    add_definitions(-fsanitize=address -fno-omit-frame-pointer)
    set(LINKER_FLAGS "${LINKER_FLAGS} -fsanitize=address")
endif()

if (ENABLE_MEMORY_SANITIZER)
    add_definitions(-fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer)
    set(LINKER_FLAGS "${LINKER_FLAGS} -fsanitize=memory -fsanitize-memory-track-origins")
endif()

if (ENABLE_UNDEFINED_SANITIZER)
    add_definitions(-fsanitize=undefined -fno-sanitize=vptr -fno-omit-frame-pointer)
    set(LINKER_FLAGS "${LINKER_FLAGS} -fsanitize=undefined -fno-sanitize=vptr")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang$")
    # Clang's default constexpr-steps limit is 1048576(2^20), GCC doesn't have one
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual -Wno-user-defined-literals -fconstexpr-steps=16777216")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASSERT_IS_EXIT")

    set(ORIGINAL_CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    set(ORIGINAL_CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
    set(ORIGINAL_CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-expansion-to-defined -Wno-literal-suffix")
endif()


add_executable(Generate_CSS_PropertyID_h Generate_CSS_PropertyID_h.cpp)
add_executable(Generate_CSS_PropertyID_cpp Generate_CSS_PropertyID_cpp.cpp)
add_executable(Generate_CSS_ValueID_h Generate_CSS_ValueID_h.cpp)
add_executable(Generate_CSS_ValueID_cpp Generate_CSS_ValueID_cpp.cpp)
add_executable(WrapperGenerator WrapperGenerator.cpp)
target_compile_options(WrapperGenerator PUBLIC -g)
target_link_libraries(Generate_CSS_PropertyID_h LagomCore)
target_link_libraries(Generate_CSS_PropertyID_cpp LagomCore)
target_link_libraries(Generate_CSS_ValueID_h LagomCore)
target_link_libraries(Generate_CSS_ValueID_cpp LagomCore)
target_link_libraries(WrapperGenerator LagomCore)
