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

# Utility rule file for btree-gtest-run.

# Include the progress variables for this target.
include CMakeFiles/btree-gtest-run.dir/progress.make

CMakeFiles/btree-gtest-run:
	./btree-gtest

btree-gtest-run: CMakeFiles/btree-gtest-run
btree-gtest-run: CMakeFiles/btree-gtest-run.dir/build.make

.PHONY : btree-gtest-run

# Rule to build all files generated by this target.
CMakeFiles/btree-gtest-run.dir/build: btree-gtest-run

.PHONY : CMakeFiles/btree-gtest-run.dir/build

CMakeFiles/btree-gtest-run.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/btree-gtest-run.dir/cmake_clean.cmake
.PHONY : CMakeFiles/btree-gtest-run.dir/clean

CMakeFiles/btree-gtest-run.dir/depend:
	cd /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/build /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/build /home/mlovatonv/utec/ada/disk-based-btree-mlovatonv/build/CMakeFiles/btree-gtest-run.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/btree-gtest-run.dir/depend

