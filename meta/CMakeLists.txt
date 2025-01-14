cmake_minimum_required(VERSION 3.10.2)

project(OpenStudioMeta)

set(CMAKE_VERSION_MAJOR 2)
set(CMAKE_VERSION_MINOR 9)
set(CMAKE_VERSION_PATCH 0)

# Use ccache is available, has to be before "project()"
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  # Support Unix Makefiles and Ninja
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CMAKE_VERSION_BUILD "Unknown" CACHE STRING "Build number")
find_package(Git)

if(NOT GIT_FOUND)
  find_program(GIT_EXECUTABLE git HINTS "$ENV{LOCALAPPDATA}/Programs/git/bin" "C:/Program Files/Git/bin")
  if(NOT GIT_EXECUTABLE_NOTFOUND)
    set(GIT_FOUND TRUE)
  endif()
endif()

if(GIT_FOUND)
  execute_process(COMMAND "${GIT_EXECUTABLE}" "rev-parse" "--short=10" "HEAD"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  TIMEOUT 10
                  RESULT_VARIABLE RESULT
                  OUTPUT_VARIABLE GIT_VERSION
                  ERROR_QUIET
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(${RESULT} EQUAL 0 AND NOT "${GIT_VERSION}" EQUAL "${CMAKE_VERSION_BUILD}")
    set(CMAKE_VERSION_BUILD ${GIT_VERSION} CACHE STRING "Build number" FORCE) # git sha
  endif()

  get_filename_component(GIT_DIR "${GIT_EXECUTABLE}" DIRECTORY)
else()
  set(GIT_DIR "")
endif()

find_program(PATCH_EXE patch HINTS "${GIT_DIR}" "${GIT_DIR}/../bin/" "${GIT_DIR}/../usr/bin/")
string(COMPARE EQUAL "${PATCH_EXE}" "PATCH_EXE-NOTFOUND" PATCH_EXE_NOTFOUND)
if(PATCH_EXE_NOTFOUND)
  message(SEND_ERROR "Required program patch not found")
endif()

option(NIGHTLY_BUILD "Use configurations for Nightly Build" OFF)

set(OPENSTUDIO_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}")

# Search for modules in the openstudiocore dir first to override cmake ones
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" "${CMAKE_SOURCE_DIR}")

# Build C++ documentation using Doxygen
# Requires: doxygen
option(BUILD_DOCUMENTATION "Build Documentation" OFF)

if(BUILD_DOCUMENTATION)
  find_package(Doxygen REQUIRED)
endif()

# Build CSharp bindings
# Requires: SWIG
option(BUILD_CSHARP_BINDINGS "Build CSharp bindings" OFF)

# Build Java bindings
# Requires: SWIG
option(BUILD_JAVA_BINDINGS "Build Java bindings" OFF)

# Build Node V8 bindings
# Requires: SWIG
option(BUILD_V8_BINDINGS "Build V8 bindings" OFF)

# Build Python bindings
# Requires: SWIG Python
option(BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)

# Build ctest testing
# Requires: EnergyPlus
option(BUILD_TESTING "Build testing targets" OFF)

# Build the actual openstudiocore project
option(BUILD_OSCORE "Build openstudiocore" OFF)

# Build package
# Requires: EnergyPlus
option(BUILD_PACKAGE "Build package" OFF)

# Build regressions
option(BUILD_REGRESSIONS "Build regressions" OFF)

# Build shared libraries, configuration affects OpenStudioCore
option(BUILD_SHARED_LIBS "Build OS libs as shared libraries" OFF)

set(MAXIMIZE_CPU_USAGE OFF CACHE BOOL "Attempt to fully load the CPU during builds")
mark_as_advanced(MAXIMIZE_CPU_USAGE)

MESSAGE(STATUS "CMAKE_C_COMPILER_VERSION = ${CMAKE_C_COMPILER_VERSION}")
MESSAGE(STATUS "CMAKE_GENERATOR = ${CMAKE_GENERATOR}")
MESSAGE(STATUS "CMAKE_GENERATOR_TOOLSET = ${CMAKE_GENERATOR_TOOLSET}")

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

# Xcode/Ninja generators undefined MAKE
if(CMAKE_GENERATOR MATCHES "Make")
  set(MAKE "$(MAKE)")
else()
  set(MAKE make)
endif()

# Add Color Output if Using Ninja
macro(AddCXXFlagIfSupported flag test)
   CHECK_CXX_COMPILER_FLAG(${flag} ${test})
   if( ${${test}} )
      message(STATUS "adding ${flag}")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
   endif()
endmacro()

if("Ninja" STREQUAL ${CMAKE_GENERATOR})
  # Clang
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    AddCXXFlagIfSupported(-fcolor-diagnostics COMPILER_SUPPORTS_fcolor-diagnostics)
  endif()

  # g++
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # For some reason it doesn't say its supported, but it works...
    # AddCXXFlagIfSupported(-fdiagnostics-color COMPILER_SUPPORTS_fdiagnostics-color)
    message(STATUS "Forcing -fdiagnostics-color=always")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
  endif()
endif()

if(MSVC)
#  # Visual Studio compiler check
#  # http://en.wikipedia.org/wiki/Visual_C%2B%2B#32-bit_and_64-bit_versions
#  if    (${CMAKE_C_COMPILER_VERSION} VERSION_LESS "18.0.21005.1")
#    message(FATAL_ERROR "Visual Studio earlier than VS2013 is not supported")
#  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.0.21005.1")
#    # VS2013
#    message(SEND_ERROR "Update 4 for Visual Studio 2013 is required")
#  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.30501")
#    # VS2013 Update 2:
#    message(SEND_ERROR "Update 4 for Visual Studio 2013 is required")
#  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.30723")
#    message(WARNING "Update 4 for Visual Studio 2013 is strongly recommended")
#  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.31101")
#    # VS2013 Update 4: no-op
#  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.40629")
#    # VS2013 Update 5: no-op
#  endif()
#
#  if ("${CMAKE_GENERATOR}" STREQUAL "Visual Studio 12 2013 Win64")
#    set(CMAKE_GENERATOR_TOOLSET "v120</PlatformToolset><PreferredToolArchitecture>x64</PreferredToolArchitecture><PlatformToolset>v120")
#  elseif ("${CMAKE_GENERATOR}" STREQUAL "Visual Studio 14 2015 Win64")
#    set(CMAKE_GENERATOR_TOOLSET "v140</PlatformToolset><PreferredToolArchitecture>x64</PreferredToolArchitecture><PlatformToolset>v140")
#  else()
#    message(STATUS "This version of Visual Studio will not use the 64 bit linker. Build times will likely suffer.")
#  endif()

  # Build with Multiple Processes
  set(BUILD_WITH_MULTIPLE_PROCESSES ON CACHE BOOL "/MP compiler flag for full processor utilization")
  mark_as_advanced(BUILD_WITH_MULTIPLE_PROCESSES)
endif()


# Apple Sanity Check
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c17 -std=c++17 -stdlib=libc++")
  #if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
  #  message(FATAL_ERROR "OSX Deployment Target Must Be Set")
  #endif()

  #set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Arch" FORCE)

  #if(NOT CMAKE_OSX_SYSROOT)
  #  message(FATAL_ERROR "OSX Sysroot must be set")
  #elseif(CMAKE_OSX_SYSROOT MATCHES ".*10\\.9.*")
  #  set(OSX_SDK "10.9")
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -std=c++11 -stdlib=libc++")
  #elseif(CMAKE_OSX_SYSROOT MATCHES ".*10\\.10.*")
  #  set(OSX_SDK "10.10")
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -std=c++11 -stdlib=libc++")
  #elseif(CMAKE_OSX_SYSROOT MATCHES ".*10\\.11.*")
  #  set(OSX_SDK "10.11")
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -std=c++11 -stdlib=libc++")
  #else()
  #  message(FATAL_ERROR "OSX Sysroot must use the 10.9, 10.10, or 10.11 SDK")
  #endif()

endif()


if(BUILD_V8_BINDINGS)
  if(WIN32)
    message(SEND_ERROR "V8 is only tested and supported on Unix like systems")
  endif()

  option(BUILD_NODE_MODULES "Build V8 Bindings as Node Modules" ON)

  if(BUILD_NODE_MODULES)
    # Build Node for linking to Node modules
    option(BUILD_NODE "Build Node" ON)
  else()
    option(BUILD_V8 "Build V8" ON)
  endif()

endif()

if(MSVC)
  mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
endif()

# Use PCH
option(USE_PCH "Use precompiled headers" OFF)

include(ExternalProject)
include(ProcessorCount)

# set number of cores to be used when building boost and qt, idea is you will do make -jX so boost and qt will build in parallel
ProcessorCount(CPUCOUNT)
if(CPUCOUNT EQUAL 0)
  set(CPUCOUNT 1)
endif()

if(UNIX)
  set(BOOST_CPUCOUNT ${CPUCOUNT})
  set(QT_CPUCOUNT ${CPUCOUNT})
endif()

mark_as_advanced(
  ProcessorCount_cmd_getconf
  ProcessorCount_cmd_sysctl
)

if(BUILD_WITH_MULTIPLE_PROCESSES)
  add_definitions(/MP)
endif()

###############################################################################

###############################################################################
# Conan

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${PROJECT_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.13/conan.cmake"
    "${PROJECT_BINARY_DIR}/conan.cmake")
endif()

include(${PROJECT_BINARY_DIR}/conan.cmake)

message(STATUS "RUNNING CONAN")

conan_check(VERSION 1.0.0 REQUIRED)

conan_add_remote(NAME bincrafters
  URL https://api.bintray.com/conan/bincrafters/public-conan)

set(CONAN_OPTIONS "")
set(CONAN_BUILD "")
if (MSVC)
  set(CONAN_OPENSSL "")
elseif (APPLE)
  set(CONAN_OPENSSL "")
else()
  set(CONAN_OPENSSL "OpenSSL/1.1.0g@conan/stable")
endif()

# DLM: add option for shared libs if we are building shared?

conan_cmake_run(REQUIRES
  ${CONAN_OPENSSL}
  BASIC_SETUP CMAKE_TARGETS NO_OUTPUT_DIRS
  OPTIONS ${CONAN_OPTIONS}
  BUILD ${CONAN_BUILD}
)

#message("CONAN_TARGETS = ${CONAN_TARGETS}")
message(STATUS "DONE RUNNING CONAN")

if(UNIX)
  if (NOT APPLE)
    set(OPENSSL_ROOT_DIR ${CONAN_OPENSSL_ROOT})
  endif()
endif()

# OpenSSL is required to build Qt
# to determine the version of OpenSSL used by Ruby:
#   ruby.exe -ropenssl -e 'puts OpenSSL::OPENSSL_VERSION'
if(UNIX)
  find_package(OpenSSL)
  if(NOT ${OPENSSL_FOUND})
    message(SEND_ERROR "OpenSSL could not be found, and is required for HTTPS communication")
    message(SEND_ERROR "Please install OpenSSL development libraries using your package management system (possibly libssl-dev)")
  else()
    mark_as_advanced(
      LIB_EAY_DEBUG
      LIB_EAY_RELEASE
      SSL_EAY_DEBUG
      SSL_EAY_RELEASE
    )
  endif()

  MESSAGE(STATUS "OPENSSL_ROOT_DIR = ${OPENSSL_ROOT_DIR}")

elseif(WIN32)

  option(BUILD_OPENSSL "Build OpenSSL From Source" OFF)


  set(OPENSSL_VERSION "1.0.2q")
  set(OPENSSL_SOURCE_URL "https://github.com/openssl/openssl/archive/OpenSSL_1_0_2q.zip")

  set(OPENSSL_SOURCE_HASH ffeaa3eadabfeb7170f85ecca14fb0d9)
  if(CMAKE_CL_64)
    set(OPENSSL_ARCH 64)
    set(OPENSSL_EXPECTED_HASH 4ee78779f5777a854b242e777c990b7a)
  else()
    set(OPENSSL_ARCH 32)
    set(OPENSSL_EXPECTED_HASH DOES_NOT_EXIST_fb2aa1a8ceb85f68055044a85f1dbc94)
  endif()

  set(OPENSSL_PATH "OpenSSL-Win${OPENSSL_ARCH}")
  set(OPENSSL_ROOT_DIR "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")

  if(BUILD_OPENSSL)
    # requires perl (http://strawberryperl.com/) and NASM (https://www.nasm.us/) in the path
    # http://p-nand-q.com/programming/windows/building_openssl_with_visual_studio_2013.html
    # http://developer.covenanteyes.com/building-openssl-for-visual-studio/
    configure_file("${CMAKE_SOURCE_DIR}/../dependencies/build_openssl.bat.in" "${CMAKE_BINARY_DIR}/build_openssl.bat")
    ExternalProject_Add(OpenSSL
      URL "${OPENSSL_SOURCE_URL}"
      URL_MD5 "${OPENSSL_SOURCE_HASH}"
      CONFIGURE_COMMAND ""
      PATCH_COMMAND ""
      BUILD_COMMAND cmd /C "${CMAKE_BINARY_DIR}/build_openssl.bat"
      INSTALL_COMMAND ""
      BUILD_IN_SOURCE 1
    )

  else()
    # download pre-built binaries
    if(EXISTS "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip")
      file(MD5 "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" OPENSSL_HASH)
    endif()
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" OR NOT "${OPENSSL_HASH}" MATCHES "${OPENSSL_EXPECTED_HASH}")
      message(STATUS "Downloading OpenSSL ${OPENSSL_VERSION} (${OPENSSL_ARCH}-bit)")
      file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/OpenSSL-Win${OPENSSL_ARCH}-${OPENSSL_VERSION}.zip" "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${OPENSSL_EXPECTED_HASH})
      execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
    endif()

  endif()

  MESSAGE("OPENSSL_ROOT_DIR = ${OPENSSL_ROOT_DIR}")
endif()

if(BUILD_NODE)
  # OSX can work with a static ruby library
  ExternalProject_Add(Node
    URL http://nodejs.org/dist/v0.10.28/node-v0.10.28.tar.gz
    CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && sh -c "./configure --prefix=${CMAKE_BINARY_DIR}/Node-prefix/src/Node-install"
    BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && ${MAKE}
    INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && ${MAKE} install && ${MAKE} install-includes && ../Node-install/bin/npm install jasmine-node node-gyp -g && ../Node-install/bin/node ../Node-install/bin/node-gyp install
  )
  set(NODE_BIN_DIR ${CMAKE_BINARY_DIR}/Node-prefix/src/Node-install/bin/)
  set(NODE_INCLUDE_DIR "$ENV{HOME}/.node-gyp/0.10.28")
endif()

if(BUILD_V8)
  ExternalProject_Add(V8
    SVN_REPOSITORY http://v8.googlecode.com/svn/branches/3.19
    CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/V8-prefix/src/V8 && ${MAKE} dependencies
    BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/V8-prefix/src/V8 && ${MAKE} x64
    INSTALL_COMMAND ""
  )
  set(V8_INCLUDE_DIR ${CMAKE_BINARY_DIR}/V8-prefix/src/V8/include)
endif()

# Ruby
option(BUILD_RUBY "Build Ruby" OFF)
if( BUILD_RUBY )

  if( UNIX )
    set(RUBY_EXT_CONFIG "")
    if (NOT APPLE)
    #  # DLM: still missing: curses, dbm, gdbm
    #  # gbdm: libgbdm3 for Up to 16.04, libgbdm5 for 18.04
    #  # sudo apt-get -y install ruby autoconf automake libtool bison libgdbm-dev zlib1g-dev libffi-dev libssl-dev
    #  # sudo apt-get -y install build-essential libssl-dev libreadline-dev zlib1g-dev

      # TODO: build against the same OpenSSL that the core project uses
      set(RUBY_EXT_CONFIG "--with-openssl-dir=${OPENSSL_ROOT_DIR}")
      message(STATUS "RUBY_EXT_CONFIG = '${RUBY_EXT_CONFIG}'")
    endif()

    # KSB: For Mac, you may need to install some things...
    # brew install homebrew/dupes/zlib
    # brew link --overwrite --force homebrew/dupes/zlib
    # brew install autoconf automake libtool
    # brew link autoconf automake libtool
    ExternalProject_Add(Ruby
      URL https://codeload.github.com/ruby/ruby/tar.gz/v2_5_1
      URL_MD5 98b550a05fec0f7a43a22d6c37404fd4
      BUILD_IN_SOURCE 1
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/../dependencies/ruby/Ruby.patch
      CONFIGURE_COMMAND autoconf && sh -c "./configure --with-static-linked-ext ${RUBY_EXT_CONFIG} --disable-install-doc --prefix=${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install"
      BUILD_COMMAND ${MAKE} # cd ext/openssl && ../../ruby extconf.rb && cd ../readline && ../../ruby extconf.rb && cd ../../ && ${MAKE}
      INSTALL_COMMAND ${MAKE} install && sh -c "cp ./rbconfig.rb ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/ruby/2.5.0/." && sh -c "rm -rf ./ext/-test-" && sh -c "mkdir ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/ext" && sh -c "mkdir ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/enc" && sh -c "find ./ext -name '*.a' | xargs -I {} cp {} ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/ext/" && sh -c "find ./enc -name '*.a' | xargs -I {} cp {} ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/enc"
    )
    find_file( RUBY_CONFIG_INCLUDE_DIR config.h PATHS "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/include/" )
    set(RUBY_LIBRARY "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/libruby-static.a") # TODO: might be libruby.2.5.1-static.a on Mac
    set(RUBY_INCLUDE_DIR "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/include/ruby-2.5.0")
    set(RUBY_EXECUTABLE "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/bin/ruby")
    set(RUBY_SOURCE_DIR "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby")
    set(RUBY_INSTALL_DIR "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install")

  else()

    ExternalProject_Add(ZLib
      URL_HASH SHA256=c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1
      URL http://zlib.net/zlib-1.2.11.tar.gz
      BUILD_COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-build" --config $<CONFIG> --target INSTALL
      CMAKE_CACHE_ARGS
        -DINSTALL_BIN_DIR:STRING=${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-install/bin/
        -DINSTALL_INC_DIR:STRING=${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-install/include/
        -DINSTALL_LIB_DIR:STRING=${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-install/lib/
        -DINSTALL_MAN_DIR:STRING=${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-install/share/man/
        -DINSTALL_PKGCONFIG_DIR:STRING=${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-install/share/pkgconfig/
    )

    # DLM: can't figure out a good way to copy pdb for debug only, just do manually for now
    ExternalProject_Add(FFI
      URL_HASH SHA256=bf6813045c0d470aedc3c69f9ec11fe351c2d7360c7d5396dc9a6ddc4830033c
      URL https://github.com/winlibs/libffi/archive/libffi-3.2.1.zip
      CONFIGURE_COMMAND ""
      BUILD_COMMAND devenv /upgrade ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI/win32/vc14_x64/libffi-msvc.sln
        COMMAND msbuild ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI/win32/vc14_x64/libffi-msvc.sln /p:Configuration=$<CONFIG> /p:DefineConstants="FFI_BUILDING" "/p:Platform=${CMAKE_VS_PLATFORM_NAME}" "/p:WindowsTargetPlatformVersion=${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}"
      INSTALL_COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI-install/$<CONFIG>/
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI-install/$<CONFIG>/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI/include ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI-install/$<CONFIG>/include
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI/fficonfig.h ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI-install/$<CONFIG>/include/fficonfig.h
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI/src/x86/ffitarget.h ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI-install/$<CONFIG>/include/ffitarget.h
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI-install/$<CONFIG>/lib
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI/win32/vc14_x64/x64/$<CONFIG>/libffi.lib ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI-install/$<CONFIG>/lib/libffi.lib
        #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI/win32/vc14_x64/x64/$<CONFIG>/libffi.pdb ${CMAKE_BINARY_DIR}/FFI-prefix/src/FFI-install/$<CONFIG>/lib/libffi.pdb
    )

    configure_file("${CMAKE_SOURCE_DIR}/../dependencies/ruby/build_ruby.bat.in" "${CMAKE_BINARY_DIR}/build_ruby.bat")
    # This will require bison, sed, and at least ruby 1.8 in your path
    # Please install DevKit even though the build will be done by msvc the ruby build system uses some traditionally unix tools

    # DLM notes:
    # @$(WIN32DIR:/=\)\rtname conftest.exe >>$(MAKEFILE) is used to determine which MSCV Runtime is linked

    # Post Build:
    # rename libs to : x64-msvcr140-ruby255.lib and x64-msvcr140-ruby255-static.lib, do not rename dll in bin dir
    # patch config.h to allow for multiple _MSC_VER values:
    #   #if !(_MSC_VER == 1915 || _MSC_VER == 1916 || _MSC_VER == 1917 || _MSC_VER == 1918 || _MSC_VER == 1919)
    # tar -zcvf ruby_2_5_5_msvc2017_static.tar.gz Ruby-install

    # Note 2.5.2 is required to fix bug with Windows 10 and Ruby
    ExternalProject_Add(Ruby
      DEPENDS ZLib FFI
      URL https://codeload.github.com/ruby/ruby/tar.gz/v2_5_5
      URL_HASH SHA256=A3BA7160C7E0CE03865CDABA9B7C26974A8A0FC23FC953269E0463C5ACAFB7A1
      BUILD_IN_SOURCE 1
      # DLM: ruby_2_5_1_msvc2017_static_v3.tar.gz on s3 was built with experimental Ruby.win.wopen.patch patch from ruby_patches branch, not using it for now
      #PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/../dependencies/ruby/Ruby.patch && ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/ruby/Ruby.win.patch && ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/ruby/Ruby.win.wopen.patch
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/../dependencies/ruby/Ruby.patch && ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/../dependencies/ruby/Ruby.win.patch && ${PATCH_EXE}

      CONFIGURE_COMMAND ""
      BUILD_COMMAND cmd /C "${CMAKE_BINARY_DIR}/build_ruby.bat" "$<CONFIG>"
      INSTALL_COMMAND ""
    )
    set(RUBY_INSTALL_DIR "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/$<CONFIG>")
  endif()
endif()

# OpenStudio Gems
# DLM: moved to https://github.com/NREL/openstudio-gems

# Boost

option(BUILD_BOOST "Build Boost" OFF)
if(BUILD_BOOST)
  if(APPLE)
    set(unix-flags "-std=c++17 -stdlib=libc++ -fPIC")
    ExternalProject_Add(Boost
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/boost_1_68_0.tar.gz
      URL_MD5 5d8b4503582fffa9eefdb9045359c239
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./bootstrap.sh
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./b2 toolset=clang cxxflags=${unix-flags} linkflags=${unix-flags} variant=debug,release define=BOOST_CHRONO_HEADER_ONLY --layout=tagged --with-filesystem --with-random --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${BOOST_CPUCOUNT} install
      INSTALL_COMMAND ""
    )
    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
    set(BOOST_LIBRARYDIR ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install/lib)
  elseif(UNIX)
    set(unix-flags "-std=c++17 -fPIC")
    ExternalProject_Add(Boost
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/boost_1_68_0.tar.gz
      URL_MD5 5d8b4503582fffa9eefdb9045359c239
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./bootstrap.sh
      PATCH_COMMAND ""
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./b2 cxxflags=${unix-flags} linkflags=${unix-flags} link=static variant=debug,release define=BOOST_CHRONO_HEADER_ONLY --layout=tagged --with-atomic --with-filesystem --with-random --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${BOOST_CPUCOUNT} install
      INSTALL_COMMAND ""
    )
    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
    set(BOOST_LIBRARYDIR ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install/lib)
  else()
    # visual studio 2017
    set(B2_TOOLSET "toolset=msvc-14.1")

    if(CMAKE_CL_64)
      ExternalProject_Add(Boost
        URL http://openstudio-resources.s3.amazonaws.com/dependencies/boost_1_68_0.tar.gz
        URL_MD5 5d8b4503582fffa9eefdb9045359c239
        CONFIGURE_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && bootstrap.bat"
        BUILD_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && b2 ${B2_TOOLSET} link=static address-model=64 define=BOOST_CHRONO_HEADER_ONLY --build-type=minimal --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT}" install
        INSTALL_COMMAND ""
      )
    else()
      ExternalProject_Add(Boost
        URL http://openstudio-resources.s3.amazonaws.com/dependencies/boost_1_68_0.tar.gz
        URL_MD5 93780777cfbf999a600f62883bd54b17
        CONFIGURE_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && bootstrap.bat"
        BUILD_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && b2 ${B2_TOOLSET} link=static address-model=32 define=BOOST_CHRONO_HEADER_ONLY --build-type=minimal --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT}" install
        INSTALL_COMMAND ""
      )
    endif()

    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
    set(BOOST_LIBRARYDIR ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install/lib)
  endif()
endif()

option(BUILD_QT "Build Qt" OFF)
option(Qt5Static "Build Static Qt" OFF)

if(BUILD_QT)

  if(APPLE)
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install")
    if( Qt5Static )
      set(QT_CONFIGURE_COMMAND "-v -release -opensource -qt-pcre -qt-harfbuzz -static -openssl-linked -I${OPENSSL_INCLUDE_DIR} OPENSSL_LIBS=\\\"${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY}\\\" -no-pch -no-ssse3 -qt-sql-sqlite -plugin-sql-sqlite -skip enginio -no-compile-examples -nomake examples -nomake tests -nomake tools -skip qtmultimedia -confirm-license -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-static-install")
    else()
      message(WARNING "Shared Apple Qt build untested")
      set(QT_CONFIGURE_COMMAND "-v -release -opensource -qt-pcre -qt-harfbuzz -shared -openssl -no-pch -no-ssse3 -qt-sql-sqlite -plugin-sql-sqlite -skip enginio -no-compile-examples -nomake examples -nomake tests -nomake tools -skip qtmultimedia -confirm-license -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install")
    endif()
    ExternalProject_Add(Qt
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/qt-everywhere-src-5.11.2.tar.xz
      URL_MD5 152a8ade9c11fe33ff5bc95310a1bb64
      CONFIGURE_COMMAND sh -c "./configure ${QT_CONFIGURE_COMMAND}"
      PATCH_COMMAND ""
      BUILD_COMMAND sh -c "make -j${CPUCOUNT} && make install"
      INSTALL_COMMAND ""
      BUILD_IN_SOURCE 1
    )
    set(QT_INSTALL_DIR "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/")

  elseif( UNIX )
    # install deps according to here http://doc.qt.io/qt-5/linux-requirements.html
    # also reference https://wiki.qt.io/Building_Qt_5_from_Git
    if( Qt5Static )
      set(QT_CONFIGURE_COMMAND "-v -release -opensource -qt-pcre -qt-harfbuzz -static -openssl -no-pch -no-ssse3 -sql-sqlite -plugin-sql-sqlite -skip enginio -skip qtconnectivity -skip qtlocation -skip qtmultimedia -skip qtpurchasing -skip qtsensors -skip qtwayland -no-compile-examples -nomake examples -nomake tests -nomake tools -qt-xcb -confirm-license  -no-qml-debug -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-static-install")
    else()
      set(QT_CONFIGURE_COMMAND "-v -release -opensource -qt-pcre -qt-harfbuzz -shared -openssl -no-pch -no-ssse3 -sql-sqlite -plugin-sql-sqlite -skip enginio -skip qtconnectivity -skip qtmultimedia -skip qtpurchasing -skip qtsensors -skip qtwayland -no-compile-examples -nomake examples -nomake tests -nomake tools -qt-xcb -confirm-license  -no-qml-debug -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install")
    endif()
    ExternalProject_Add(Qt
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/qt-everywhere-src-5.11.2.tar.xz
      URL_MD5 152a8ade9c11fe33ff5bc95310a1bb64
      CONFIGURE_COMMAND sh -c "./configure ${QT_CONFIGURE_COMMAND}"
      PATCH_COMMAND ""
      BUILD_COMMAND sh -c "make -j${CPUCOUNT} && make install"
      INSTALL_COMMAND ""
      BUILD_IN_SOURCE 1
    )
    set(QT_INSTALL_DIR "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/")

  else()
    # DLM: I could not get Qt to install Webengine correctly from the super build level it would return the following message:
    #    Could not detect Windows SDK version ('WindowsSDKVersion' environment variable is not set).
    #    Qt WebEngine on Windows requires a Windows SDK version 10.0.10586 or newer.
    #    QtWebEngine will not be built.
    # Installed the Windows 10 SDK and initializing the command prompt would set that env variable but Qt did not recognize it:
    #    "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" amd64
    # Initializing the command prompt and calling 'nmake install' from inside the qtwebengine did install Webengine components
    # In the end I uploaded two zip files, one zip of the official installer downloaded from Qt 'qt_5_11_msvc2017_64_shared.zip'
    # The other one was built by me 'qt_5_11_msvc2017_64_shared_nrel_build.zip'
    # I reccomend using the official one, will make it easier to update in the future
    # Static Qt libraries are built by NREL using script below

    configure_file("${CMAKE_SOURCE_DIR}/../dependencies/qt/build_static_qt.bat.in" "${CMAKE_BINARY_DIR}/build_static_qt.bat")
    configure_file("${CMAKE_SOURCE_DIR}/../dependencies/qt/build_shared_qt.bat.in" "${CMAKE_BINARY_DIR}/build_shared_qt.bat")
    if( Qt5Static )
      set(QT_BUILD_SCRIPT "build_static_qt.bat")
    else()
      set(QT_BUILD_SCRIPT "build_shared_qt.bat")
    endif()
    ExternalProject_Add(Qt
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/qt-everywhere-src-5.11.2.zip
      URL_MD5 f6647d66d1c062b0603831290a4c7e4a
      BUILD_IN_SOURCE 1
      CONFIGURE_COMMAND cmd /C "${CMAKE_BINARY_DIR}/${QT_BUILD_SCRIPT}"
      PATCH_COMMAND ""
      BUILD_COMMAND cmd /C "nmake"
      INSTALL_COMMAND cmd /C "nmake install"
    )
    set(QT_INSTALL_DIR "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/")
  endif()

else()
  set(QT_INSTALL_DIR "")
endif()

option(BUILD_DVIEW "Build DView" OFF)
if( BUILD_DVIEW )
    ExternalProject_Add(curl
      URL https://curl.haxx.se/download/curl-7.55.1.tar.gz
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )

    ExternalProject_Add(lk
      GIT_REPOSITORY git://github.com/NREL/lk
      GIT_TAG 1e4291ab85b2679fbb06391c3eaf23c8053e8481
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )

    ExternalProject_Add(DView
      DEPENDS lk curl
      GIT_REPOSITORY https://github.com/NREL/wex.git
      GIT_TAG 2e28be26a5b4690b95863e960276c2d8e4aa0867
      CMAKE_CACHE_ARGS
        -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
        -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
        -DCMAKE_OSX_SYSROOT:STRING=${CMAKE_OSX_SYSROOT}
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DLK_DIR:DIR=${CMAKE_BINARY_DIR}/lk-prefix/src/lk/
        -DCURL_DIR:DIR=${CMAKE_BINARY_DIR}/curl-prefix/src/curl/
        -DCMAKE_INSTALL_PREFIX:DIR=${CMAKE_BINARY_DIR}/DView-prefix/src/DView-install
        -DwxWidgets_EXCLUDE_COMMON_LIBRARIES:BOOL=false
        -DwxWidgets_USE_STATIC:BOOL=true
    )
endif()

# Set up the dependencies for OpenStudioCore
if(BUILD_BOOST)
  list(APPEND OpenStudioCore_DEPENDS Boost)
endif()

if(BUILD_RUBY)
  list(APPEND OpenStudioCore_DEPENDS Ruby)
endif()

if(BUILD_NODE)
  list(APPEND OpenStudioCore_DEPENDS Node)
endif()

if(BUILD_V8)
  list(APPEND OpenStudioCore_DEPENDS V8)
endif()

if (BUILD_OSCORE)
  set(OpenStudioCore_DIR ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build)

  if(MSVC)
    set(OS_BUILD_COMMAND BUILD_COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build" --config $(Configuration) -- /maxcpucount)
  else()
    set(OS_BUILD_COMMAND "")
  endif()

  ExternalProject_Add(OSCore
    DEPENDS ${OpenStudioCore_DEPENDS}
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/openstudiocore
    ${OS_BUILD_COMMAND}
    CMAKE_CACHE_ARGS
      -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
      -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
      -DCMAKE_OSX_SYSROOT:STRING=${CMAKE_OSX_SYSROOT}
      -DBUILD_DOCUMENTATION:BOOL=${BUILD_DOCUMENTATION}
      -DBUILD_CSHARP_BINDINGS:BOOL=${BUILD_CSHARP_BINDINGS}
      -DBUILD_JAVA_BINDINGS:BOOL=${BUILD_JAVA_BINDINGS}
      -DBUILD_V8_BINDINGS:BOOL=${BUILD_V8_BINDINGS}
      -DBUILD_NODE_MODULES:BOOL=${BUILD_NODE_MODULES}
      -DBUILD_PYTHON_BINDINGS:BOOL=${BUILD_PYTHON_BINDINGS}
      -DBUILD_TESTING:BOOL=${BUILD_TESTING}
      -DBUILD_PACKAGE:BOOL=${BUILD_PACKAGE}
      -DBUILD_PAT:BOOL=${BUILD_PAT}
      -DBUILD_WITH_MULTIPLE_PROCESSES:BOOL=${BUILD_WITH_MULTIPLE_PROCESSES}
      -DUSE_PCH:BOOL=${USE_PCH}
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
      -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
      -DCMAKE_MODULE_LINKER_FLAGS:STRING=${CMAKE_MODULE_LINKER_FLAGS}
      -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
      -DBOOST_ROOT:STRING=${BOOST_ROOT}
      -DBOOST_LIBRARYDIR:STRING=${BOOST_LIBRARYDIR}
      -DQT_INSTALL_DIR:PATH=${QT_INSTALL_DIR}
      -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
      -DNODE_BIN_DIR:STRING=${NODE_BIN_DIR}
      -DNODE_INCLUDE_DIR:STRING=${NODE_INCLUDE_DIR}
      -DV8_INCLUDE_DIR:STRING=${V8_INCLUDE_DIR}
      -DDOXYGEN_EXECUTABLE:STRING=${DOXYGEN_EXECUTABLE}
      -DDOXYGEN_DOT_EXECUTABLE:STRING=${DOXYGEN_DOT_EXECUTABLE}
      -DMAXIMIZE_CPU_USAGE:BOOL=${MAXIMIZE_CPU_USAGE}
      -DRUBY_INSTALL_DIR:PATH=${RUBY_INSTALL_DIR}
      INSTALL_COMMAND ""
  )

  #if(BUILD_QT AND MSVC)
  #  file(GLOB dlls ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/bin/*.dll)
  #  foreach(dll ${dlls})
  #    get_filename_component(filename "${dll}" NAME_WE)
  #    ExternalProject_Add_Step(OSCore "Install${filename}ReleaseDLL"
  #      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Release"
  #      DEPENDERS configure
  #    )
  #    ExternalProject_Add_Step(OSCore "Install${filename}DebugDLL"
  #      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Debug"
  #      DEPENDERS configure
  #    )
  #    ExternalProject_Add_Step(OSCore "Install${filename}RelWithDebInfoDLL"
  #      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/RelWithDebInfo"
  #      DEPENDERS configure
  #    )
  #    ExternalProject_Add_Step(OSCore "Install${filename}MinSizeRelDLL"
  #      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/MinSizeRel"
  #      DEPENDERS configure
  #    )
  #  endforeach()
  #endif()

  #if(MSVC)
  #  ExternalProject_Add_Step(OSCore MakeRubyReleaseFolder
  #    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Release
  #    DEPENDERS configure
  #  )
  #  ExternalProject_Add_Step(OSCore MakeRubyDebugFolder
  #    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Debug
  #    DEPENDERS configure
  #  )
  #  ExternalProject_Add_Step(OSCore MakeRubyRelWithDebInfoFolder
  #    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/RelWithDebInfo
  #    DEPENDERS configure
  #  )
  #  ExternalProject_Add_Step(OSCore MakeRubyMinSizeRelFolder
  #    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/MinSizeRel
  #    DEPENDERS configure
  #  )
  #
  #
  #  ExternalProject_Add_Step(OSCore MakeRubyReleaseDLL
  #    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Release/${RUBY_DLL}"
  #    DEPENDERS configure
  #  )
  #  ExternalProject_Add_Step(OSCore MakeRubyDebugDLL
  #    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Debug/${RUBY_DLL}"
  #    DEPENDERS configure
  #  )
  #  ExternalProject_Add_Step(OSCore MakeRubyRelWithDebInfoDLL
  #    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/RelWithDebInfo/${RUBY_DLL}"
  #    DEPENDERS configure
  #  )
  #  ExternalProject_Add_Step(OSCore MakeRubyMinSizeRelDLL
  #    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/MinSizeRel/${RUBY_DLL}"
  #    DEPENDERS configure
  #  )
  #
  #endif()


  set(OPENSTUDIOCORE_BUILD_DIR ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build)
  set(OPENSTUDIOCORE_LIB_DIR ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products)
  set(OPENSTUDIOCORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)
  set(OPENSTUDIOCORE_ROOT_DIR ${CMAKE_SOURCE_DIR}/)

  ###############################################################################
  # Use CPack
  if(BUILD_PACKAGE)
    include(OpenStudioCPack.cmake)
  endif()
  ###############################################################################


  if(BUILD_REGRESSIONS)
    ExternalProject_Add(Regressions
      DEPENDS OSCore
      GIT_REPOSITORY git://github.com/NREL/OpenStudio-resources
      GIT_TAG develop
      CMAKE_CACHE_ARGS
        -DOPENSTUDIO_BUILD_DIR:PATH=${CMAKE_BINARY_DIR}
        -DQT_INSTALL_DIR:PATH=${QT_INSTALL_DIR}
      INSTALL_COMMAND ""
    )
  endif()
endif()
