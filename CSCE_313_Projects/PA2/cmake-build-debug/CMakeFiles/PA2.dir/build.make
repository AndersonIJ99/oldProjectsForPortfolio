# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /snap/clion/124/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/124/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/osboxes/CSCE_313-508/PA2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/osboxes/CSCE_313-508/PA2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/PA2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/PA2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PA2.dir/flags.make

CMakeFiles/PA2.dir/main.cpp.o: CMakeFiles/PA2.dir/flags.make
CMakeFiles/PA2.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/osboxes/CSCE_313-508/PA2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PA2.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PA2.dir/main.cpp.o -c /home/osboxes/CSCE_313-508/PA2/main.cpp

CMakeFiles/PA2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA2.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/osboxes/CSCE_313-508/PA2/main.cpp > CMakeFiles/PA2.dir/main.cpp.i

CMakeFiles/PA2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA2.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/osboxes/CSCE_313-508/PA2/main.cpp -o CMakeFiles/PA2.dir/main.cpp.s

# Object files for target PA2
PA2_OBJECTS = \
"CMakeFiles/PA2.dir/main.cpp.o"

# External object files for target PA2
PA2_EXTERNAL_OBJECTS =

PA2: CMakeFiles/PA2.dir/main.cpp.o
PA2: CMakeFiles/PA2.dir/build.make
PA2: CMakeFiles/PA2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/osboxes/CSCE_313-508/PA2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable PA2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PA2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PA2.dir/build: PA2

.PHONY : CMakeFiles/PA2.dir/build

CMakeFiles/PA2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PA2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PA2.dir/clean

CMakeFiles/PA2.dir/depend:
	cd /home/osboxes/CSCE_313-508/PA2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/osboxes/CSCE_313-508/PA2 /home/osboxes/CSCE_313-508/PA2 /home/osboxes/CSCE_313-508/PA2/cmake-build-debug /home/osboxes/CSCE_313-508/PA2/cmake-build-debug /home/osboxes/CSCE_313-508/PA2/cmake-build-debug/CMakeFiles/PA2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PA2.dir/depend

