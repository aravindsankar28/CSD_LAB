# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ganesh/projects/CacheSimulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ganesh/projects/CacheSimulator/build

# Include any dependencies generated for this target.
include CMakeFiles/cachesimulator.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cachesimulator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cachesimulator.dir/flags.make

CMakeFiles/cachesimulator.dir/main.o: CMakeFiles/cachesimulator.dir/flags.make
CMakeFiles/cachesimulator.dir/main.o: ../main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ganesh/projects/CacheSimulator/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/cachesimulator.dir/main.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/cachesimulator.dir/main.o -c /home/ganesh/projects/CacheSimulator/main.cpp

CMakeFiles/cachesimulator.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cachesimulator.dir/main.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ganesh/projects/CacheSimulator/main.cpp > CMakeFiles/cachesimulator.dir/main.i

CMakeFiles/cachesimulator.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cachesimulator.dir/main.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ganesh/projects/CacheSimulator/main.cpp -o CMakeFiles/cachesimulator.dir/main.s

CMakeFiles/cachesimulator.dir/main.o.requires:
.PHONY : CMakeFiles/cachesimulator.dir/main.o.requires

CMakeFiles/cachesimulator.dir/main.o.provides: CMakeFiles/cachesimulator.dir/main.o.requires
	$(MAKE) -f CMakeFiles/cachesimulator.dir/build.make CMakeFiles/cachesimulator.dir/main.o.provides.build
.PHONY : CMakeFiles/cachesimulator.dir/main.o.provides

CMakeFiles/cachesimulator.dir/main.o.provides.build: CMakeFiles/cachesimulator.dir/main.o

# Object files for target cachesimulator
cachesimulator_OBJECTS = \
"CMakeFiles/cachesimulator.dir/main.o"

# External object files for target cachesimulator
cachesimulator_EXTERNAL_OBJECTS =

cachesimulator: CMakeFiles/cachesimulator.dir/main.o
cachesimulator: CMakeFiles/cachesimulator.dir/build.make
cachesimulator: CMakeFiles/cachesimulator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable cachesimulator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cachesimulator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cachesimulator.dir/build: cachesimulator
.PHONY : CMakeFiles/cachesimulator.dir/build

CMakeFiles/cachesimulator.dir/requires: CMakeFiles/cachesimulator.dir/main.o.requires
.PHONY : CMakeFiles/cachesimulator.dir/requires

CMakeFiles/cachesimulator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cachesimulator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cachesimulator.dir/clean

CMakeFiles/cachesimulator.dir/depend:
	cd /home/ganesh/projects/CacheSimulator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ganesh/projects/CacheSimulator /home/ganesh/projects/CacheSimulator /home/ganesh/projects/CacheSimulator/build /home/ganesh/projects/CacheSimulator/build /home/ganesh/projects/CacheSimulator/build/CMakeFiles/cachesimulator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cachesimulator.dir/depend

