find_package(Threads REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS json headers)

file(GLOB SOURCES "*.cpp")
add_library(SerializeX INTERFACE)
target_link_libraries(SerializeX INTERFACE Boost::headers Boost::json Threads::Threads)
set_target_properties(SerializeX PROPERTIES CXX_STANDARD 23)

target_include_directories(SerializeX INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/SerializeX> # Path used after installation
)

install(TARGETS SerializeX
    EXPORT SerializeXTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/SerializeX
)

install(DIRECTORY include/ DESTINATION include/SerializeX)

install(EXPORT SerializeXTargets
    FILE SerializeXTargets.cmake
    NAMESPACE SerializeX::
    DESTINATION lib/cmake/SerializeX
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SerializeXConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/SerializeXConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SerializeXConfig.cmake"
    INSTALL_DESTINATION lib/cmake/SerializeX
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SerializeXConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SerializeXConfigVersion.cmake"
    DESTINATION lib/cmake/SerializeX
)
