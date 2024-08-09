GLAD_C=/usr/src/glad.c

INCLUDES=-I./ -I./include
SOURCES=$(shell find -name "*.cpp")
OBJ=$(patsubst ./%.cpp, obj/%.o, $(SOURCES))
DEBUG_OBJ=$(OBJ:obj/%=debug/%)
DEPENDENCIES=$(OBJ:%.o=%.d) $(DEBUG_OBJ:%.o=%.d)


bin/VoxelTerrain: $(OBJ) obj/glad.o
	@g++ -Wall $^ -O2 -o $@ -lglfw
	@cp shaders/* bin/shaders

debug/VoxelTerrain: $(DEBUG_OBJ) obj/glad.o
	@g++ -Wall $^ -g -o $@ -lglfw

run: bin/VoxelTerrain
	@./bin/VoxelTerrain

valgrind: debug/VoxelTerrain
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./debug

clean:
	@find . -type f -name '*.o' -delete
	@find . -type f -name '*.d' -delete
	@rm -f bin/VoxelTerrain debug/VoxelTerrain
	@rm -f bin/shaders/*.glsl

.PHONY: run valgrind clean


obj/%.o: %.cpp
	@g++ -Wall -c $< $(INCLUDES) -O2 -o $@ -MMD -MP -MF obj/$*.d

debug/%.o: %.cpp
	@g++ -Wall -c $< $(INCLUDES) -g -o $@ -MMD -MP -MF debug/$*.d

obj/glad.o: $(GLAD_C)
	@gcc -c $< -O2 -o $@

include $(wildcard $(DEPENDENCIES))