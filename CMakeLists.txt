cmake_minimum_required(VERSION 2.8)

project(autologiccontroller)

SET(INSTALL_BIN_DIR         bin/controller CACHE PATH "Executable install dir")

FIND_PACKAGE(appfw COMPONENTS thread REQUIRED)
FIND_PACKAGE(autosocm COMPONENTS thread REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/modules)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/sharedmemory)
include_directories(../CA_Cockpik_IC_AutoServices/services/autosoundservice/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/modules                    modulessrc)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/sharedmemory               sharesrc)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/modules/chimemodule        chime_srcs)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/modules/commonmodule       common_srcs)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/modules/gaugemodule        gauge_srcs)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/modules/telltalemodule     telltale_srcs)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/modules/tripcomputermodule tripcomputer_srcs)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/modules/warningmodule      warning_srcs)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/modules/adasmodule         adas_srcs)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/modules/interactionmodule  interaction_srcs)

add_executable(autologiccontroller
                ${modulessrc}
                ${CMAKE_CURRENT_SOURCE_DIR}/src/autologiccontroller.cpp
                ${sharesrc}
                ${chime_srcs}
                ${common_srcs}
                ${gauge_srcs}
                ${telltale_srcs}
                ${tripcomputer_srcs}
                ${warning_srcs}
                ${adas_srcs}
                ${interaction_srcs})

target_link_libraries(autologiccontroller pthread rt ${APPFW_LIBRARIES} ${AUTOSOCM_LIBRARIES} log)
install(TARGETS autologiccontroller RUNTIME DESTINATION ${INSTALL_BIN_DIR})



