# Directories
SRC := ./src
OBJ := ./obj
BIN := ./bin

# Files
SRC_FILES := $(wildcard $(SRC)/*.cpp)
OBJ_FILES := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRC_FILES))
BIN_FILES := $(BIN)/graphix

# Making Executable And Running
all: $(OBJ_FILES)
	mkdir -p $(BIN)
	g++ -std=c++14 -o $(BIN_FILES) $^
	$(BIN_FILES)

# Making Object Files
$(OBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(OBJ)
	g++ -std=c++14 -o $@ -c $<

# Cleaning Files
clean:
	rm -rf $(OBJ);
	rm -rf $(BIN);
	rm *.ppm;
	rm *.png;
