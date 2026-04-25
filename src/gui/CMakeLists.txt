cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(NOT TARGET calculator_math)
    message(FATAL_ERROR "Target calculator_math is missing. Configure from repository root using cmake -S . -B build.")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    helpwindow.cpp
    helpwindow.h
    helpwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET gui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(gui SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(gui
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(gui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets calculator::math)

set(APP_EXECUTABLE_NAME "ivs-kalkulacka")
set(APP_ICON_NAME "ivs-kalkulacka")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.gui)
endif()

if(DEFINED BUNDLE_ID_OPTION)
	set_target_properties(gui PROPERTIES
		${BUNDLE_ID_OPTION}
	)
endif()

set_target_properties(gui PROPERTIES
    OUTPUT_NAME ${APP_EXECUTABLE_NAME}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(UNIX AND NOT APPLE)
    set(APP_DESKTOP_FILE "${CMAKE_CURRENT_BINARY_DIR}/${APP_EXECUTABLE_NAME}.desktop")

    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/packaging/ivs-kalkulacka.desktop.in
        ${APP_DESKTOP_FILE}
        @ONLY
    )

    install(FILES
        ${APP_DESKTOP_FILE}
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
    )

    install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.svg
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps
        RENAME ${APP_ICON_NAME}.svg
    )
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gui)
endif()
