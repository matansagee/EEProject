# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/matan/ClionProjects/EEProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/matan/ClionProjects/EEProject

# Include any dependencies generated for this target.
include CMakeFiles/EEProject.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/EEProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/EEProject.dir/flags.make

CMakeFiles/EEProject.dir/main.c.o: CMakeFiles/EEProject.dir/flags.make
CMakeFiles/EEProject.dir/main.c.o: main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/matan/ClionProjects/EEProject/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/EEProject.dir/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/EEProject.dir/main.c.o   -c /home/matan/ClionProjects/EEProject/main.c

CMakeFiles/EEProject.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/EEProject.dir/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/matan/ClionProjects/EEProject/main.c > CMakeFiles/EEProject.dir/main.c.i

CMakeFiles/EEProject.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/EEProject.dir/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/matan/ClionProjects/EEProject/main.c -o CMakeFiles/EEProject.dir/main.c.s

CMakeFiles/EEProject.dir/main.c.o.requires:
.PHONY : CMakeFiles/EEProject.dir/main.c.o.requires

CMakeFiles/EEProject.dir/main.c.o.provides: CMakeFiles/EEProject.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/EEProject.dir/build.make CMakeFiles/EEProject.dir/main.c.o.provides.build
.PHONY : CMakeFiles/EEProject.dir/main.c.o.provides

CMakeFiles/EEProject.dir/main.c.o.provides.build: CMakeFiles/EEProject.dir/main.c.o

# Object files for target EEProject
EEProject_OBJECTS = \
"CMakeFiles/EEProject.dir/main.c.o"

# External object files for target EEProject
EEProject_EXTERNAL_OBJECTS =

EEProject: CMakeFiles/EEProject.dir/main.c.o
EEProject: CMakeFiles/EEProject.dir/build.make
EEProject: CMakeFiles/EEProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable EEProject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EEProject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/EEProject.dir/build: EEProject
.PHONY : CMakeFiles/EEProject.dir/build

CMakeFiles/EEProject.dir/requires: CMakeFiles/EEProject.dir/main.c.o.requires
.PHONY : CMakeFiles/EEProject.dir/requires

CMakeFiles/EEProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/EEProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/EEProject.dir/clean

CMakeFiles/EEProject.dir/depend:
	cd /home/matan/ClionProjects/EEProject && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/matan/ClionProjects/EEProject /home/matan/ClionProjects/EEProject /home/matan/ClionProjects/EEProject /home/matan/ClionProjects/EEProject /home/matan/ClionProjects/EEProject/CMakeFiles/EEProject.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/EEProject.dir/depend

