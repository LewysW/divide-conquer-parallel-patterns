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
CMAKE_COMMAND = /opt/clion-2019.2.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2019.2.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lewys/Documents/CS4204/CS4204-P2-DC/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CS4204_P2_DC.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CS4204_P2_DC.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CS4204_P2_DC.dir/flags.make

CMakeFiles/CS4204_P2_DC.dir/main.cpp.o: CMakeFiles/CS4204_P2_DC.dir/flags.make
CMakeFiles/CS4204_P2_DC.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CS4204_P2_DC.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CS4204_P2_DC.dir/main.cpp.o -c /home/lewys/Documents/CS4204/CS4204-P2-DC/src/main.cpp

CMakeFiles/CS4204_P2_DC.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CS4204_P2_DC.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lewys/Documents/CS4204/CS4204-P2-DC/src/main.cpp > CMakeFiles/CS4204_P2_DC.dir/main.cpp.i

CMakeFiles/CS4204_P2_DC.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CS4204_P2_DC.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lewys/Documents/CS4204/CS4204-P2-DC/src/main.cpp -o CMakeFiles/CS4204_P2_DC.dir/main.cpp.s

CMakeFiles/CS4204_P2_DC.dir/Task.cpp.o: CMakeFiles/CS4204_P2_DC.dir/flags.make
CMakeFiles/CS4204_P2_DC.dir/Task.cpp.o: ../Task.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CS4204_P2_DC.dir/Task.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CS4204_P2_DC.dir/Task.cpp.o -c /home/lewys/Documents/CS4204/CS4204-P2-DC/src/Task.cpp

CMakeFiles/CS4204_P2_DC.dir/Task.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CS4204_P2_DC.dir/Task.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lewys/Documents/CS4204/CS4204-P2-DC/src/Task.cpp > CMakeFiles/CS4204_P2_DC.dir/Task.cpp.i

CMakeFiles/CS4204_P2_DC.dir/Task.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CS4204_P2_DC.dir/Task.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lewys/Documents/CS4204/CS4204-P2-DC/src/Task.cpp -o CMakeFiles/CS4204_P2_DC.dir/Task.cpp.s

CMakeFiles/CS4204_P2_DC.dir/mergesort.c.o: CMakeFiles/CS4204_P2_DC.dir/flags.make
CMakeFiles/CS4204_P2_DC.dir/mergesort.c.o: ../mergesort.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/CS4204_P2_DC.dir/mergesort.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/CS4204_P2_DC.dir/mergesort.c.o   -c /home/lewys/Documents/CS4204/CS4204-P2-DC/src/mergesort.c

CMakeFiles/CS4204_P2_DC.dir/mergesort.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CS4204_P2_DC.dir/mergesort.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lewys/Documents/CS4204/CS4204-P2-DC/src/mergesort.c > CMakeFiles/CS4204_P2_DC.dir/mergesort.c.i

CMakeFiles/CS4204_P2_DC.dir/mergesort.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CS4204_P2_DC.dir/mergesort.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lewys/Documents/CS4204/CS4204-P2-DC/src/mergesort.c -o CMakeFiles/CS4204_P2_DC.dir/mergesort.c.s

CMakeFiles/CS4204_P2_DC.dir/pfib.c.o: CMakeFiles/CS4204_P2_DC.dir/flags.make
CMakeFiles/CS4204_P2_DC.dir/pfib.c.o: ../pfib.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/CS4204_P2_DC.dir/pfib.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/CS4204_P2_DC.dir/pfib.c.o   -c /home/lewys/Documents/CS4204/CS4204-P2-DC/src/pfib.c

CMakeFiles/CS4204_P2_DC.dir/pfib.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CS4204_P2_DC.dir/pfib.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lewys/Documents/CS4204/CS4204-P2-DC/src/pfib.c > CMakeFiles/CS4204_P2_DC.dir/pfib.c.i

CMakeFiles/CS4204_P2_DC.dir/pfib.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CS4204_P2_DC.dir/pfib.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lewys/Documents/CS4204/CS4204-P2-DC/src/pfib.c -o CMakeFiles/CS4204_P2_DC.dir/pfib.c.s

CMakeFiles/CS4204_P2_DC.dir/qsort.c.o: CMakeFiles/CS4204_P2_DC.dir/flags.make
CMakeFiles/CS4204_P2_DC.dir/qsort.c.o: ../qsort.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/CS4204_P2_DC.dir/qsort.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/CS4204_P2_DC.dir/qsort.c.o   -c /home/lewys/Documents/CS4204/CS4204-P2-DC/src/qsort.c

CMakeFiles/CS4204_P2_DC.dir/qsort.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/CS4204_P2_DC.dir/qsort.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/lewys/Documents/CS4204/CS4204-P2-DC/src/qsort.c > CMakeFiles/CS4204_P2_DC.dir/qsort.c.i

CMakeFiles/CS4204_P2_DC.dir/qsort.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/CS4204_P2_DC.dir/qsort.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/lewys/Documents/CS4204/CS4204-P2-DC/src/qsort.c -o CMakeFiles/CS4204_P2_DC.dir/qsort.c.s

CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.o: CMakeFiles/CS4204_P2_DC.dir/flags.make
CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.o: ../DCSkeleton.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.o -c /home/lewys/Documents/CS4204/CS4204-P2-DC/src/DCSkeleton.cpp

CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lewys/Documents/CS4204/CS4204-P2-DC/src/DCSkeleton.cpp > CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.i

CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lewys/Documents/CS4204/CS4204-P2-DC/src/DCSkeleton.cpp -o CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.s

# Object files for target CS4204_P2_DC
CS4204_P2_DC_OBJECTS = \
"CMakeFiles/CS4204_P2_DC.dir/main.cpp.o" \
"CMakeFiles/CS4204_P2_DC.dir/Task.cpp.o" \
"CMakeFiles/CS4204_P2_DC.dir/mergesort.c.o" \
"CMakeFiles/CS4204_P2_DC.dir/pfib.c.o" \
"CMakeFiles/CS4204_P2_DC.dir/qsort.c.o" \
"CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.o"

# External object files for target CS4204_P2_DC
CS4204_P2_DC_EXTERNAL_OBJECTS =

CS4204_P2_DC: CMakeFiles/CS4204_P2_DC.dir/main.cpp.o
CS4204_P2_DC: CMakeFiles/CS4204_P2_DC.dir/Task.cpp.o
CS4204_P2_DC: CMakeFiles/CS4204_P2_DC.dir/mergesort.c.o
CS4204_P2_DC: CMakeFiles/CS4204_P2_DC.dir/pfib.c.o
CS4204_P2_DC: CMakeFiles/CS4204_P2_DC.dir/qsort.c.o
CS4204_P2_DC: CMakeFiles/CS4204_P2_DC.dir/DCSkeleton.cpp.o
CS4204_P2_DC: CMakeFiles/CS4204_P2_DC.dir/build.make
CS4204_P2_DC: CMakeFiles/CS4204_P2_DC.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable CS4204_P2_DC"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CS4204_P2_DC.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CS4204_P2_DC.dir/build: CS4204_P2_DC

.PHONY : CMakeFiles/CS4204_P2_DC.dir/build

CMakeFiles/CS4204_P2_DC.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CS4204_P2_DC.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CS4204_P2_DC.dir/clean

CMakeFiles/CS4204_P2_DC.dir/depend:
	cd /home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lewys/Documents/CS4204/CS4204-P2-DC/src /home/lewys/Documents/CS4204/CS4204-P2-DC/src /home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug /home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug /home/lewys/Documents/CS4204/CS4204-P2-DC/src/cmake-build-debug/CMakeFiles/CS4204_P2_DC.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CS4204_P2_DC.dir/depend

