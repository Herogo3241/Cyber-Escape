# Detect platform
ifeq ($(OS),Windows_NT)
    PLATFORM = Windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        PLATFORM = Linux
    else ifeq ($(UNAME_S),Darwin)
        PLATFORM = macOS
    endif
endif

# Compiler and source files
CXX = g++
SRC = src/main.cpp
TARGET = cyber_escape
MYLIBS = src/MyRay.cpp src/Enemy.cpp src/Player.cpp src/Monitor.cpp src/MatrixEffect.cpp src/Board.cpp src/levels/level1.cpp src/levels/level2.cpp src/levels/level3.cpp src/levels/Level.cpp src/GameManager.cpp 

# Platform-specific settings
ifeq ($(PLATFORM),Windows)
    EXE = $(TARGET).exe
    LIBS = -lraylib -lgdi32 -lwinmm -mwindows
    RUN_CMD = ./$(EXE)
    CLEAN_CMD = del /F /Q $(EXE)
else ifeq ($(PLATFORM),Linux)
    EXE = $(TARGET)
    LIBS = -lraylib -lm -ldl -lpthread -lGL -lX11
    RUN_CMD = ./$(EXE)
    CLEAN_CMD = rm -f $(EXE)
else ifeq ($(PLATFORM),macOS)
    EXE = $(TARGET)
    LIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    RUN_CMD = ./$(EXE)
    CLEAN_CMD = rm -f $(EXE)
endif

# Emscripten-specific settings
ifeq ($(PLATFORM),Linux)
    EMSCRIPTEN = emcc
    EMSCRIPTEN_FLAGS = -s USE_GLFW=3 -s USE_WEBGL2=1 -s ASYNCIFY --preload-file assets@/assets -O3
    EMSCRIPTEN_TARGET = index.html
    EMSCRIPTEN_CLEAN_CMD = rm -f $(EMSCRIPTEN_TARGET) index.js index.wasm index.data
endif

# Rules
.PHONY: build run clean release web

# Build for platform (Linux, macOS, Windows)
build:
	$(CXX) -o $(EXE) $(SRC) $(MYLIBS) $(LIBS)

# Run the platform-specific build
run: build
	$(RUN_CMD)

# Clean platform-specific build
clean:
	$(CLEAN_CMD)

# Release optimized build for platform
release:
	$(CXX) -o game $(SRC) $(LIBS) -O3

# Build for the web (Emscripten)
web:
	$(EMSCRIPTEN) $(SRC) $(MYLIBS) $(EMSCRIPTEN_FLAGS) -o $(EMSCRIPTEN_TARGET)

# Clean the Emscripten build
web_clean:
	$(EMSCRIPTEN_CLEAN_CMD)
