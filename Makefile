CXX=g++
CFLAGS=-std=c++14 -Wall -Wextra -pthread
OBJ_DIR=./obj

# Find all .cpp files in ./src/server directory
SRC_SERVER = $(shell find ./src/server -name "*.cpp")
SRC_CLIENT = $(shell find ./src/client -name "*.cpp")

# Replace .cpp with .o and place in ./obj/server directory
OBJ_SERVER = $(patsubst ./src/server/%.cpp, $(OBJ_DIR)/server/%.o, $(SRC_SERVER))
OBJ_CLIENT = $(patsubst ./src/client/%.cpp, $(OBJ_DIR)/client/%.o, $(SRC_CLIENT))

# Rule to build both server and client executables
all: server client

server: $(OBJ_SERVER)
	$(CXX) $(CFLAGS) -o server $(OBJ_SERVER)

client: $(OBJ_CLIENT)
	$(CXX) $(CFLAGS) -o client $(OBJ_CLIENT)

# Rule to compile server source files into object files
$(OBJ_DIR)/server/%.o: ./src/server/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c -o $@ $<

# Rule to compile client source files into object files
$(OBJ_DIR)/client/%.o: ./src/client/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c -o $@ $<

# Rule to clean all compiled files and executables
clean: clean-client clean-server
	rm -rf $(OBJ_DIR)

clean-client:
	rm -f ./client

clean-server:
	rm -f ./server
