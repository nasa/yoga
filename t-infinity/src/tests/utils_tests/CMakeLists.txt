add_catch_unit_test(SnapLoaderTests SnapLoaderTests.cpp)
target_link_libraries(SnapLoaderTests PUBLIC infinity::infinity)
add_dependencies(SnapLoaderTests infinity::SnapReader)
get_filename_component(INFINITY_UTILS_DIR ${CMAKE_CURRENT_BINARY_DIR}/../../utilities REALPATH)
target_compile_definitions(SnapLoaderTests PRIVATE INFINITY_UTILS_DIR="${INFINITY_UTILS_DIR}")
