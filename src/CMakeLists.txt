cmake_minimum_required(VERSION 3.0)

project(libspng  LANGUAGES C  VERSION 0.7.3 )

set(SPNG_MAJOR 0)
set(SPNG_MINOR 7)
set(SPNG_REVISION 3)
set(SPNG_VERSION ${SPNG_MAJOR}.${SPNG_MINOR}.${SPNG_REVISION})

option(ENABLE_OPT "Enable architecture-specific optimizations" ON)
option(SPNG_SHARED "Build shared lib" ON)
option(SPNG_STATIC "Build static lib" ON)
option(BUILD_EXAMPLES "Build examples" ON)

# 改变安装路径可以使用下面的语句，Unix: "/usr/local", Windows: "c:/Program Files/${PROJECT_NAME}" 为默认值，所有的DESTINATION均以此目录为根目录
# set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/mymath)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/../${PROJECT_NAME}_${CMAKE_C_COMPILER_VERSION}_install)


include(GNUInstallDirs)

# Allow the usage of [PackageName]_ROOT variables for FindPackage
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12")
    cmake_policy(SET CMP0074 NEW)
endif()

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

set(spng_SOURCES spng/spng.c)

if(NOT CMAKE_HOST_WIN32)
    set(spng_LIBS -lm ${ZLIB_LIBRARIES})
else()
    set(spng_LIBS ${ZLIB_LIBRARIES})
endif()

if(NOT ENABLE_OPT)
    add_definitions( -DSPNG_DISABLE_OPT=1 )
endif()

set(TARGET_LIB_NAME spng  )

if(SPNG_SHARED)
    add_library(spng SHARED ${spng_SOURCES})
    target_link_libraries(spng ${spng_LIBS})
    install(TARGETS spng DESTINATION lib)

    if(BUILD_EXAMPLES)
        add_executable(example examples/example.c)
        target_include_directories(example PRIVATE ${PROJECT_SOURCE_DIR}/spng)
        target_link_libraries(example spng ${spng_LIBS})
    endif()
endif()

if(SPNG_STATIC)
    add_library(spng_static STATIC ${spng_SOURCES})
    target_compile_definitions(spng_static PUBLIC SPNG_STATIC)
    install(TARGETS spng_static DESTINATION lib)
endif()

install(FILES spng/spng.h DESTINATION include)


if(NOT CMAKE_HOST_WIN32 OR CYGWIN OR MINGW)
    set(prefix      ${CMAKE_INSTALL_PREFIX})
    set(exec_prefix ${CMAKE_INSTALL_PREFIX})
    set(libdir      ${CMAKE_INSTALL_FULL_LIBDIR})
    set(includedir  ${CMAKE_INSTALL_FULL_INCLUDEDIR})
    set(LIBS        "-lz -lm")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests/libspng.pc.in ${CMAKE_CURRENT_BINARY_DIR}/tests/libspng.pc @ONLY)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tests/libspng.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()


#//----------------------------------------------

target_include_directories( spng PRIVATE  ${CMAKE_SOURCE_DIR}/spng)
set_target_properties(spng PROPERTIES PUBLIC_HEADER ${CMAKE_SOURCE_DIR}/spng/spng.h)

# 写入库的版本信息
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        spngConfigVersion.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
        )

# 将库文件，可执行文件，头文件安装到指定目录
install(TARGETS spng
        EXPORT spngTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
        )

# 将库的相关信息输出为MyMathConfig.cmake，供find_package查找
install(EXPORT spngTargets
        FILE spngTargets.cmake
        NAMESPACE spng::
        DESTINATION lib/cmake/spng
        )

configure_file(spngConfig.cmake.in spng/spngConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/spng/spngConfig.cmake"
                "${CMAKE_CURRENT_BINARY_DIR}/spngConfigVersion.cmake"
        DESTINATION lib/cmake/spng
        )





