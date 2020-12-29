cmake_minimum_required (VERSION 3.8)
project(jatapt VERSION 0.2.0.1) # MAJOR.MINOR.PATCH.TWEAK

SET(VERSION_TYPE "IDB") # "(I)n (D)evelopment (B)uild"
#SET(VERSION_TYPE "RCB") # "(R)elease (C)andidate (B)uild"
#SET(VERSION_TYPE "PVB") # "(P)re(V)iew (B)uild"

SET(GAME_STRING "JATAPT")
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d")

set(PROJECT_STRING "${GAME_STRING} [${CMAKE_SYSTEM_NAME} ${VERSION_TYPE} ${CMAKE_PROJECT_VERSION}]")

include_directories("lib")
include_directories("src")
include_directories("include")

file(GLOB_RECURSE SRC
    "src/*"
)

if(WIN32)
	add_executable (${PROJECT_NAME} WIN32 ${SRC})
else()
	add_executable (${PROJECT_NAME} ${SRC})
endif()

#paths for build outputs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/debug/build_files)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/debug/)

configure_file(${PROJECT_SOURCE_DIR}/src/Alcubierre/build_info.h.in ${PROJECT_SOURCE_DIR}/src/Alcubierre/build_info.h)
configure_file(${PROJECT_SOURCE_DIR}/src/JATAPT.rc.in ${PROJECT_SOURCE_DIR}/src/JATAPT.rc)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_STRING})



############################ Library Compiling and/or Linking ############################

if(WIN32)
	#spooky preprocessor and linker definitions
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc /std:c++17 /d2FH4 -D_WIN32_WINNT=0x0601")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows")
	#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

	#microsoft windows libs: make sure not to include the ".lib" as crosscompilers use the name of the lib as a symbolic link
	#and will get confused and not find the lib if you put the extension at the end. also these need to be lowercase or they will not link
	target_link_libraries(${PROJECT_NAME} gdi32 kernel32 psapi ws2_32 dnsapi)

	#libs for stuff that is crappy, slow or inconvience to compile every time
	file(GLOB_RECURSE WINDOWS_STATIC_LIBS ${PROJECT_SOURCE_DIR}/lib/win64/*.lib)
	target_link_libraries(${PROJECT_NAME} ${WINDOWS_STATIC_LIBS})

  #tronkko/dirent
  add_subdirectory(${PROJECT_SOURCE_DIR}/lib/src/dirent-master)
  include_directories(${PROJECT_SOURCE_DIR}/lib/src/dirent-master/include)
endif(WIN32)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fexceptions -g -Wall -Wwrite-strings")

	#best to not use these to often and just require the user to use a package manager to get them
	file(GLOB_RECURSE LINUX_STATIC_LIBS "${PROJECT_SOURCE_DIR}/lib/linux/*")
	target_link_libraries(${PROJECT_NAME} ${LINUX_STATIC_LIBS})

	set(GAMENETWORKINGSOCKETS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(GAMENETWORKINGSOCKETS_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	add_subdirectory(${PROJECT_SOURCE_DIR}/lib/src/GameNetworkingSockets)
	target_link_libraries(${PROJECT_NAME} GameNetworkingSockets)
endif(UNIX)

IF(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fexceptions -g -Wall")
endif(APPLE)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_DIR "${PROJECT_SOURCE_DIR}/lib/src/glfw")
include_directories("${GLFW_DIR}/include")
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} glfw)

#glad (OpenGL 4.0, No Extensions) Might change this for linux builds as my ubuntu x230 crashes on init with gl4.0
set(GLAD_DIR "${PROJECT_SOURCE_DIR}/lib/src/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
include_directories(${GLAD_DIR}/include/)
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

#lz4
set(LZ4_DIR "${PROJECT_SOURCE_DIR}/lib/src/lz4")
file(GLOB LZ4_SRC
	${LZ4_DIR}/*.h
	${LZ4_DIR}/*.c
)
add_library("lz4" ${LZ4_SRC})
target_include_directories("lz4" PRIVATE "${LZ4_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${LZ4_DIR}")
target_link_libraries(${PROJECT_NAME} "lz4")

#imgui-docking-test
set(IMGUI_DIR "${PROJECT_SOURCE_DIR}/lib/src/imgui-docking")
file(GLOB IMGUI_SRC
    ${IMGUI_DIR}/*.cpp
	${IMGUI_DIR}/*.h
)
add_library("imgui" ${IMGUI_SRC})
target_include_directories("imgui" PRIVATE ${IMGUI_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_DIR})
target_link_libraries(${PROJECT_NAME} "imgui")

#mandreyel/mio
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/src/mio)
target_link_libraries(${PROJECT_NAME} mio)
include_directories(${PROJECT_SOURCE_DIR}/lib/src/mio/include)

#copy windows precompiled dlls into output folder
if(WIN32 OR SYNTHETIC)
	file(GLOB WINDOWS_DLLS "${PROJECT_SOURCE_DIR}/output/runtime/requirements/*" )
	file(COPY ${WINDOWS_DLLS} DESTINATION ${EXECUTABLE_OUTPUT_PATH})
endif()

#graeme-hill/crossguid
set(CROSSGUID_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/src/crossguid-master)
target_link_libraries(${PROJECT_NAME} crossguid)

#for some reason glfw3 wants to compile everything including macos and linux stuff even when being built from win32
#(future tom here) apparently if you compile glfw3 on linux it overites some sort of db file that then makes glfw
#think that it is being build on linux.. even when being build with no linux pre-processor definitions
#so either don't include that on a commit or add the linux specific files to the .gitignore

if(UNIX OR SYNTHETIC)
	#i'm sure there is a better way to do this but .... ya know.
	if(NOT APPLE)
		set(CMAKE_C_FLAGS "-static-libstdc++ -static-libgcc -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")
		set(CMAKE_CXX_FLAGS "-std=c++17 -static-libstdc++ -static-libgcc -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")
	endif(NOT APPLE)
	add_definitions(-DLINUX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif(UNIX)
