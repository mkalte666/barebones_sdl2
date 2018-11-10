#    CMakeLists.txt 
#
#    Copyright (C) 2018  Malte Kießling
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.

# some cmake default foo 
cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)

# disable in-source build. found at https://stackoverflow.com/questions/1208681/with-cmake-how-would-you-disable-in-source-builds
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(barebones_sdl2)

# some more debug/release defines because im paranoid 
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG -DDEBUGGING -DNRELEASE)
else()
    add_definitions(-DNDEBUG -DNDEBUGGING -DRELEASE)
endif()

# put stuff into "bin" and "lib" because we can 
# sauce: https://stackoverflow.com/questions/7747857/in-cmake-how-do-i-work-around-the-debug-and-release-directories-visual-studio-2
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/buildpackage/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/buildpackage/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/buildpackage/bin)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

# this is for the following find package. uncomment what you need. 
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_TTF REQUIRED)
#find_package(SDL2_IMAGE REQUIRED)
#find_package(SDL2_MIXER REQUIRED)

# includes. glm will stay, but should not cause problems. i just use it alot. uncomment what you need
include_directories(
    ${GLM_INCLUDE_DIR} 
    ${SDL2_INCLUDE_DIR} 
    ${SDL2_TTF_INCLUDE_DIR} 
    #${SDL2_IMAGE_INCLUDE_DIR}
    #${SDL2_MIXER_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/src 
    )

# sources go here 
set(ALL_SOURCES 
    src/main.cpp
)

# more the usual. create executable, add libraries 
add_executable(barebones_sdl2
    ${ALL_SOURCES}
    )

target_link_libraries(barebones_sdl2 
    ${SDL2_LIBRARY} 
    ${SDL2MAIN_LIBRARY} 
    ${SDL2_TTF_LIBRARY}
    #${SDL2_IMAGE_LIBRARY}
    #${SDL2_MIXER_LIBRARY}
    )

# now the tricky part: we need both the sdl build stuff *and* the whole project tree 
# however there are two problems: first, we dont want to copy everything every time 
# second, for the sdl2 libs, we need to know if we are 32 or 64 bit. 

# first, the copy all bullshit i came to like 
file(GLOB_RECURSE srcdir RELATIVE ${CMAKE_SOURCE_DIR} "*")
foreach(f ${srcdir})
    set(file_target ${CMAKE_BINARY_DIR}/buildpackage/${f})
    configure_file(${f} ${file_target})
endforeach(f)

# is there a better way? i dont know. found at http://www.saoe.net/blog/778/
if (${CMAKE_SIZEOF_VOID_P} EQUAL "8")
    set (PROJECT_ARCHITECTURE "x64")
else ()
    set (PROJECT_ARCHITECTURE "x86")
endif ()

# copy sdl2 stuff. uncomment what you need
add_custom_command(TARGET barebones_sdl2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SDL2_PATH}/lib/${PROJECT_ARCHITECTURE}/"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_custom_command(TARGET barebones_sdl2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SDL2_TTF_PATH}/lib/${PROJECT_ARCHITECTURE}/"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#add_custom_command(TARGET barebones_sdl2 POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${SDL2_IMAGE_PATH}/lib/${PROJECT_ARCHITECTURE}/"
#        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#add_custom_command(TARGET barebones_sdl2 POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${SDL2_MIXER_PATH}/lib/${PROJECT_ARCHITECTURE}/"
#        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})