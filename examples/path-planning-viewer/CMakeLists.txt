set(srcs
    main.cpp
    MainWindow.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/source/grabber)
include_directories(${CMAKE_SOURCE_DIR}/source/viewer)
include_directories(${CMAKE_SOURCE_DIR}/source/common)
include_directories(${CMAKE_SOURCE_DIR}/source/robot-control)

qt5_wrap_ui(uis_hdrs MainWindow.ui)
qt5_add_resources(resources icons.qrc)

add_executable(path-planning-viewer ${srcs} ${uis_hdrs} ${resources})

target_link_libraries(path-planning-viewer common grabber viewer robot-control Qt5::Core Qt5::Widgets Qt5::Gui
                        ${QTGSTREAMER_LIBRARY} ${QTGSTREAMER_LIBRARIES}
                        ${QTGSTREAMER_UTILS_LIBRARY} ${QTGSTREAMER_UTILS_LIBRARIES}
                        ${QTGSTREAMER_UTILS_LIBRARY} ${QTGSTREAMER_UTILS_LIBRARIES}
                        ${QTGLIB_LIBRARIES} ${QTGSTREAMER_UI_LIBRARY} ${QTGSTREAMER_UI_LIBRARIES}
                        ${GSTREAMER_LIBRARIES} ${GSTREAMER_VIDEO_LIBRARIES}
                        ${Boost_LIBRARIES})
