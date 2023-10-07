PROJECT_NAME := RedNoise

BUILD_DIR := build

# Define the names of key files
SOURCE_FILE := src/$(PROJECT_NAME).cpp
OBJECT_FILE := $(BUILD_DIR)/$(PROJECT_NAME).o
EXECUTABLE := $(BUILD_DIR)/$(PROJECT_NAME)
SDW_DIR := ./libs/sdw/
GLM_DIR := ./libs/glm-0.9.7.2/
SDW_SOURCE_FILES := $(wildcard $(SDW_DIR)*.cpp)
SDW_OBJECT_FILES := $(patsubst $(SDW_DIR)%.cpp, $(BUILD_DIR)/%.o, $(SDW_SOURCE_FILES))

# Build settings
COMPILER := clang++
COMPILER_OPTIONS := -c -pipe -Wall -std=c++11 # If you have an older compiler, you might have to use -std=c++0x
DEBUG_OPTIONS := -ggdb -g3
FUSSY_OPTIONS := -Werror -pedantic
SANITIZER_OPTIONS := -O1 -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer
SPEEDY_OPTIONS := -Ofast -funsafe-math-optimizations -march=native
LINKER_OPTIONS :=

# Set up flags
SDW_COMPILER_FLAGS := -I$(SDW_DIR)
GLM_COMPILER_FLAGS := -I$(GLM_DIR)
# If you have a manual install of SDL, you might not have sdl2-config installed, so the following line might not work
# Compiler flags should look something like: -I/usr/local/include/SDL2 -D_THREAD_SAFE
SDL_COMPILER_FLAGS := $(shell sdl2-config --cflags)
# If you have a manual install of SDL, you might not have sdl2-config installed, so the following line might not work
# Linker flags should look something like: -L/usr/local/lib -lSDL2
SDL_LINKER_FLAGS := $(shell sdl2-config --libs)
SDW_LINKER_FLAGS := $(SDW_OBJECT_FILES)

default: debug

# Rule to compile and link for use with a debugger (although works fine even if you aren't using a debugger !)
debug: $(SDW_OBJECT_FILES)
	$(COMPILER) $(COMPILER_OPTIONS) $(DEBUG_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) $(DEBUG_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule to help find runtime errors (when you get a segmentation fault)
# NOTE: This needs the "Address Sanitizer" library to be installed in order to work (so it might not work on lab machines !)
diagnostic: $(SDW_OBJECT_FILES)
	$(COMPILER) $(COMPILER_OPTIONS) $(FUSSY_OPTIONS) $(SANITIZER_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) $(FUSSY_OPTIONS) $(SANITIZER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule to build for high performance executable (for manually testing interaction)
speedy: $(SDW_OBJECT_FILES)
	$(COMPILER) $(COMPILER_OPTIONS) $(SPEEDY_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) $(SPEEDY_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule to compile and link for final production release
production: $(SDW_OBJECT_FILES)
	$(COMPILER) $(COMPILER_OPTIONS) -o $(OBJECT_FILE) $(SOURCE_FILE) $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)
	$(COMPILER) $(LINKER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILE) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule for building all of the the DisplayWindow classes
$(BUILD_DIR)/%.o: $(SDW_DIR)%.cpp
	@mkdir -p $(BUILD_DIR)
	$(COMPILER) $(COMPILER_OPTIONS) -c -o $@ $^ $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

# Files to remove during clean
clean:
	rm $(BUILD_DIR)/*
