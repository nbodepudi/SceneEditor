# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/nagasai/Downloads/course_graphics/base/Assignment_2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/nagasai/Downloads/course_graphics/base/Assignment_2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/main_positions_bin.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/main_positions_bin.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main_positions_bin.dir/flags.make

CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.o: CMakeFiles/main_positions_bin.dir/flags.make
CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.o: ../extra/main_positions.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/nagasai/Downloads/course_graphics/base/Assignment_2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.o -c /Users/nagasai/Downloads/course_graphics/base/Assignment_2/extra/main_positions.cpp

CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/nagasai/Downloads/course_graphics/base/Assignment_2/extra/main_positions.cpp > CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.i

CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/nagasai/Downloads/course_graphics/base/Assignment_2/extra/main_positions.cpp -o CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.s

CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.o: CMakeFiles/main_positions_bin.dir/flags.make
CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.o: ../src/Helpers.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/nagasai/Downloads/course_graphics/base/Assignment_2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.o -c /Users/nagasai/Downloads/course_graphics/base/Assignment_2/src/Helpers.cpp

CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/nagasai/Downloads/course_graphics/base/Assignment_2/src/Helpers.cpp > CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.i

CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/nagasai/Downloads/course_graphics/base/Assignment_2/src/Helpers.cpp -o CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.s

# Object files for target main_positions_bin
main_positions_bin_OBJECTS = \
"CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.o" \
"CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.o"

# External object files for target main_positions_bin
main_positions_bin_EXTERNAL_OBJECTS =

main_positions_bin: CMakeFiles/main_positions_bin.dir/extra/main_positions.cpp.o
main_positions_bin: CMakeFiles/main_positions_bin.dir/src/Helpers.cpp.o
main_positions_bin: CMakeFiles/main_positions_bin.dir/build.make
main_positions_bin: libglfw3.a
main_positions_bin: CMakeFiles/main_positions_bin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/nagasai/Downloads/course_graphics/base/Assignment_2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable main_positions_bin"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main_positions_bin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main_positions_bin.dir/build: main_positions_bin
.PHONY : CMakeFiles/main_positions_bin.dir/build

CMakeFiles/main_positions_bin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main_positions_bin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main_positions_bin.dir/clean

CMakeFiles/main_positions_bin.dir/depend:
	cd /Users/nagasai/Downloads/course_graphics/base/Assignment_2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/nagasai/Downloads/course_graphics/base/Assignment_2 /Users/nagasai/Downloads/course_graphics/base/Assignment_2 /Users/nagasai/Downloads/course_graphics/base/Assignment_2/cmake-build-debug /Users/nagasai/Downloads/course_graphics/base/Assignment_2/cmake-build-debug /Users/nagasai/Downloads/course_graphics/base/Assignment_2/cmake-build-debug/CMakeFiles/main_positions_bin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main_positions_bin.dir/depend

