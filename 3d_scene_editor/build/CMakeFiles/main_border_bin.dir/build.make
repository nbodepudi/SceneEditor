# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.21.4/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.21.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/nagasai/Downloads/course_graphics/base/Assignment_3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/nagasai/Downloads/course_graphics/base/Assignment_3/build

# Include any dependencies generated for this target.
include CMakeFiles/main_border_bin.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main_border_bin.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main_border_bin.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main_border_bin.dir/flags.make

CMakeFiles/main_border_bin.dir/extra/main_border.cpp.o: CMakeFiles/main_border_bin.dir/flags.make
CMakeFiles/main_border_bin.dir/extra/main_border.cpp.o: ../extra/main_border.cpp
CMakeFiles/main_border_bin.dir/extra/main_border.cpp.o: CMakeFiles/main_border_bin.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/nagasai/Downloads/course_graphics/base/Assignment_3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main_border_bin.dir/extra/main_border.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main_border_bin.dir/extra/main_border.cpp.o -MF CMakeFiles/main_border_bin.dir/extra/main_border.cpp.o.d -o CMakeFiles/main_border_bin.dir/extra/main_border.cpp.o -c /Users/nagasai/Downloads/course_graphics/base/Assignment_3/extra/main_border.cpp

CMakeFiles/main_border_bin.dir/extra/main_border.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main_border_bin.dir/extra/main_border.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/nagasai/Downloads/course_graphics/base/Assignment_3/extra/main_border.cpp > CMakeFiles/main_border_bin.dir/extra/main_border.cpp.i

CMakeFiles/main_border_bin.dir/extra/main_border.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main_border_bin.dir/extra/main_border.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/nagasai/Downloads/course_graphics/base/Assignment_3/extra/main_border.cpp -o CMakeFiles/main_border_bin.dir/extra/main_border.cpp.s

CMakeFiles/main_border_bin.dir/src/Helpers.cpp.o: CMakeFiles/main_border_bin.dir/flags.make
CMakeFiles/main_border_bin.dir/src/Helpers.cpp.o: ../src/Helpers.cpp
CMakeFiles/main_border_bin.dir/src/Helpers.cpp.o: CMakeFiles/main_border_bin.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/nagasai/Downloads/course_graphics/base/Assignment_3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main_border_bin.dir/src/Helpers.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main_border_bin.dir/src/Helpers.cpp.o -MF CMakeFiles/main_border_bin.dir/src/Helpers.cpp.o.d -o CMakeFiles/main_border_bin.dir/src/Helpers.cpp.o -c /Users/nagasai/Downloads/course_graphics/base/Assignment_3/src/Helpers.cpp

CMakeFiles/main_border_bin.dir/src/Helpers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main_border_bin.dir/src/Helpers.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/nagasai/Downloads/course_graphics/base/Assignment_3/src/Helpers.cpp > CMakeFiles/main_border_bin.dir/src/Helpers.cpp.i

CMakeFiles/main_border_bin.dir/src/Helpers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main_border_bin.dir/src/Helpers.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/nagasai/Downloads/course_graphics/base/Assignment_3/src/Helpers.cpp -o CMakeFiles/main_border_bin.dir/src/Helpers.cpp.s

# Object files for target main_border_bin
main_border_bin_OBJECTS = \
"CMakeFiles/main_border_bin.dir/extra/main_border.cpp.o" \
"CMakeFiles/main_border_bin.dir/src/Helpers.cpp.o"

# External object files for target main_border_bin
main_border_bin_EXTERNAL_OBJECTS =

main_border_bin: CMakeFiles/main_border_bin.dir/extra/main_border.cpp.o
main_border_bin: CMakeFiles/main_border_bin.dir/src/Helpers.cpp.o
main_border_bin: CMakeFiles/main_border_bin.dir/build.make
main_border_bin: libglfw3.a
main_border_bin: CMakeFiles/main_border_bin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/nagasai/Downloads/course_graphics/base/Assignment_3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable main_border_bin"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main_border_bin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main_border_bin.dir/build: main_border_bin
.PHONY : CMakeFiles/main_border_bin.dir/build

CMakeFiles/main_border_bin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main_border_bin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main_border_bin.dir/clean

CMakeFiles/main_border_bin.dir/depend:
	cd /Users/nagasai/Downloads/course_graphics/base/Assignment_3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/nagasai/Downloads/course_graphics/base/Assignment_3 /Users/nagasai/Downloads/course_graphics/base/Assignment_3 /Users/nagasai/Downloads/course_graphics/base/Assignment_3/build /Users/nagasai/Downloads/course_graphics/base/Assignment_3/build /Users/nagasai/Downloads/course_graphics/base/Assignment_3/build/CMakeFiles/main_border_bin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main_border_bin.dir/depend

