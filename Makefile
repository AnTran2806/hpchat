CXX=g++
CFLAGS=-std=c++14 -Wall -Wextra -pthread
OBJ_DIR=./obj

# Find all .cpp files in ./src/ChatAppServer directory
SRC_CHATAPPSERVER = $(shell find ./src/ChatAppServer -name "*.cpp")
SRC_CHATAPPCLIENT = $(shell find ./src/ChatAppClient -name "*.cpp")

# Replace .cpp with .o and place in ./obj/ChatAppServer directory
OBJ_CHATAPPSERVER = $(patsubst ./src/ChatAppServer/%.cpp, $(OBJ_DIR)/ChatAppServer/%.o, $(SRC_CHATAPPSERVER))
OBJ_CHATAPPCLIENT = $(patsubst ./src/ChatAppClient/%.cpp, $(OBJ_DIR)/ChatAppClient/%.o, $(SRC_CHATAPPCLIENT))

# Rule to build both ChatAppServer and ChatAppClient executables
all: ChatAppServer ChatAppClient

ChatAppServer: $(OBJ_CHATAPPSERVER)
	$(CXX) $(CFLAGS) -o ChatAppServer $(OBJ_CHATAPPSERVER)

ChatAppClient: $(OBJ_CHATAPPCLIENT)
	$(CXX) $(CFLAGS) -o ChatAppClient $(OBJ_CHATAPPCLIENT)

# Rule to compile ChatAppServer source files into object files
$(OBJ_DIR)/ChatAppServer/%.o: ./src/ChatAppServer/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c -o $@ $<

# Rule to compile ChatAppClient source files into object files
$(OBJ_DIR)/ChatAppClient/%.o: ./src/ChatAppClient/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c -o $@ $<

# Rule to clean all compiled files and executables
clean: clean-ChatAppClient clean-ChatAppServer
	rm -rf $(OBJ_DIR)

clean-ChatAppClient:
	rm -f ./ChatAppClient

clean-ChatAppServer:
	rm -f ./ChatAppServer
