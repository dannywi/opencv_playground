cmake_minimum_required(VERSION 3.00)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(opencv_apps)

set(OpenCV_DIR "external/opencv")
find_package(OpenCV REQUIRED)

function(setup_target TARGET_NAME)
  target_link_libraries(${TARGET_NAME} ${OpenCV_LIBS})

  target_include_directories(${TARGET_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
  target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
endfunction()

function(set_target_name)
  get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
  string(REPLACE " " "_" FOLDER_NAME ${FOLDER_NAME})
  set(TARGET_NAME ${FOLDER_NAME} PARENT_SCOPE)
endfunction()

add_subdirectory(app_hello_world)
add_subdirectory(app_mask)
add_subdirectory(app_blend)