cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(traf_exercise VERSION ${PROJECT_VERSION})

add_executable(traf_exercise 
				main.cpp node.cpp 
				netAnalyzer.cpp 
				node.h 
				netAnalyzer.h)

set_target_properties(traf_exercise PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
 
file(COPY ${CMAKE_SOURCE_DIR}/traf.txt
     DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/traf_test.txt
     DESTINATION ${CMAKE_BINARY_DIR})

install(TARGETS traf_exercise RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT alexsh93@live.ru)
include(CPack)
