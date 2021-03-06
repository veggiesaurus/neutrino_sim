#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(neutrinosim)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_BUILD_TYPE Release)

# Find boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.4.0 COMPONENTS program_options iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})


#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(neutrino neutrino.cc ${sources} ${headers})
target_link_libraries(neutrino ${Boost_LIBRARIES})                                                                                                                                                                                                                            
target_link_libraries(neutrino ${Geant4_LIBRARIES} )
TARGET_LINK_LIBRARIES( neutrino ${Boost_LIBRARIES} )
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build N01. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(N01_SCRIPTS
    neutrino.in neutrino.out vis.mac
  )

foreach(_script ${N01_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
add_custom_target(neutrinosim DEPENDS neutrino)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS neutrino DESTINATION bin)

set(CMAKE_BUILD_TYPE Release)