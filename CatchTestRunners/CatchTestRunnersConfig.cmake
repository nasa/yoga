include(CMakeFindDependencyMacro)
find_dependency(Catch2)
find_dependency(MessagePasser)
find_dependency(Kokkos)
include("${CMAKE_CURRENT_LIST_DIR}/CatchTestRunners.cmake")
