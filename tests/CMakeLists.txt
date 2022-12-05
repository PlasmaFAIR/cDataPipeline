# Unit tests
# ----------------

# TODO

# Integrated tests
# ----------------

# Add cSimpleModel executable as an integrated testing dependency.
add_subdirectory("integrated_tests/cDataPipelineSimpleModel")
add_dependencies(build-integrated-tests cSimpleModel)

add_test(
    NAME
        cDataPipeline-Integrated-SimpleModel-PullData
    COMMAND
        fair pull tests/integrated_tests/cDataPipelineSimpleModel.yaml
    WORKING_DIRECTORY
        ${CMAKE_SOURCE_DIR}
)

add_test(
    NAME
        cDataPipeline-Integrated-SimpleModel-RunModel
    COMMAND
        fair run tests/integrated_tests/cDataPipelineSimpleModel.yaml
    WORKING_DIRECTORY
        ${CMAKE_SOURCE_DIR}
)

set_tests_properties(
        cDataPipeline-Integrated-SimpleModel-RunModel 
    PROPERTIES DEPENDS
        cDataPipeline-Integrated-SimpleModel-PullData
)

add_custom_target(
        run-simple-model
    COMMAND
        ctest -R cDataPipeline-Integrated-SimpleModel --output-on-failure
)

add_dependencies(run-integrated-tests run-simple-model)
