set(srcs
    main.cpp
    gui/MainWindow.cpp
    gui/PreferencesDialog.cpp
    settings/Settings.cpp
    settings/Registry.cpp
)

qt5_wrap_ui(uis_hdrs gui/MainWindow.ui gui/PreferencesDialog.ui)
qt5_add_resources(resources icons.qrc)

include_directories(${CMAKE_SOURCE_DIR}/source/grabber)
include_directories(${CMAKE_SOURCE_DIR}/source/viewer)
include_directories(${CMAKE_SOURCE_DIR}/source/common)
include_directories(${CMAKE_SOURCE_DIR}/source/hub)
include_directories(${CMAKE_SOURCE_DIR}/source/tracker)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/source/messages)
include_directories(${CMAKE_SOURCE_DIR}/source/inter-species)
include_directories(${CMAKE_SOURCE_DIR}/source/robot-control)
include_directories(${CMAKE_SOURCE_DIR}/source/settings-interface)

add_executable(cats  ${srcs} ${uis_hdrs} ${resources})

# make a link to .aesl scripts
if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/aesl)
    execute_process(COMMAND rm -rf aesl
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/aesl)
    execute_process(COMMAND ln -s ${CMAKE_SOURCE_DIR}/source/robot-control/aesl aesl
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

target_link_libraries(cats common grabber tracker viewer hub
                        messages inter-species robot-control
                        settings-interface
                        Qt5::Core Qt5::Widgets Qt5::Gui Qt5::DBus
                        ${QTGSTREAMER_LIBRARY} ${QTGSTREAMER_LIBRARIES}
                        ${QTGSTREAMER_UTILS_LIBRARY} ${QTGSTREAMER_UTILS_LIBRARIES}
                        ${QTGLIB_LIBRARIES} ${QTGSTREAMER_UI_LIBRARY}
                        ${QTGSTREAMER_UI_LIBRARIES} ${GSTREAMER_LIBRARY}
                        ${GSTREAMER_VIDEO_LIBRARY} ${PROTOBUF_LIBRARIES}
                        -fsanitize=address)

#install(TARGETS cats DESTINATION .)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/aesl/* DESTINATION .)
#SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}")
