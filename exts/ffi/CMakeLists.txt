find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBFFI REQUIRED IMPORTED_TARGET libffi)

link_libraries(${LIBFFI_LIBRARIES})
include_directories(${LIBFFI_INCLUDE_DIRS})
