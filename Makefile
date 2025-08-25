NAME=VoxelTerrain
INCLUDES=-Iinclude
SOURCES=$(shell find src -name "*.cpp")
OBJ=$(SOURCES:src/%.cpp=obj/%.o)
DEBUG_OBJ=$(OBJ:obj/%=debug/%)
DIRECTORIES=$(sort $(dir $(OBJ) $(DEBUG_OBJ))) bin/ bin/shaders/
DEPENDENCIES=$(OBJ:%.o=%.d) $(DEBUG_OBJ:%.o=%.d)
LIBRARIES=-lglfw
OPTI=-O2
GLAD_C=/usr/local/src/glad/glad.c


bin: $(DIRECTORIES) bin/$(NAME)

debug: $(DIRECTORIES) debug/$(NAME)


bin/$(NAME): $(OBJ) obj/glad.o
	@echo "Linking..."
	@g++ -Wall $^ $(OPTI) -o $@ $(LIBRARIES)
	@cp shaders/* bin/shaders

debug/$(NAME): $(DEBUG_OBJ) obj/glad.o
	@echo "Linking (debug)..."
	@g++ -Wall $^ -g -o $@ $(LIBRARIES)

obj/%.o: src/%.cpp
	@echo "Compiling $*..."
	@g++ -Wall -c $< $(INCLUDES) $(OPTI) -o $@ -MMD -MP -MF $(@:.o=.d)

debug/%.o: src/%.cpp
	@echo "Compiling $* (debug)..."
	@g++ -Wall -c $< $(INCLUDES) -g -o $@ -MMD -MP -MF $(@:.o=.d)

obj/glad.o: $(GLAD_C)
	@echo "Compiling glad..."
	@g++ -c $< $(OPTI) -o $@

%/: 
	@mkdir -p $@


run: bin
	@echo "Running..."
	@./bin/$(NAME)

valgrind: debug
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./debug/$(NAME)

clean:
	@rm -fr bin/* obj/* debug/*


.PHONY: bin debug run valgrind clean

include $(wildcard $(DEPENDENCIES))