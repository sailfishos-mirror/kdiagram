cmake_minimum_required(VERSION 3.16)

project( kdiagram )

set(QT_REQUIRED_VERSION "6.5.0")
set(KF_MIN_VERSION "6.3.0")
set(KF_MAJOR_VERSION "6")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

set(KDE_COMPILERSETTINGS_LEVEL 5.84)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMGenerateHeaders)
include(ECMSetupVersion)
include(ECMGeneratePriFile)
include(ECMPoQmTools)
include(ECMAddTests)

include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(FeatureSummary)
include(GenerateExportHeader)
include(ECMDeprecationSettings)

set(HAS_ECMAddQch TRUE)
include(ECMAddQch)

cmake_dependent_option(BUILD_QCH
    "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF
    "HAS_ECMAddQch" OFF
)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

find_package(Qt6 ${QT_REQUIRED_VERSION} REQUIRED NO_MODULE
    COMPONENTS
    PrintSupport
    Svg
    Widgets
)
if(BUILD_TESTING)
    find_package(Qt6Test QUIET)
    if (NOT Qt6Test_FOUND)
        message(STATUS "Qt6Test not found, unit tests will not be built.")
    endif()
endif()

find_package(Qt6Sql QUIET)
if (NOT Qt6Sql_FOUND)
    message(STATUS "Qt5Sql not found, examples with SQL will not be built.")
endif()

add_definitions(
    -DQT_NO_STL
    -DQT_FATAL_ASSERT
    -DUSE_EXCEPTIONS
)

ecm_set_disabled_deprecation_versions(QT 5.15
    KF 6.5.0
)


# TODO: port from .pro
#  NOMINMAX avoids conflicting definitions of min and max
#  _USE_MATH_DEFINES makes M_PI defined
# win32 {
# !win32-g++ {
#     DEFINES += NOMINMAX _USE_MATH_DEFINES
#     QMAKE_CXXFLAGS += /GR /EHsc /wd4251
# }
# }

# TODO: port g++.pri to cmake and check what is still needed


ecm_install_po_files_as_qm(poqm)

add_subdirectory( src )

if(BUILD_TESTING)
    add_subdirectory( examples )
    add_subdirectory( tests )

    if(Qt6Test_FOUND)
        add_subdirectory( qtests )
    endif()
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
