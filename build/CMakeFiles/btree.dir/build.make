# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /home/mlovatonv/miniconda3/bin/cmake

# The command to remove a file.
RM = /home/mlovatonv/miniconda3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/build

# Include any dependencies generated for this target.
include CMakeFiles/btree.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/btree.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/btree.dir/flags.make

CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.o: CMakeFiles/btree.dir/flags.make
CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.o: ../src/utec/disk/pagemanager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.o -c /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/src/utec/disk/pagemanager.cpp

CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/src/utec/disk/pagemanager.cpp > CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.i

CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/src/utec/disk/pagemanager.cpp -o CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.s

# Object files for target btree
btree_OBJECTS = \
"CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.o"

# External object files for target btree
btree_EXTERNAL_OBJECTS =

libbtree.a: CMakeFiles/btree.dir/src/utec/disk/pagemanager.cpp.o
libbtree.a: CMakeFiles/btree.dir/build.make
libbtree.a: CMakeFiles/btree.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libbtree.a"
	$(CMAKE_COMMAND) -P CMakeFiles/btree.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/btree.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/btree.dir/build: libbtree.a

.PHONY : CMakeFiles/btree.dir/build

CMakeFiles/btree.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/btree.dir/cmake_clean.cmake
.PHONY : CMakeFiles/btree.dir/clean

CMakeFiles/btree.dir/depend:
	cd /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/build /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/build /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/build/CMakeFiles/btree.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/btree.dir/depend

