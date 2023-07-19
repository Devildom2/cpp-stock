CMAKE_MINIMUM_REQUIRED(VERSION 3.22) #Cmakeソフトウェアの最低保証バージョンを明記
project(ringbuffer CXX) #PROJECT_NAMEとPROJECT_SOURSE_DIRの2つが宣言・定義される CXXはプログラム言語選択(C++)
message("project name is automaically set as: PROJECT_NAME ${PROJECT_NAME} PROJECT_SOURSE_DIR ${PROJECT_SOURSE_DIR}")

set(PROJECT_VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

#find_package(Boost REQUIRED 
#  COMPONENTS
#    context
#    filesystem
#    program_options
#    regex
#    system
#    thread
#)

if(Boost_FOUND)
  message (STATUS "Found Boost")
else()
  message(WARNING "Boost not found")
endif()

# find_package(OpenCV REQUIRED)
# if(OpenCV_FOUND)
#   message (STATUS "Found OpenCV")
# else(OpenCV_FOUND)
#   message(WARNING "OpenCV not found")
# endif()

set(target_SRC
  src/FileLogger.cpp
  src/Logger.cpp
  src/main.cpp
)

#set(target_HEADERS
#  include/cmake_training/my_class.h
#)

add_executable(main ${target_SRC})
target_include_directories(main PUBLIC include ${BOOST_INCLUDE_DIRS})
target_link_libraries(main PUBLIC ${Boost_LIBRARIES})


MESSAGE(STATUS "--------------------------------------------")
MESSAGE(STATUS "Info :")
MESSAGE(STATUS "  Project Name = ${PROJECT_NAME}")
MESSAGE(STATUS "  Version =     ${PROJECT_VERSION}")
MESSAGE(STATUS "  CMAKE_VERSION =        ${CMAKE_VERSION}")
MESSAGE(STATUS "  CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "  CMAKE_BUILD_TYPE =     ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "  CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")
message(STATUS "  Boost_LIBRARIES =        ${Boost_LIBRARIES}")
message(STATUS "  OpenCV_VERSION =       ${OpenCV_VERSION}")