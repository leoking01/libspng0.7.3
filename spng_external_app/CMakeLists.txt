project(TestInstallation)
cmake_minimum_required(VERSION 3.5)

find_package(spng  REQUIRED  PATHS  "D:\\win10\\aspDep14\\libspng\\libspng_19.0.24215.1_install")

add_executable(mymathApp mymathApp.cc)
target_link_libraries(mymathApp spng::spng)
target_include_directories(mymathApp  PRIVATE  "D:\\win10\\aspDep14\\libspng\\libspng_19.0.24215.1_install/include")
