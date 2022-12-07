CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
project(
    "fdpapic"
    VERSION 0.1.0
    DESCRIPTION "C API of the FAIR Data Pipeline"
    LANGUAGES C CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(fdpapic ./include/fdp/fdp.h ./src/fdp_c_bindings.cxx)
add_library(fdpapi::fdpapic ALIAS fdpapic)
target_compile_features(fdpapic PRIVATE cxx_std_11)
target_include_directories(
    fdpapic 
    PUBLIC
    $<BUILD_INTERFACE:${fdpapic_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Uses cppDataPipeline as a dependency to build the C API
add_subdirectory("external_packages/cppDataPipeline")
target_link_libraries(fdpapic PRIVATE fdpapi::fdpapi)

# Set rules for installing targets
set(fdpapic_targets fdpapic)
install(TARGETS ${fdpapic_targets}
        EXPORT fdpapicTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install export file, which contains code to allow other projects to import this project
install(
    EXPORT fdpapicTargets
    FILE fdpapicTargets.cmake
    NAMESPACE fdpapi::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fdpapic
)

# Install this project's include dir, so projects importing it can access it
install(DIRECTORY ./include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Create files that allow installed project to be discovered using find_package
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/fdpapicConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/fdpapicConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fdpapic
)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/fdpapicConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)

# Specify where to install the config files we generated
install(
    FILES 
        "${PROJECT_BINARY_DIR}/fdpapicConfig.cmake"
        "${PROJECT_BINARY_DIR}/fdpapicConfigVersion.cmake"
    DESTINATION 
        ${CMAKE_INSTALL_LIBDIR}/cmake/fdpapic
)

# Configure the .in pkg-config file and install
CONFIGURE_FILE(
    ${PROJECT_SOURCE_DIR}/cmake/fdpapic.pc.in ${PROJECT_BINARY_DIR}/fdpapic.pc @ONLY
)

INSTALL(
    FILES 
        ${PROJECT_BINARY_DIR}/fdpapic.pc
    DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)


# Tests
string(COMPARE EQUAL "${PROJECT_NAME}" "${CMAKE_PROJECT_NAME}" FDPAPIC_IS_TOP_LEVEL)
if(FDPAPIC_IS_TOP_LEVEL)
    enable_testing()

    add_custom_target(build-unit-tests)
    add_custom_target(build-integrated-tests)
    add_custom_target(run-unit-tests)
    add_custom_target(run-integrated-tests)

    # Include subdirectory 'tests'
    # Tests within this subdirectory must add themselves as dependencies
    # to the targets listed above
    add_subdirectory(tests EXCLUDE_FROM_ALL)

    # Manage dependencies
    add_dependencies(run-unit-tests build-unit-tests)
    add_dependencies(run-integrated-tests build-integrated-tests)

    # Add nice targets for the user
    add_custom_target(build-tests)
    add_dependencies(build-tests build-unit-tests build-integrated-tests)

    add_custom_target(run-tests)
    add_dependencies(run-tests run-unit-tests run-integrated-tests)

    add_custom_target(tests)
    add_dependencies(tests run-tests)
endif()
