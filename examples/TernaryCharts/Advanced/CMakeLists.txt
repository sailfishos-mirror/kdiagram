set(TernaryChartsAdvanced_SRCS
    mainwindow.cpp
    mainwindow.h
    main.cpp
)

qt_wrap_ui(TernaryChartsAdvanced_SRCS
    mainwindow.ui
)

add_executable(TernaryChartsAdvanced  ${TernaryChartsAdvanced_SRCS})

target_link_libraries(TernaryChartsAdvanced KChart6 Qt::Widgets)
