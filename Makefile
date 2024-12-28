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
EMSCRIPTEN = emcc
SRC = src/main.cpp
TARGET = cyber_escape
MYLIBS = src/MyRay.cpp src/Enemy.cpp src/Player.cpp src/Monitor.cpp src/MatrixEffect.cpp src/Board.cpp src/levels/level1.cpp src/levels/level2.cpp src/levels/level3.cpp src/levels/Level.cpp src/GameManager.cpp 

# Common flags for release builds
RELEASE_FLAGS = -O3 -flto -march=native -DNDEBUG

# Platform-specific settings
ifeq ($(PLATFORM),Windows)
    EXE = $(TARGET).exe
    LIBS = -static -static-libgcc -static-libstdc++ -lraylib -lgdi32 -lwinmm -mwindows
    RUN_CMD = ./$(EXE)
    CLEAN_CMD = del /F /Q $(EXE)
else ifeq ($(PLATFORM),Linux)
    EXE = $(TARGET)
    LIBS = -static -Wl,--whole-archive -lraylib -Wl,--no-whole-archive -lm -ldl -lpthread -lGL -lX11
    RUN_CMD = ./$(EXE)
    CLEAN_CMD = rm -f $(EXE)
else ifeq ($(PLATFORM),macOS)
    EXE = $(TARGET)
    LIBS = -static -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    RUN_CMD = ./$(EXE)
    CLEAN_CMD = rm -f $(EXE)
endif

# Emscripten-specific settings
WEB_TARGET = web/game.html
WEB_FLAGS = -Wall -std=c++14 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -Os \
            -I. -I C:/raylib/raylib/src -I C:/raylib/raylib/src/external -L. -L C:/raylib/raylib/src \
            -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=134217728 -s FORCE_FILESYSTEM=1 \
            --preload-file assets --shell-file C:/raylib/raylib/src/shell.html \
            C:/raylib/raylib/src/web/libraylib.a -DPLATFORM_WEB \
            -s 'EXPORTED_FUNCTIONS=["_free","_malloc","_main"]' \
            -s EXPORTED_RUNTIME_METHODS=ccall -s ASSERTIONS=1 -s STACK_SIZE=5242880 -s ALLOW_MEMORY_GROWTH


WEB_CLEAN_CMD = rm -f web/game.html web/game.js web/game.wasm web/game.data

# Rules
.PHONY: build run clean release web web_clean

build:
	$(CXX) -o $(EXE) $(SRC) $(MYLIBS) $(LIBS)

run: build
	$(RUN_CMD)

clean:
	$(CLEAN_CMD)

release:
	$(CXX) $(RELEASE_FLAGS) -o $(TARGET)_release $(SRC) $(MYLIBS) $(LIBS)

web:
	$(EMSCRIPTEN) -o $(WEB_TARGET) $(SRC) $(MYLIBS) $(WEB_FLAGS)

web_clean:
	$(WEB_CLEAN_CMD)
