# CMakeList.txt : CMake project for arkanoid, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(arkanoid)

if(MSVC)
	# fix __cplusplus macro for MSVC
	add_compile_options(
		"/Zc:__cplusplus"
		"/std:c++latest"
	)
else()
	set(CMAKE_CXX_STANDARD 20)
endif()

option(arkanoid_INHERIT_PARENT_SRC "Adds all parent's source files" OFF)
option(arkanoid_INHERIT_PARENT_LIBS "Adds all parent's libraries" ON)
option(CMAKE_VERBOSE "Additional logs will be shown" OFF)
option(arkanoid_CMAKE_VERBOSE "Additional logs will be shown" OFF)

if (CMAKE_VERBOSE)
	set(arkanoid_CMAKE_VERBOSE ON)
endif()

if (arkanoid_CMAKE_VERBOSE)
	message("arkanoid: arkanoid_INHERIT_PARENT_SRC is ${arkanoid_INHERIT_PARENT_SRC}")
	message("arkanoid: arkanoid_INHERIT_PARENT_LIBS is ${arkanoid_INHERIT_PARENT_LIBS}")
endif()

if (LOG_ON)
	add_compile_definitions("LOG_ON")
	message("arkanoid: LOG_ON is defined")
else()
	message("arkanoid: LOG_ON is not defined")
endif()
if (LOG_LEVEL_VERBOSE)
	add_compile_definitions("LOG_LEVEL_VERBOSE")
	message("arkanoid: LOG_LEVEL_VERBOSE is defined")
else()
	message("arkanoid: LOG_LEVEL_VERBOSE is not defined")
endif ()
if (LOG_LEVEL_DEBUG)
	add_compile_definitions("LOG_LEVEL_DEBUG")
	message("arkanoid: LOG_LEVEL_DEBUG is defined")
else()
	message("arkanoid: LOG_LEVEL_DEBUG is not defined")
endif()

# Inherit parent libs logic
set(PARENT_LIBS ${PARENT_LIBS} ${LIBS})
set(PARENT_LIBS ${PARENT_LIBS} PARENT_SCOPE)
if (arkanoid_INHERIT_PARENT_LIBS)
	set(LIBS ${PARENT_LIBS})
	if (arkanoid_CMAKE_VERBOSE)
		message("arkanoid PARENT_LIBS: ${PARENT_LIBS}")
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
	message(SEND_ERROR "arkanoid: ERROR: DEPS is not specified. Take the default value")
	set(DEPS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
else()
	message("arkanoid: DEPS: ${DEPS}")
endif()
set(DEPS ${DEPS} PARENT_SCOPE)

# Set and add external includes
if (NOT "${INCLUDES}" STREQUAL "")
	message("arkanoid: Add extra includes: ${INCLUDES}")
else()
	set(INCLUDES $ENV{arkanoid_includes})
endif()

# SDL dependency
set(SDL_DIR ${DEPS}/SDL)
add_subdirectory(${SDL_DIR} ${SDL_DIR}/build EXCLUDE_FROM_ALL)
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
if (arkanoid_INHERIT_PARENT_SRC)
	set(SRC ${PARENT_SRC})
	if (arkanoid_CMAKE_VERBOSE)
		message("arkanoid PARENT_SRC: ${PARENT_SRC}")
	endif()
endif()
# End of Inherit parent sources logic

# add executable
add_executable (arkanoid ${SRC} ${HEADERS} main.cpp)
# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications  
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(arkanoid PRIVATE SDL2::SDL2main)
endif()
# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(arkanoid PRIVATE SDL2::SDL2)

# link libraries
# Link all librariles including inherited
set(cnt 1)
foreach(item ${LIBS})
	if (${cnt} EQUAL 1)
		if (arkanoid_CMAKE_VERBOSE)	
			message("arkanoid: Link library '${item}'")
		endif()
		set(lib_name ${item})
	elseif(${cnt} EQUAL 2)
		if (arkanoid_CMAKE_VERBOSE)
			message("arkanoid: Library '${lib_name}' path: ${item}")
		endif()
		set(lib_path ${item})
	elseif(${cnt} EQUAL 3)
		if (arkanoid_CMAKE_VERBOSE)
			message("arkanoid: Library '${lib_name}' binary path: ${item}")
		endif()
		#add_subdirectory("${lib_path}" "${item}")
		target_link_libraries(arkanoid ${lib_name})
	endif()
	MATH(EXPR cnt "${cnt}+1")
endforeach()
