# CMakeList.txt : CMake project for arkanoid, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
cmake_policy(SET CMP0042 OLD)
set(CMAKE_MACOSX_RPATH 1)

project(arkanoid)

if(MSVC)
	# fix __cplusplus macro for MSVC
	add_compile_options(
		"/Zc:__cplusplus"
		"/std:c++latest"
	)
else()
	set(CMAKE_CXX_STANDARD 17)
endif()

if ("${MODULE_NAME}" STREQUAL "")
	message("MODULE_NAME is empty. Set to 'arkanoid'")
	set(MODULE_NAME arkanoid)
else()
	message("MODULE_NAME is set externally to '${MODULE_NAME}'")
endif()

option(BUILD_SHARED_LIBRARY "Build as a shared library" OFF)
option(${MODULE_NAME}_INHERIT_PARENT_SRC "Adds all parent's source files" OFF)
option(${MODULE_NAME}_INHERIT_PARENT_LIBS "Adds all parent's libraries" ON)
option(CMAKE_VERBOSE "Additional logs will be shown" OFF)
option(${MODULE_NAME}_CMAKE_VERBOSE "Additional logs will be shown" OFF)

if (CMAKE_VERBOSE)
	set(${MODULE_NAME}_CMAKE_VERBOSE ON)
endif()

if (${MODULE_NAME}_CMAKE_VERBOSE)
	message("${MODULE_NAME}: ${MODULE_NAME}_INHERIT_PARENT_SRC is ${${MODULE_NAME}_INHERIT_PARENT_SRC}")
	message("${MODULE_NAME}: ${MODULE_NAME}_INHERIT_PARENT_LIBS is ${${MODULE_NAME}_INHERIT_PARENT_LIBS}")
endif()

if (LOG_ON)
	add_compile_definitions("LOG_ON")
	message("${MODULE_NAME}: LOG_ON is defined")
else()
	message("${MODULE_NAME}: LOG_ON is not defined")
endif()
if (LOG_LEVEL_VERBOSE)
	add_compile_definitions("LOG_LEVEL_VERBOSE")
	message("${MODULE_NAME}: LOG_LEVEL_VERBOSE is defined")
else()
	message("${MODULE_NAME}: LOG_LEVEL_VERBOSE is not defined")
endif ()
if (LOG_LEVEL_DEBUG)
	add_compile_definitions("LOG_LEVEL_DEBUG")
	message("${MODULE_NAME}: LOG_LEVEL_DEBUG is defined")
else()
	message("${MODULE_NAME}: LOG_LEVEL_DEBUG is not defined")
endif()

# Inherit parent libs logic
set(PARENT_LIBS ${PARENT_LIBS} ${LIBS})
set(PARENT_LIBS ${PARENT_LIBS} PARENT_SCOPE)
if (${MODULE_NAME}_INHERIT_PARENT_LIBS)
	set(LIBS ${PARENT_LIBS})
	if (${MODULE_NAME}_CMAKE_VERBOSE)
		message("${MODULE_NAME} PARENT_LIBS: ${PARENT_LIBS}")
	endif()
endif()
# End of Inherit parent libs logic

# set build folder name variable for any dependency
if (CMAKE_CXX_COMPILER EQUAL "g++")
	set(BUILD_DIR_NAME "Build-g++")
else()
	set(BUILD_DIR_NAME "Build-cmake")
endif()

# set deps variables
set(arkanoid_deps "$ENV{arkanoid_deps}")
set(arkanoid_deps "${arkanoid_deps}" PARENT_SCOPE)
if ("${DEPS}" STREQUAL "")
	set(DEPS ${arkanoid_deps})
endif()
if ("${DEPS}" STREQUAL "")
	set(DEPS "$ENV{build_deps}")
endif()
if ("${DEPS}" STREQUAL "")
	message(SEND_ERROR "${MODULE_NAME}: ERROR: DEPS is not specified. Take the default value")
	set(DEPS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
else()
	message("${MODULE_NAME}: DEPS: ${DEPS}")
endif()
set(DEPS ${DEPS} PARENT_SCOPE)

# Set and add external includes
if (NOT "${INCLUDES}" STREQUAL "")
	message("${MODULE_NAME}: Add extra includes: ${INCLUDES}")
else()
	set(INCLUDES $ENV{arkanoid_includes})
endif()

# SDL dependency
set(SDL_DIR ${DEPS}/SDL)
set(SDL_BUILD_DIR ${SDL_DIR}/build)
add_subdirectory(${SDL_DIR} ${SDL_BUILD_DIR} EXCLUDE_FROM_ALL)
set(CUSTOM_INCLUDES ${CUSTOM_INCLUDES} ${SDL_DIR}/include)

# Set arkanoid_INCLUDES global variable
set(arkanoid_INCLUDES ${arkanoid_INCLUDES}
	${CUSTOM_INCLUDES}
	${INCLUDES}
	${CMAKE_CURRENT_SOURCE_DIR}/src
)
set(arkanoid_INCLUDES ${arkanoid_INCLUDES} PARENT_SCOPE)

# include all directories
if (NOT "${arkanoid_INCLUDES}" STREQUAL "")
	message("arkanoid_INCLUDES generated: ${arkanoid_INCLUDES}")
	include_directories("${arkanoid_INCLUDES}")
endif()

# Fetch CPP from external includes
if (NOT "${INCLUDES}" STREQUAL "")
	message("Project: External INCLUDES variable: ${INCLUDES}")
	foreach(items ${INCLUDES})
		file(GLOB_RECURSE INCLUDES_SRC "${items}/*.cpp")
	endforeach()
	message("Project: External INCLUDES sources: ${INCLUDES_SRC}")
endif()

# set up sources
file(GLOB SRC "*.cpp" "src/*.cpp")
file(GLOB HEADERS "*.h" "src/*.h")
set(SRC ${SRC} ${INCLUDES_SRC})

# Inherit parent sources logic
set(PARENT_SRC ${PARENT_SRC} ${SRC})
set(PARENT_SRC ${PARENT_SRC} PARENT_SCOPE)

if (${MODULE_NAME}_INHERIT_PARENT_SRC)
	set(SRC ${PARENT_SRC})
	if (${MODULE_NAME}_CMAKE_VERBOSE)
		message("${MODULE_NAME} PARENT_SRC: ${PARENT_SRC}")
	endif()
endif()
# End of Inherit parent sources logic

# add executable
if(BUILD_LIBRARY)
	message("BUILD_LIBRARY")
	add_library (${MODULE_NAME} ${SRC} ${HEADERS} main.cpp)
elseif(BUILD_SHARED_LIBRARY)
	message("BUILD_SHARED_LIBRARY")
	add_library (${MODULE_NAME} SHARED ${SRC} ${HEADERS} main.cpp)
else()
	message("BUILD_EXECUTABLE")
	add_executable (${MODULE_NAME} ${SRC} ${HEADERS} main.cpp)
endif()

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications  
message("TARGETS: ${BUILDSYSTEM_TARGETS}")
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(${MODULE_NAME} PRIVATE SDL2::SDL2main)
elseif(TARGET SDL2main)
	target_link_libraries(${MODULE_NAME} PRIVATE SDL2main)
else()
	error("No SDL2main target found!")
endif()
# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(${MODULE_NAME} PRIVATE SDL2)

# link libraries
# Link all librariles including inherited
set(cnt 1)
foreach(item ${LIBS})
	if (${cnt} EQUAL 1)
		if (${MODULE_NAME}_CMAKE_VERBOSE)	
			message("${MODULE_NAME}: Link library '${item}'")
		endif()
		set(lib_name ${item})
	elseif(${cnt} EQUAL 2)
		if (${MODULE_NAME}_CMAKE_VERBOSE)
			message("${MODULE_NAME}: Library '${lib_name}' path: ${item}")
		endif()
		set(lib_path ${item})
	elseif(${cnt} EQUAL 3)
		if (${MODULE_NAME}_CMAKE_VERBOSE)
			message("${MODULE_NAME}: Library '${lib_name}' binary path: ${item}")
		endif()
		#add_subdirectory("${lib_path}" "${item}")
		target_link_libraries(${MODULE_NAME} ${lib_name})
	endif()
	MATH(EXPR cnt "${cnt}+1")
endforeach()

if (WIN32)
	add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${MODULE_NAME}> $<TARGET_FILE_DIR:${MODULE_NAME}>
	COMMAND_EXPAND_LISTS
	)
endif()
