get_filename_component(MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
set(TARGET_NAME ${PROJECT_NAME}_${MODULE_NAME})

# Qt
if(${Qt5_DIR})
  find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGL PATHS ${Qt5_DIR} NO_DEFAULT_PATH)
else()
  find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGL)
endif()

# Boost
find_package(Boost REQUIRED)

# target
file(GLOB_RECURSE SRCS "*.cc")
add_executable(${TARGET_NAME} ${SRCS})
target_link_libraries(${TARGET_NAME} midi_io midi_proto ${PROTOBUF_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL GL boost_system boost_filesystem)

# install
install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# install resources
add_subdirectory(resources)
