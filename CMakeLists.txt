CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
project(
    "cDataPipeline"
    VERSION 0.1.0
    DESCRIPTION "C API of the FAIR Data Pipeline"
    LANGUAGES C CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Uses cppDataPipeline as a dependency to build the C API
add_subdirectory("external_packages/cppDataPipeline")
add_library(cDataPipeline STATIC ./include/fdp/fdp.h ./src/fdp_c_bindings.cxx)
add_library(cDataPipeline::cDataPipeline ALIAS cDataPipeline)
target_compile_features(cDataPipeline PRIVATE cxx_std_11)
target_include_directories(
    cDataPipeline 
    PUBLIC
    $<BUILD_INTERFACE:${cDataPipeline_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${fdpapi_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(cDataPipeline PUBLIC $<BUILD_INTERFACE:fdpapi>)



# List of targets to export
set(cDataPipeline_targets cDataPipeline)
install(TARGETS ${cDataPipeline_targets}
        EXPORT cDataPipelineTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Set bits to allow this package to be findable by other cmake projects
write_basic_package_version_file("cDataPipelineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/cDataPipelineConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/cDataPipelineConfig.cmake"
    INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/cDataPipeline
)

install(
    FILES 
        "${PROJECT_BINARY_DIR}/cDataPipelineConfig.cmake"
        "${PROJECT_BINARY_DIR}/cDataPipelineConfigVersion.cmake"
    DESTINATION 
        ${CMAKE_INSTALL_LIBDIR}/cmake/cDataPipeline
)

install(
    EXPORT cDataPipelineTargets
    FILE cDataPipelineTargets.cmake
    NAMESPACE cDataPipeline::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cDataPipeline
)


# install project include dir to system include dir
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
