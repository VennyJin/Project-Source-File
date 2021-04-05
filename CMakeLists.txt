##########################################################
# set the project name
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
set(ProjectId "MI3")
message(Project Id : ${ProjectId})
project(${ProjectId})
set(CMAKE_CXX_STANDARD 14)

# Setting the variables
set(PROJ_NAME "MI3")
set(PROJ_INCLUDES "")
set(PROJ_LIBRARIES "")
set(SOURCE
${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
${CMAKE_CURRENT_LIST_DIR}/src/example_helper.cpp
${CMAKE_CURRENT_LIST_DIR}/src/Acquisition.cpp

############external test files #######
#${CMAKE_CURRENT_LIST_DIR}/src/testcode/Trigger.cpp
#${CMAKE_CURRENT_LIST_DIR}/src/dmd_grayscale_triggerin_example.cpp
)
message(------project name : ${PROJ_NAME} ---------)

##########################################################

# IF(WIN32)
#   SET(AJILEDRIVER_HOME "C:/Program Files (x86)/Ajile/AjileDriver/" CACHE STRING "Location of Ajile Driver include and lib")
#   ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
# ENDIF(WIN32)

# IF(WIN32)
#   LINK_DIRECTORIES(${LINK_DIRECTORIES} ${AJILEDRIVER_HOME}/lib)
#   INCLUDE_DIRECTORIES(
#     ${INCLUDE_DIRECTORIES} 
#     ${AJILEDRIVER_HOME}/include
#     )
# ENDIF(WIN32)

#IF(UNIX)
#  TARGET_LINK_LIBRARIES( ${PROJ_NAME}
#    -lajiledriver
#    ${OpenCV_LIBS}
#    )
# ELSE(UNIX)
#   IF(WIN32)
#     TARGET_LINK_LIBRARIES( dmd_grayscale_sinewave_example
#       ajiledriver.lib
#       ${OpenCV_LIBS}
#     )
#   ENDIF(WIN32)
#ENDIF(UNIX)

# ##############################################################################
# Spinnaker SDK
if(WIN32)
  set(Spinnaker_LIBS debug "Spinnakerd_v140.lib" optimized "Spinnaker_v140.lib")
  set(Spinnaker_INCLUDE_DIR
      "C:/Program Files/Point Grey Research/Spinnaker/include")
  link_directories(
    "C:/Program Files/Point Grey Research/Spinnaker/lib64/vs2015")
endif(WIN32)
if(UNIX)
  add_definitions(-DLINUX)
  set(Spinnaker_LIBS "libSpinnaker.so")
  set(Spinnaker_INCLUDE_DIR "/usr/include/spinnaker")
  link_directories("/usr/lib")
endif(UNIX)

# ##############################################################################
find_package( OpenCV REQUIRED )
INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${OpenCV_INCLUDE_DIRS}
  #${Spinnaker_INCLUDE_DIR} 
)
##########################################################
# Project
add_executable(${PROJ_NAME} ${SOURCE})

target_link_libraries(${PROJ_NAME} ${OpenCV_LIBS} ${Spinnaker_LIBS} -lajiledriver)

target_include_directories(${PROJ_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS} ${Spinnaker_INCLUDE_DIR} ./)

#message(: ${CMAKE_CURRENT_BINARY_DIR})
#message(: ${CMAKE_CURRENT_SOURCE_DIR})
#message(: ${OpenCV_INCLUDE_DIRS})
#message(: ${PROJECT_SOURCE_DIR})
#message(: ${Spinnaker_INCLUDE_DIR})