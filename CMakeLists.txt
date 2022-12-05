CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
project(
    "cDataPipeline"
    VERSION 0.1.0
    DESCRIPTION "C API of the FAIR Data Pipeline"
    LANGUAGES C CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(cDataPipeline SHARED ./include/fdp/fdp.h ./src/fdp_c_bindings.cxx)
add_library(cDataPipeline::cDataPipeline ALIAS cDataPipeline)
target_compile_features(cDataPipeline PRIVATE cxx_std_11)
target_include_directories(
    cDataPipeline 
    PUBLIC
    $<BUILD_INTERFACE:${cDataPipeline_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Uses cppDataPipeline as a dependency to build the C API
add_subdirectory("external_packages/cppDataPipeline")
target_link_libraries(cDataPipeline PRIVATE cppDataPipeline::cppDataPipeline)

# Set rules for installing targets
set(cDataPipeline_targets cDataPipeline)
install(TARGETS ${cDataPipeline_targets}
        EXPORT cDataPipelineTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install export file, which contains code to allow other projects to import this project
install(
    EXPORT cDataPipelineTargets
    FILE cDataPipelineTargets.cmake
    NAMESPACE cDataPipeline::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cDataPipeline
)

# Install this project's include dir, so projects importing it can access it
install(DIRECTORY ./include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Create files that allow installed project to be discovered using find_package
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/cDataPipelineConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/cDataPipelineConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cDataPipeline
)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/cDataPipelineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)

# Specify where to install the config files we generated
install(
    FILES 
        "${PROJECT_BINARY_DIR}/cDataPipelineConfig.cmake"
        "${PROJECT_BINARY_DIR}/cDataPipelineConfigVersion.cmake"
    DESTINATION 
        ${CMAKE_INSTALL_LIBDIR}/cmake/cDataPipeline
)

# Tests
string(COMPARE EQUAL "${PROJECT_NAME}" "${CMAKE_PROJECT_NAME}" C_DATA_PIPELINE_IS_TOP_LEVEL)
if(C_DATA_PIPELINE_IS_TOP_LEVEL)
    enable_testing()
    add_custom_target(build-unit-tests)
    add_custom_target(build-integrated-tests)
    add_custom_target(build-tests)
    add_dependencies(build-tests build-unit-tests build-integrated-tests)
    
    add_custom_target(run-unit-tests)
    add_custom_target(run-integrated-tests)
    add_custom_target(run-tests)
    add_dependencies(run-unit-tests build-unit-tests)
    add_dependencies(run-integrated-tests build-integrated-tests)
    add_dependencies(run-tests run-unit-tests run-integrated-tests)

    # Include subdirectory 'tests'
    # Tests within this subdirectory must add themselves as dependencies
    # to the target build-unit-tests or build-integrated-tests
    add_subdirectory(tests EXCLUDE_FROM_ALL)

    add_custom_target(tests)
    add_dependencies(tests run-tests)
endif()
