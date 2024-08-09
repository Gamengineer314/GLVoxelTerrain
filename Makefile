INCLUDES=-I./ -I./include
HEADERS=$(shell find include -name "*.hpp")
SOURCES=main.cpp $(shell find src -name "*.cpp")
SHADERS=$(shell find shaders -name "*.glsl")
GLAD_C=/usr/src/glad.c

bin/VoxelTerrain: $(HEADERS) $(SOURCES) $(SHADERS)
	@echo "Building program..."
	@g++ -Wall $(GLAD_C) $(SOURCES) $(INCLUDES) -O2 -o $@ -lglfw
	@echo "Copying shaders..."
	@cp shaders/* bin/shaders

debug: $(HEADERS) $(SOURCES) $(SHADERS)
	@echo "Building program in debug mode..."
	@g++ -Wall $(GLAD_C) $(SOURCES) $(INCLUDES) -g -o $@ -lglfw

run: bin/VoxelTerrain
	@echo "Running program..."
	@./bin/VoxelTerrain

valgrind: debug
	@echo "Running program with Valgrind..."
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./debug