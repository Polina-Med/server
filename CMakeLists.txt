project(afar_server)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
enable_language(CXX)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

SET(VISA_INCLUDES $ENV{AG_HOME}/include)
SET(VISA_LIBS $ENV{AG_HOME}/LIB/MSC)

SET(IVI_INCLUDES $ENV{IVI_HOME}/include)
SET(IVI_LIBS $ENV{IVI_HOME}/LIB/MSC)

add_definitions(-DNOMINMAX)
add_definitions(-DSTATIC_LIBMONGOCLIENT)
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})

include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Network_INCLUDES})
include_directories(${Boost_INCLUDE_DIRS})
include_directories( ../../lib/db_ctrl $ENV{MONGO_HOME}/include  ../../lib/mongo_ctrl_v3  ../../lib/socket_rpc/include ../../lib/common_functions 
						${VISA_INCLUDES} ${IVI_INCLUDES}
						${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/../../Devices ${PROJECT_SOURCE_DIR}/../../Devices/PXIModules
						${PROJECT_SOURCE_DIR}/../../lib/interfaces
						)


###
#if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
#endif()
find_package(libmongocxx REQUIRED)
include_directories(${LIBMONGOCXX_INCLUDE_DIRS})
###
 
set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")

if(WIN32)
	set(MY_WIN_FLAG WIN32)
	set(CMAKE_CXX_FLAGS "/EHa /Zc:wchar_t")
	set(ADDITIONAL_LIBS ws2_32)
else()
	set(ADDITIONAL_LIBS pthread)
endif()
  
SET(AFAR_SRV_HEADERS afar_server_application.h )
SET(AFAR_SRV_SOURCES main.cpp afar_server_application.cpp)
SET(RPC_SOURCES 		${PROJECT_SOURCE_DIR}/afar_server_socket_rpc.h
					${PROJECT_SOURCE_DIR}/afar_server_socket_rpc.cpp

)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY} $ENV{OMNIBUS_HOME}/lib ${Boost_LIBRARY_DIRS} ${VISA_LIBS} ${IVI_LIBS})

FILE(GLOB_RECURSE SRC_CPP ${PROJECT_SOURCE_DIR}/src/*.cpp)
FILE(GLOB_RECURSE SRC_H ${PROJECT_SOURCE_DIR}/src/*.h)
FILE(GLOB_RECURSE SRC_HPP ${PROJECT_SOURCE_DIR}/src/*.hpp)
 
QT5_ADD_RESOURCES(AFAR_SERVER_RESOURCES afar_server.qrc)
QT5_ADD_RESOURCES(AFAR_VISUALIZE_RESOURCES afar_visualize.qrc)
add_executable(afar_server ${MY_WIN_FLAG} ${AFAR_SRV_HEADERS} ${AFAR_SRV_SOURCES} ${RPC_SOURCES} 
                                                                  ${MON_LIB_H} ${MON_LIB_CPP} ${SRC_CPP} ${SRC_H} ${SRC_HPP} ${AFAR_SERVER_RESOURCES})
target_link_libraries (afar_server ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES} 
                                                  mongo_controller_v3 common_functions db_ctrl socket_rpc devices pxi_modules
											 ${Boost_LIBRARIES} ${ADDITIONAL_LIBS}
												visa32.lib AgMWSwitch ${LIBMONGOCXX_LIBRARIES})


if(WIN32)
  qt5_use_modules(afar_server Core Widgets Network)
endif()
add_custom_target(afar_rpc_gen
	COMMAND rpc_parser ${PROJECT_SOURCE_DIR}/afar_server_application.h ${PROJECT_SOURCE_DIR} afar_server
)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")



set (AFAR_SERVER_plugin_SOURCES afar_server_plugin.h  afar_server_plugin.cpp ${PROJECT_SOURCE_DIR}/afar_server_rpc.h ${PROJECT_SOURCE_DIR}/afar_server_rpc.cpp)
                        

add_library(afar_server_plugin SHARED ${AFAR_SERVER_plugin_SOURCES}  )
target_link_libraries(afar_server_plugin ${Qt5Core_LIBRARIES}  ${Qt5Widgets_LIBRARIES} common_functions db_ctrl mongo_controller_v3 socket_rpc)
qt5_use_modules(afar_server_plugin Core Widgets)

add_dependencies(afar_server afar_server_plugin)
 
add_dependencies(afar_server_plugin afar_rpc_gen)

set (AFAR_SERVER_visualize_plugin_SOURCES afar_server_visualize_plugin.h afar_server_visualize_plugin.cpp )

add_library(afar_server_visualize_plugin SHARED ${AFAR_SERVER_visualize_plugin_SOURCES} ${AFAR_VISUALIZE_RESOURCES})
target_link_libraries(afar_server_visualize_plugin ${Qt5Core_LIBRARIES}  ${Qt5Widgets_LIBRARIES} common_functions db_ctrl mongo_controller_v3 ${LIBMONGOCXX_LIBRARIES} )
qt5_use_modules(afar_server_visualize_plugin Core Widgets)
 

set (AFAR_SERVER_client_visualize_plugin_SOURCES afar_server_client_visualize_plugin.h afar_server_client_visualize_plugin.cpp )

add_library(afar_server_client_visualize_plugin SHARED ${AFAR_SERVER_client_visualize_plugin_SOURCES})
target_link_libraries(afar_server_client_visualize_plugin ${Qt5Core_LIBRARIES}  ${Qt5Widgets_LIBRARIES} common_functions db_ctrl mongo_controller_v3 ${LIBMONGOCXX_LIBRARIES} )
qt5_use_modules(afar_server_client_visualize_plugin Core Widgets)