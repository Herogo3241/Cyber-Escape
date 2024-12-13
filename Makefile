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
SRC = main.cpp
TARGET = main
MYLIBS = src/MyRay.cpp src/Enemy.cpp src/Player.cpp src/Monitor.cpp src/MatrixEffect.cpp src/Board.cpp

# Platform-specific settings
ifeq ($(PLATFORM),Windows)
    EXE = $(TARGET).exe
    LIBS = -lraylib -lgdi32 -lwinmm
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

# Rules
.PHONY: build run clean release

build:
	$(CXX) -o $(EXE) $(SRC) $(MYLIBS) $(LIBS)

run: build
	$(RUN_CMD)

clean:
	$(CLEAN_CMD)

release:
	$(CXX) -o game $(SRC) $(LIBS) -O3
