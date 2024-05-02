# ///////////////////////////////////////////////////////////////////////////
# ===========================================================================
cmake_minimum_required (VERSION 3.8)

# ===========================================================================
project("AppPrj")

message(STATUS "CMake Evn")
message(STATUS "CMAKE_CXX_COMPILER_ID     : ${CMAKE_CXX_COMPILER_ID}"     )
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_COMPILER        : ${CMAKE_CXX_COMPILER}"        )
message(STATUS "CMAKE_CURRENT_SOURCE_DIR  : ${CMAKE_CURRENT_SOURCE_DIR}"  )
message(STATUS "CMAKE_INSTALL_PREFIX      : ${CMAKE_INSTALL_PREFIX}"      )
message(STATUS "PROJECT_BINARY_DIR        : ${PROJECT_BINARY_DIR}"        )
message(STATUS "PROJECT_SOURCE_DIR        : ${PROJECT_SOURCE_DIR}"        )





# ///////////////////////////////////////////////////////////////////////////
# ===========================================================================
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")

 #include_directories(App 
#	$ENV{UniversalCRTSdkDir}/Include/ucrt
#)





# ///////////////////////////////////////////////////////////////////////////
# ===========================================================================
add_executable(App WIN32
	"${CMAKE_CURRENT_SOURCE_DIR}/source/Main.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/Main.hpp"
)

target_compile_definitions(App PUBLIC UNICODE _UNICODE)
 




# ///////////////////////////////////////////////////////////////////////////
# ===========================================================================
target_include_directories(App PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/library
)

target_include_directories(App PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/library/wxx/include
)

target_link_directories(App PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/library/vld/lib/Win64
)

target_include_directories(App PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_precompile_headers(App PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/source/stdafx.h"
)

# ===========================================================================
target_sources(App PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/source/stdafx.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/stdafx.cpp"
)

target_sources(App PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/resource.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/Application.rc"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/PerMonitorV2.manifest"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/Application.ico"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/toolbar.bmp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/toolbar16.bmp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/toolbar24.bmp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/wxx/SDtop.bmp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/wxx/SDbottom.bmp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/wxx/SDleft.bmp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/wxx/SDright.bmp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/wxx/SDmiddle.bmp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/wxx/SDcenter.bmp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/wxx/SplitH.cur"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/res/wxx/Splitv.cur"
)

