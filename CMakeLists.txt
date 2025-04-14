cmake_minimum_required(VERSION 3.10)
project(TestVesuvius)

# Recherche de libLAS
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBLAS REQUIRED liblas)

# Ajout de l'exécutable
add_executable(main main.cpp)

# Inclusion des répertoires d'en-têtes de libLAS
target_include_directories(main PRIVATE ${LIBLAS_INCLUDE_DIRS})

# Liaison avec les bibliothèques de libLAS
target_link_libraries(main PRIVATE ${LIBLAS_LIBRARIES})
