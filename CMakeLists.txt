cmake_minimum_required(VERSION 3.10)
project(TestVesuvius)

# Ajouter manuellement les chemins si pkg-config ne fonctionne pas
include_directories("C:/OSGeo4W/include")
link_directories("C:/OSGeo4W/lib")

# Recherche de libLAS
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBLAS REQUIRED liblas)

# Ajout de l'exécutable
add_executable(main test_Vesuvius.cpp)

# Inclusion des répertoires d'en-têtes de libLAS
target_include_directories(main PRIVATE ${LIBLAS_INCLUDE_DIRS})

# Liaison avec les bibliothèques de libLAS
target_link_libraries(main PRIVATE ${LIBLAS_LIBRARIES})
