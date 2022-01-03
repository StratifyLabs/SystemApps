#Copy this file to the application project folder as CMakeLists.txt
cmake_minimum_required (VERSION 3.12)

if(NOT SOS_SDK_PATH)
	set(SOS_SDK_PATH $ENV{SOS_SDK_PATH})
endif()

set(CMAKE_MODULE_PATH ${SOS_SDK_PATH}/cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_MODULE_PATH}/sos-toolchain.cmake)
include(sos-sdk)

set(RAM_SIZE 98304)
project(FileSelect CXX C)

sos_sdk_app_target(RELEASE ${PROJECT_NAME} "" release ${SOS_ARCH})
sos_sdk_add_subdirectory(SOURCELIST src)

if(SOS_IS_LINK)
	if(DESIGNLAB_DIRECTORY AND NOT IS_DIRECTORY ${DESIGNLAB_DIRECTORY})
		message(STATUS "DESIGNLAB_DIRECTORY is ${DESIGNLAB_DIRECTORY}")
		message(FATAL_ERROR "DESIGNLAB_DIRECTORY must be set to the design lab directory")
	endif()
	sos_sdk_add_out_of_source_directory(DESIGNLAB_SOURCELIST ${DESIGNLAB_DIRECTORY} designlab)
else()
	set(DESIGNLAB_SOURCELIST "")
endif()

message(STATUS "Stratify API Test Executable ${RELEASE_TARGET}")
add_executable(${RELEASE_TARGET})
target_sources(${RELEASE_TARGET}
	PRIVATE
	${SOURCELIST}
	${DESIGNLAB_SOURCELIST}
	${CMAKE_CURRENT_SOURCE_DIR}/sl_settings.json
	)

target_compile_options(${RELEASE_TARGET}
	PRIVATE
	-Os
	)

if(SOS_IS_LINK)
	target_include_directories(${RELEASE_TARGET}
		PRIVATE
		${DESIGNLAB_DIRECTORY}/..
		)
endif()

set_property(TARGET ${RELEASE_TARGET} PROPERTY CXX_STANDARD 17)

set(DEPENDENCIES DesignAPI FsAPI SysAPI VarAPI)
if(SOS_IS_ARM)
	set(DEPENDENCIES ${DEPENDENCIES} StratifyOS_crt)
endif()

sos_sdk_app_add_arch_targets("${RELEASE_OPTIONS}" "${DEPENDENCIES}" ${RAM_SIZE})

target_compile_options(${RELEASE_TARGET}
	PRIVATE
	-Os
	)

