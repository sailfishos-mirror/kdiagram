set(DataValueTexts_SRCS
    mainwindow.cpp
    mainwindow.h
    main.cpp
)

qt_wrap_ui(DataValueTexts_SRCS
    mainwindow.ui
)

qt_add_resources(DataValueTexts_SRCS
    DataValueTexts.qrc
)

add_executable(DataValueTexts  ${DataValueTexts_SRCS})

target_link_libraries(DataValueTexts testtools KChart6 Qt::Widgets)
