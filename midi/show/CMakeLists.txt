get_filename_component(MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
set(TARGET_NAME ${PROJECT_NAME}_${MODULE_NAME})

# Qt5
set(Qt5_COMPONENTS Core Gui Widgets OpenGL)
set(Qt5_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL)
if(DEFINED ENV{Qt5_ROOT_DIR})
  find_package(Qt5 REQUIRED COMPONENTS ${Qt5_COMPONENTS} PATHS "$ENV{Qt5_ROOT_DIR}" NO_DEFAULT_PATH)
else()
  message(WARNING "Qt5_ROOT_DIR is not set, fallback to default paths")
  find_package(Qt5 REQUIRED COMPONENTS ${Qt5_COMPONENTS})
endif()

# OpenCV
if(DEFINED ENV{OpenCV_ROOT_DIR})
  find_package(OpenCV REQUIRED PATHS "$ENV{OpenCV_ROOT_DIR}/share/OpenCV" NO_DEFAULT_PATH)
else()
  message(WARNING "OpenCV_ROOT_DIR is not set, fallback to default paths")
  find_package(OpenCV REQUIRED)
endif()
include_directories("${OpenCV_INCLUDE_DIRS}")

# Boost
find_package(Boost REQUIRED)

# target
file(GLOB_RECURSE SRCS "*.cc")
add_executable(${TARGET_NAME} ${SRCS})
target_link_libraries(${TARGET_NAME} midi_io midi_proto ${PROTOBUF_LIBRARIES} ${Qt5_LIBRARIES} GL boost_system boost_filesystem pthread)

# install
install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# install resources
add_subdirectory(resources)
