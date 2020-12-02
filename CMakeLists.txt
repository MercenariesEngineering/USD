cmake_minimum_required(VERSION 3.13)
project(usd)

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    if (WIN32)
        message(FATAL_ERROR "Compiler does not support 64-bit builds. "
                "If you are using Visual Studio, make sure you are in the "
                "x64 command prompt and have specified the Win64 cmake "
                "generator (e.g., -G \"Visual Studio 14 2015 Win64\")")
    else()
        message(FATAL_ERROR "Compiler does not support 64-bit builds.")
    endif()
endif()

#### This part is added to the original file in order to tweak the build for Rumba ##
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
if( DEFINED RUMBA_CONAN_BUILD_INFO_PATH )
    include( "${RUMBA_CONAN_BUILD_INFO_PATH}" )
else()
    include( "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake" )
endif()
conan_basic_setup(TARGETS)

# help Usd build to use our dependencies
if( DEFINED HDF5_ROOT )
# set HDF5_ROOT in the env so FindHDF5.cmake can find it
message( status "Using HDF5_ROOT: ${HDF5_ROOT}" )
set( ENV{HDF5_ROOT} ${HDF5_ROOT} )
endif()

set( Boost_PROGRAM_OPTIONS_FOUND on )
if( UNIX )
    set( Boost_PROGRAM_OPTIONS_LIBRARY "${CONAN_LIB_DIRS_BOOST}/libboost_program_options.so" )
    set( Boost_PYTHON37_LIBRARY "${CONAN_LIB_DIRS_BOOST}/libboost_python37.so" )
    set( OIIO_LOCATION "${CONAN_LIB_DIRS_OPENIMAGEIO}" )
    set( OPENGL_gl_LIBRARY "${CONAN_LIB_DIRS_GLU}/libGLU.a" )
    include_directories( "${CONAN_INCLUDE_DIRS_GLU}" )
else()
    if( NOT DEFINED CONAN_LIB_DIRS_BOOST )
        set( CONAN_LIB_DIRS_BOOST ${CONAN_LIB_DIRS_BOOST_RELEASE} )
    endif()
    if( NOT DEFINED CONAN_LIB_DIRS_GLEW )
        set( CONAN_LIB_DIRS_GLEW ${CONAN_LIB_DIRS_GLEW_RELEASE} )
    endif()
    if( NOT DEFINED CONAN_DEFINES )
        set( CONAN_DEFINES ${CONAN_DEFINES_RELEASE} )
    endif()
    set( Boost_PROGRAM_OPTIONS_FOUND on )
    set( Boost_PROGRAM_OPTIONS_LIBRARY "${CONAN_LIB_DIRS_BOOST}/boost_program_options.lib" )
    set( Boost_PYTHON37_LIBRARY "${CONAN_LIB_DIRS_BOOST}/boost_python37.lib" )
    set( GLEW_LIBRARY "${CONAN_LIB_DIRS_GLEW}/glew32.lib" )
    add_definitions(${CONAN_DEFINES})
endif()

set( HDF5_USE_STATIC_LIBRARIES on )
set( Boost_USE_STATIC_LIBS on )

# activate the options we need for Rumba
set( PXR_BUILD_OPENCOLORIO_PLUGIN on )
set( PXR_BUILD_OPENIMAGEIO_PLUGIN on )
set( PXR_BUILD_MATERIALX_PLUGIN on )
set( PXR_BUILD_ALEMBIC_PLUGIN on )
set( PXR_ENABLE_HDF5_SUPPORT on )
set( PXR_ENABLE_PTEX_SUPPORT on )
set( PXR_BUILD_USD_IMAGING on )
set( PXR_ENABLE_GL_SUPPORT on )
set( PXR_BUILD_IMAGING on )
set( PXR_USE_PYTHON_3 on )

# deactivate unused components / features
set( PXR_ENABLE_OPENVDB_SUPPORT off )
set( PXR_BUILD_HOUDINI_PLUGIN off )
set( PXR_BUILD_EMBREE_PLUGIN off )
set( PXR_BUILD_DOCUMENTATION off )
set( PXR_BUILD_KATANA_PLUGIN off )
set( PXR_BUILD_DRACO_PLUGIN off )
set( PXR_BUILD_PRMAN_PLUGIN off )
set( PXR_ENABLE_OSL_SUPPORT off )

# options that might change in the future
set( PXR_ENABLE_PYTHON_SUPPORT on )
set( PXR_BUILD_USDVIEW on )
set( PXR_BUILD_TESTS off )

# tweak compile options
if( UNIX ) 
    add_compile_options( ${CONAN_DEFINES} )
    add_compile_options( -DUSD_FORCE_GNU_STL_EXTENSIONS )
    # there are too much warnings, disable them completely to not flood the console output
    add_compile_options(-Wno-everything -Wno-inconsistent-missing-override -Wno-potentially-evaluated-exp -Wno-shadow -Wno-shadow-field -Wno-shadow-uncaptured-local -Wno-unused-private-field -Wno-expansion-to-defined -Wno-unneeded-internal-declaration -Wno-unused-variable )
endif()
#### end of tweaks ##

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_SOURCE_DIR}/cmake/defaults
                      ${CMAKE_SOURCE_DIR}/cmake/modules
                      ${CMAKE_SOURCE_DIR}/cmake/macros)

include(Options)
include(ProjectDefaults)
include(Packages)

# This has to be defined after Packages is included, because it relies on the
# discovered path to the python executable.
set(PXR_PYTHON_SHEBANG "${PYTHON_EXECUTABLE}" 
    CACHE 
    STRING
    "Replacement path for Python #! line."
)

# CXXDefaults will set a variety of variables for the project.
# Consume them here. This is an effort to keep the most common
# build files readable.
include(CXXDefaults)
add_definitions(${_PXR_CXX_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${_PXR_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")

include(Public)

pxr_toplevel_prologue()

add_subdirectory(pxr)
add_subdirectory(extras)

if (PXR_BUILD_PRMAN_PLUGIN)
    set(prmanPluginPath "third_party/renderman-${RENDERMAN_VERSION_MAJOR}")

    get_filename_component(prmanPluginAbsPath "${prmanPluginPath}" ABSOLUTE)
    if (EXISTS "${prmanPluginAbsPath}")
        add_subdirectory("${prmanPluginPath}")
    else()
        message(FATAL_ERROR "No RenderMan plugin available for "
                "RenderMan ${RENDERMAN_VERSION_MAJOR}")
    endif()
endif()

if (PXR_BUILD_DOCUMENTATION)
    pxr_build_documentation()
endif()   

pxr_toplevel_epilogue()
