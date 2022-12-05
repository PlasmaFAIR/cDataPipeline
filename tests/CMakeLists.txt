# Unit tests
# ----------------

# TODO

# Integrated tests
# ----------------

# Add cSimpleModel executable as an integrated testing dependency.
add_subdirectory("integrated_tests/cDataPipelineSimpleModel")
add_dependencies(build-tests cSimpleModel)
