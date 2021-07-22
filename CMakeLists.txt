cmake_minimum_required( VERSION 3.2 )
cmake_policy(SET CMP0072 NEW) # OpenGL Policy to avoid warning

function( copy_resources targetName )
    set( COPY_RES )

    if( MSVC )
        # collect a list of all the DLLs to copy
        #

        file( GLOB files "visual/libs/bin/*.dll" )
        set( COPY_RES ${COPY_RES} ${files} )
    endif( MSVC )

    # Copy any files from res folder
    file( GLOB files "res/*.*" )
    set( COPY_RES ${COPY_RES} ${files} )

    foreach( RES_FILE ${COPY_RES} )
        add_custom_command( TARGET ${targetName} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${RES_FILE}" $<TARGET_FILE_DIR:${targetName}> )
    endforeach()
endfunction()

project( SimulationWorld )

if( ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" )
    message( STATUS "Found Clang" )
    # no built in variable for Clang so make one
    set (CMAKE_COMPILER_IS_CLANG TRUE)

    if( "${CMAKE_CXX_COMPILER_ID}" MATCHES "AppleClang" )
        message( STATUS "Compiler: Apple Clang" )
        # no built in variable for Apple Clang so make one
        set (CMAKE_COMPILER_IS_APPLE_CLANG TRUE)
    endif()
endif()

if( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG )
    set( CMAKE_COMPILER_IS_GNUCXX_LIKE TRUE )
endif()

# determine bitness
set( IS_64_BIT FALSE )

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set( IS_64_BIT TRUE )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

# determine processor architecture directory
if( WIN32 )
    # directory names for 32 and 64 bit
    if( IS_64_BIT )
        set( ARCH "x64" )
        message(FATAL_ERROR "Windows only supports 32 bit version of the project, run again cmake and select 32 bit platform.")
    else()
        set( ARCH "x86" )
    endif()
else()
    # on non windows systems use processor as architecture
    set( ARCH ${CMAKE_SYSTEM_PROCESSOR} )
endif()

message( STATUS "Using architecture ${ARCH}" )

# determine compiler directory
if( MSVC )
    set ( COMPILER "vc14" )
elseif(APPLE)
    set ( COMPILER "xcode" )
else()
    set ( COMPILER "gnu" )
endif()

message( STATUS "Using Compiler IDE: ${COMPILER}" )

# determine platform directory
if( MSVC )
  set ( PLATFORM "windows" )
elseif(APPLE)
    set ( PLATFORM "Apple" )
else()
  set ( PLATFORM "linux" )
endif()

message( STATUS "Using platform ${PLATFORM}" )


if( MSVC )
    message( STATUS "Adding freeglut includes ..." )

    include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/visual/libs/include )
    set( LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/visual/libs/lib )

    message ( STATUS "Visual Studio compiler" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_WARNINGS" )
endif()

set(CMAKE_CXX_STANDARD 11)

set( ALL_FILES )

set( MAIN
    src/main/includes.h
    src/main/main.cpp
)
source_group( "main" FILES ${MAIN} )
set( ALL_FILES ${ALL_FILES} ${MAIN} )

set( Framework
    src/framework/application.cpp
    src/framework/application.h
    src/framework/framework.cpp
    src/framework/framework.h
    src/framework/image.cpp
    src/framework/image.h
    src/framework/utils.cpp
    src/framework/utils.h
)
source_group( "framework" FILES ${Framework} )
set( ALL_FILES ${ALL_FILES} ${Framework} src/simulation_objects/food.cpp src/simulation_objects/food.h src/simulation_objects/creature.cpp src/simulation_objects/creature.h src/simulation_objects/simulation_includes.h src/simulation_objects/map.cpp src/simulation_objects/map.h src/simulation_objects/cell_information.cpp src/simulation_objects/cell_information.h src/simulation_objects/objects_hitboxes/rectangle.cpp src/simulation_objects/objects_hitboxes/rectangle.h src/simulation_objects/objects_hitboxes/screen_object.cpp src/simulation_objects/objects_hitboxes/screen_object.h src/simulation_objects/simulation_constants.h)

include_directories( src )
include_directories( src/main )
include_directories( src/framework )

set( ProjectName "SimulationWorld")

message( STATUS "Creating SimulationWorld project." )
add_executable( ${ProjectName} ${ALL_FILES} )
copy_resources( ${ProjectName} )

if( MSVC )
    target_link_libraries( ${ProjectName} opengl32 )
    target_link_libraries( ${ProjectName} glu32 )
    target_link_libraries( ${ProjectName} ${LIB_DIR}/glut32.lib )
    target_link_libraries( ${ProjectName} ${LIB_DIR}/glew32.lib )
    target_link_libraries( ${ProjectName} ${LIB_DIR}/SDL2.lib )
    target_link_libraries( ${ProjectName} ${LIB_DIR}/SDL2main.lib )
elseif( CMAKE_COMPILER_IS_GNUCXX_LIKE )
    find_package(OpenGL 3.2 REQUIRED)

    include_directories(${OPENGL_INCLUDE_DIR})
    target_link_libraries(${ProjectName} ${OPENGL_LIBRARIES})

    message(STATUS "OPENGL_INCLUDE_DIR: ${OPENGL_INCLUDE_DIR}")
    message(STATUS "OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")

    find_package(GLUT REQUIRED)

    include_directories(${GLUT_INCLUDE_DIR})
    target_link_libraries(${ProjectName} ${GLUT_LIBRARY})

    message(STATUS "GLUT_INCLUDE_DIR: ${GLUT_INCLUDE_DIR}")
    message(STATUS "GLUT_LIBRARY: ${GLUT_LIBRARY}")

    if(APPLE)
        set(SDL2_INCLUDE_DIRS "/opt/local/include")
        set(SDL2_LIBRARIES "/opt/local/lib/libSDL2.dylib")

        if(EXISTS "${SDL2_LIBRARIES}")
            message(STATUS "SDL installed using mac ports")
        else()
            set(SDL2_INCLUDE_DIRS "/usr/local/include")
            set(SDL2_LIBRARIES "/usr/local/lib/libSDL2.dylib")

            if(NOT EXISTS "${SDL2_LIBRARIES}")
                message(FATAL_ERROR "You must install sdl libraries using homebrew or mac ports.")
            endif()

            message(STATUS "SDL installed using homebrew")
		endif()			
    else()
        find_package(GLEW REQUIRED)

        include_directories(${GLEW_INCLUDE_DIR})
        target_link_libraries(${ProjectName} ${GLEW_LIBRARY})

        message(STATUS "GLEW_INCLUDE_DIR: ${GLEW_INCLUDE_DIR}")
        message(STATUS "GLEW_LIBRARY: ${GLEW_LIBRARY}")

        include(FindPkgConfig)
        PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    endif()

    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(${ProjectName} ${SDL2_LIBRARIES})

    message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
    message(STATUS "SDL2_LIBRARY: ${SDL2_LIBRARIES}")
endif()

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(${ProjectName} ${PYTHON_LIBRARIES})