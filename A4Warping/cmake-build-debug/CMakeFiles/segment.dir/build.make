# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.12

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.2.5\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.2.5\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/segment.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/segment.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/segment.dir/flags.make

CMakeFiles/segment.dir/segment/main.cpp.obj: CMakeFiles/segment.dir/flags.make
CMakeFiles/segment.dir/segment/main.cpp.obj: CMakeFiles/segment.dir/includes_CXX.rsp
CMakeFiles/segment.dir/segment/main.cpp.obj: ../segment/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/segment.dir/segment/main.cpp.obj"
	C:\nuwen\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\segment.dir\segment\main.cpp.obj -c "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\main.cpp"

CMakeFiles/segment.dir/segment/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segment.dir/segment/main.cpp.i"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\main.cpp" > CMakeFiles\segment.dir\segment\main.cpp.i

CMakeFiles/segment.dir/segment/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segment.dir/segment/main.cpp.s"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\main.cpp" -o CMakeFiles\segment.dir\segment\main.cpp.s

CMakeFiles/segment.dir/segment/segment.cpp.obj: CMakeFiles/segment.dir/flags.make
CMakeFiles/segment.dir/segment/segment.cpp.obj: CMakeFiles/segment.dir/includes_CXX.rsp
CMakeFiles/segment.dir/segment/segment.cpp.obj: ../segment/segment.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/segment.dir/segment/segment.cpp.obj"
	C:\nuwen\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\segment.dir\segment\segment.cpp.obj -c "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\segment.cpp"

CMakeFiles/segment.dir/segment/segment.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segment.dir/segment/segment.cpp.i"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\segment.cpp" > CMakeFiles\segment.dir\segment\segment.cpp.i

CMakeFiles/segment.dir/segment/segment.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segment.dir/segment/segment.cpp.s"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\segment.cpp" -o CMakeFiles\segment.dir\segment\segment.cpp.s

CMakeFiles/segment.dir/segment/Hough.cpp.obj: CMakeFiles/segment.dir/flags.make
CMakeFiles/segment.dir/segment/Hough.cpp.obj: CMakeFiles/segment.dir/includes_CXX.rsp
CMakeFiles/segment.dir/segment/Hough.cpp.obj: ../segment/Hough.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/segment.dir/segment/Hough.cpp.obj"
	C:\nuwen\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\segment.dir\segment\Hough.cpp.obj -c "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\Hough.cpp"

CMakeFiles/segment.dir/segment/Hough.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segment.dir/segment/Hough.cpp.i"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\Hough.cpp" > CMakeFiles\segment.dir\segment\Hough.cpp.i

CMakeFiles/segment.dir/segment/Hough.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segment.dir/segment/Hough.cpp.s"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\Hough.cpp" -o CMakeFiles\segment.dir\segment\Hough.cpp.s

CMakeFiles/segment.dir/segment/warp.cpp.obj: CMakeFiles/segment.dir/flags.make
CMakeFiles/segment.dir/segment/warp.cpp.obj: CMakeFiles/segment.dir/includes_CXX.rsp
CMakeFiles/segment.dir/segment/warp.cpp.obj: ../segment/warp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/segment.dir/segment/warp.cpp.obj"
	C:\nuwen\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\segment.dir\segment\warp.cpp.obj -c "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\warp.cpp"

CMakeFiles/segment.dir/segment/warp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segment.dir/segment/warp.cpp.i"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\warp.cpp" > CMakeFiles\segment.dir\segment\warp.cpp.i

CMakeFiles/segment.dir/segment/warp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segment.dir/segment/warp.cpp.s"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\warp.cpp" -o CMakeFiles\segment.dir\segment\warp.cpp.s

CMakeFiles/segment.dir/segment/Projection.cpp.obj: CMakeFiles/segment.dir/flags.make
CMakeFiles/segment.dir/segment/Projection.cpp.obj: CMakeFiles/segment.dir/includes_CXX.rsp
CMakeFiles/segment.dir/segment/Projection.cpp.obj: ../segment/Projection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/segment.dir/segment/Projection.cpp.obj"
	C:\nuwen\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\segment.dir\segment\Projection.cpp.obj -c "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\Projection.cpp"

CMakeFiles/segment.dir/segment/Projection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segment.dir/segment/Projection.cpp.i"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\Projection.cpp" > CMakeFiles\segment.dir\segment\Projection.cpp.i

CMakeFiles/segment.dir/segment/Projection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segment.dir/segment/Projection.cpp.s"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\Projection.cpp" -o CMakeFiles\segment.dir\segment\Projection.cpp.s

CMakeFiles/segment.dir/segment/cutPiece.cpp.obj: CMakeFiles/segment.dir/flags.make
CMakeFiles/segment.dir/segment/cutPiece.cpp.obj: CMakeFiles/segment.dir/includes_CXX.rsp
CMakeFiles/segment.dir/segment/cutPiece.cpp.obj: ../segment/cutPiece.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/segment.dir/segment/cutPiece.cpp.obj"
	C:\nuwen\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\segment.dir\segment\cutPiece.cpp.obj -c "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\cutPiece.cpp"

CMakeFiles/segment.dir/segment/cutPiece.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segment.dir/segment/cutPiece.cpp.i"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\cutPiece.cpp" > CMakeFiles\segment.dir\segment\cutPiece.cpp.i

CMakeFiles/segment.dir/segment/cutPiece.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segment.dir/segment/cutPiece.cpp.s"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\cutPiece.cpp" -o CMakeFiles\segment.dir\segment\cutPiece.cpp.s

CMakeFiles/segment.dir/segment/prepareLearning.cpp.obj: CMakeFiles/segment.dir/flags.make
CMakeFiles/segment.dir/segment/prepareLearning.cpp.obj: CMakeFiles/segment.dir/includes_CXX.rsp
CMakeFiles/segment.dir/segment/prepareLearning.cpp.obj: ../segment/prepareLearning.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/segment.dir/segment/prepareLearning.cpp.obj"
	C:\nuwen\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\segment.dir\segment\prepareLearning.cpp.obj -c "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\prepareLearning.cpp"

CMakeFiles/segment.dir/segment/prepareLearning.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segment.dir/segment/prepareLearning.cpp.i"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\prepareLearning.cpp" > CMakeFiles\segment.dir\segment\prepareLearning.cpp.i

CMakeFiles/segment.dir/segment/prepareLearning.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segment.dir/segment/prepareLearning.cpp.s"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\prepareLearning.cpp" -o CMakeFiles\segment.dir\segment\prepareLearning.cpp.s

CMakeFiles/segment.dir/segment/equalization.cpp.obj: CMakeFiles/segment.dir/flags.make
CMakeFiles/segment.dir/segment/equalization.cpp.obj: CMakeFiles/segment.dir/includes_CXX.rsp
CMakeFiles/segment.dir/segment/equalization.cpp.obj: ../segment/equalization.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/segment.dir/segment/equalization.cpp.obj"
	C:\nuwen\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\segment.dir\segment\equalization.cpp.obj -c "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\equalization.cpp"

CMakeFiles/segment.dir/segment/equalization.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segment.dir/segment/equalization.cpp.i"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\equalization.cpp" > CMakeFiles\segment.dir\segment\equalization.cpp.i

CMakeFiles/segment.dir/segment/equalization.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segment.dir/segment/equalization.cpp.s"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\equalization.cpp" -o CMakeFiles\segment.dir\segment\equalization.cpp.s

CMakeFiles/segment.dir/segment/reshape.cpp.obj: CMakeFiles/segment.dir/flags.make
CMakeFiles/segment.dir/segment/reshape.cpp.obj: CMakeFiles/segment.dir/includes_CXX.rsp
CMakeFiles/segment.dir/segment/reshape.cpp.obj: ../segment/reshape.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/segment.dir/segment/reshape.cpp.obj"
	C:\nuwen\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\segment.dir\segment\reshape.cpp.obj -c "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\reshape.cpp"

CMakeFiles/segment.dir/segment/reshape.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segment.dir/segment/reshape.cpp.i"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\reshape.cpp" > CMakeFiles\segment.dir\segment\reshape.cpp.i

CMakeFiles/segment.dir/segment/reshape.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segment.dir/segment/reshape.cpp.s"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\reshape.cpp" -o CMakeFiles\segment.dir\segment\reshape.cpp.s

CMakeFiles/segment.dir/segment/canny.cpp.obj: CMakeFiles/segment.dir/flags.make
CMakeFiles/segment.dir/segment/canny.cpp.obj: CMakeFiles/segment.dir/includes_CXX.rsp
CMakeFiles/segment.dir/segment/canny.cpp.obj: ../segment/canny.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/segment.dir/segment/canny.cpp.obj"
	C:\nuwen\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\segment.dir\segment\canny.cpp.obj -c "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\canny.cpp"

CMakeFiles/segment.dir/segment/canny.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/segment.dir/segment/canny.cpp.i"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\canny.cpp" > CMakeFiles\segment.dir\segment\canny.cpp.i

CMakeFiles/segment.dir/segment/canny.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/segment.dir/segment/canny.cpp.s"
	C:\nuwen\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\segment\canny.cpp" -o CMakeFiles\segment.dir\segment\canny.cpp.s

# Object files for target segment
segment_OBJECTS = \
"CMakeFiles/segment.dir/segment/main.cpp.obj" \
"CMakeFiles/segment.dir/segment/segment.cpp.obj" \
"CMakeFiles/segment.dir/segment/Hough.cpp.obj" \
"CMakeFiles/segment.dir/segment/warp.cpp.obj" \
"CMakeFiles/segment.dir/segment/Projection.cpp.obj" \
"CMakeFiles/segment.dir/segment/cutPiece.cpp.obj" \
"CMakeFiles/segment.dir/segment/prepareLearning.cpp.obj" \
"CMakeFiles/segment.dir/segment/equalization.cpp.obj" \
"CMakeFiles/segment.dir/segment/reshape.cpp.obj" \
"CMakeFiles/segment.dir/segment/canny.cpp.obj"

# External object files for target segment
segment_EXTERNAL_OBJECTS =

segment.exe: CMakeFiles/segment.dir/segment/main.cpp.obj
segment.exe: CMakeFiles/segment.dir/segment/segment.cpp.obj
segment.exe: CMakeFiles/segment.dir/segment/Hough.cpp.obj
segment.exe: CMakeFiles/segment.dir/segment/warp.cpp.obj
segment.exe: CMakeFiles/segment.dir/segment/Projection.cpp.obj
segment.exe: CMakeFiles/segment.dir/segment/cutPiece.cpp.obj
segment.exe: CMakeFiles/segment.dir/segment/prepareLearning.cpp.obj
segment.exe: CMakeFiles/segment.dir/segment/equalization.cpp.obj
segment.exe: CMakeFiles/segment.dir/segment/reshape.cpp.obj
segment.exe: CMakeFiles/segment.dir/segment/canny.cpp.obj
segment.exe: CMakeFiles/segment.dir/build.make
segment.exe: CMakeFiles/segment.dir/linklibs.rsp
segment.exe: CMakeFiles/segment.dir/objects1.rsp
segment.exe: CMakeFiles/segment.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable segment.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\segment.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/segment.dir/build: segment.exe

.PHONY : CMakeFiles/segment.dir/build

CMakeFiles/segment.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\segment.dir\cmake_clean.cmake
.PHONY : CMakeFiles/segment.dir/clean

CMakeFiles/segment.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping" "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping" "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug" "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug" "C:\Users\Chen Sihang\Desktop\finalProject\A4Warping\cmake-build-debug\CMakeFiles\segment.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/segment.dir/depend
