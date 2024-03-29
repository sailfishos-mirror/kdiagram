set( KGANTTLIB_VERSION "3.0.1" )

ecm_setup_version(${KGANTTLIB_VERSION}
    VARIABLE_PREFIX KGANTT
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kgantt_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KGantt6ConfigVersion.cmake"
    SOVERSION 3
)

if(BUILD_TESTING)
    if(Qt6Test_FOUND)
        add_subdirectory(test)
    endif()
endif()

# TODO: remove this, code should be built the same with and without unit tests
# those KGantt unit tests which are enabled in the sources should be moved into own files
if(NOT BUILD_TESTING)
    add_definitions(-DKDAB_NO_UNIT_TESTS)
endif()

set( kgantt_LIB_SRCS
    kganttglobal.cpp
    kganttprintingcontext.cpp
    kganttview.cpp
    kganttstyleoptionganttitem.cpp
    kganttgraphicsview.cpp
    kganttabstractrowcontroller.cpp
    kgantttreeviewrowcontroller.cpp
    kganttlistviewrowcontroller.cpp
    kganttgraphicsscene.cpp
    kganttgraphicsitem.cpp
    kganttconstraint.cpp
    kganttconstraintproxy.cpp
    kganttconstraintgraphicsitem.cpp
    kganttitemdelegate.cpp
    kganttforwardingproxymodel.cpp
    kganttsummaryhandlingproxymodel.cpp
    kganttproxymodel.cpp
    kganttconstraintmodel.cpp
    kganttabstractgrid.cpp
    kganttdatetimegrid.cpp
    kganttlegend.cpp
    kganttdatetimetimeline.cpp
    kganttdatetimetimelinedialog.cpp
    kganttpenstylecombobox.cpp
    unittest/test.cpp
    unittest/testregistry.cpp
)

qt_wrap_ui(kgantt_LIB_SRCS
    kganttdatetimetimelinedialog.ui
)

ecm_create_qm_loader(kgantt_LIB_SRCS kgantt6_qt)

add_library( KGantt6 ${kgantt_LIB_SRCS} )

generate_export_header( KGantt6 BASE_NAME KGantt )

target_include_directories(KGantt6
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KGantt6>"
)

target_link_libraries( KGantt6
PUBLIC
    Qt::Widgets
PRIVATE
    Qt::PrintSupport
)

set_target_properties( KGantt6 PROPERTIES
  VERSION     ${KGANTT_VERSION}
  SOVERSION   ${KGANTT_SOVERSION}
  EXPORT_NAME "KGantt6"
)

install( TARGETS KGantt6
    EXPORT KGantt6Targets
    ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(kgantt_LIB_HEADERS
    HEADER_NAMES
    KGanttGlobal
    KGanttPrintingContext
    KGanttView
    KGanttStyleOptionGanttItem
    KGanttGraphicsView
    KGanttAbstractRowController
    KGanttTreeViewRowController
    KGanttListViewRowController
    KGanttGraphicsScene
    KGanttGraphicsItem
    KGanttConstraint
    KGanttConstraintProxy
    KGanttConstraintGraphicsItem
    KGanttItemDelegate
    KGanttForwardingProxyModel
    KGanttSummaryHandlingProxyModel
    KGanttProxyModel
    KGanttConstraintModel
    KGanttAbstractGrid
    KGanttDateTimeGrid
    KGanttLegend
    KGanttDateTimeTimeLine
    KGanttDateTimeTimeLineDialog
    KGanttPenStyleComboBox

    REQUIRED_HEADERS kgantt_LIB_HEADERS
)

install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/kgantt_export.h
            ${kgantt_LIB_HEADERS}
            kganttabstractgrid_p.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KGantt6
        COMPONENT Devel)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kgantt_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KGantt6 COMPONENT Devel)

if(BUILD_QCH)
    ecm_add_qch(
        KGantt6_QCH
        NAME KGantt6
        VERSION ${KGANTTLIB_VERSION}
        ORG_DOMAIN org.kde
        SOURCES
            Mainpage.dox
            ${kgantt_LIB_HEADERS}
            ${kgantt_LIB_SRCS}
        LINK_QCHS
            Qt6Core_QCH
            Qt6Widgets_QCH
        BLANK_MACROS
            KGANTT_EXPORT
            KGANTT_DEPRECATED
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KGantt6")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KGantt6_QCH
        FILE KGantt6QchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KGantt6QchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KGantt6Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KGantt6Config.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/KGantt6Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/KGantt6ConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel)

install(EXPORT KGantt6Targets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KGantt6Targets.cmake)

ecm_generate_pri_file(
    VERSION ${KGANTTLIB_VERSION}
    BASE_NAME KGantt6
    LIB_NAME KGantt6
    DEPS "widgets printsupport"
    FILENAME_VAR PRI_FILENAME
    INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KGantt
)

install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
