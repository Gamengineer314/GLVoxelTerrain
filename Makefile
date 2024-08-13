GLAD_C=/usr/src/glad/glad.c

INCLUDES=-I. -I./include
SOURCES=$(shell find -name "*.cpp")
OBJ=$(patsubst ..%.cpp,obj/%.o,$(subst /,.,$(SOURCES)))
DEBUG_OBJ=$(OBJ:obj/%=debug/%)
DEPENDENCIES=$(OBJ:%.o=%.d) $(DEBUG_OBJ:%.o=%.d)
OPTI=-O2


bin/VoxelTerrain: $(OBJ) obj/glad.o
	@g++ -Wall $^ $(OPTI) -o $@ -lglfw3
	@cp shaders/* bin/shaders

debug/VoxelTerrain: $(DEBUG_OBJ) obj/glad.o
	@g++ -Wall $^ -g -o $@ -lglfw

run: bin/VoxelTerrain
	@$<

valgrind: debug/VoxelTerrain
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$<

clean:
	@find . -name '*.o' -delete
	@find . -name '*.d' -delete
	@rm -f bin/VoxelTerrain debug/VoxelTerrain
	@rm -f bin/shaders/*.glsl

.PHONY: run valgrind clean


.SECONDEXPANSION:

$(OBJ): $$(patsubst obj/%/o,%.cpp,$$(subst .,/,$$@))
	@g++ -Wall -c $< $(INCLUDES) $(OPTI) -o $@ -MMD -MP -MF $(@:.o=.d)

$(DEBUG_OBJ): $$(patsubst debug/%/o,%.cpp,$$(subst .,/,$$@))
	@g++ -Wall -c $< $(INCLUDES) -g -o $@ -MMD -MP -MF $(@:.o=.d)

obj/glad.o: $(GLAD_C)
	@gcc -c $< $(OPTI) -o $@

include $(wildcard $(DEPENDENCIES))