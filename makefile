# Directories
SRC := ./src
OBJ := ./obj
BIN := ./bin

# Files
SRC_FILES := $(wildcard $(SRC)/*.cpp)
OBJ_FILES := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRC_FILES))
BIN_FILES := $(BIN)/main

# Making Executable
all: $(OBJ_FILES)
	mkdir -p $(BIN)
	g++ -o $(BIN_FILES) $^

# Making Object Files
$(OBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(OBJ)
	g++ -o $@ -c $<

# Running Program
run: $(BIN_FILES)
	$(BIN_FILES)
	convert test.ppm test.png;
	display test.png;

# Cleaning Files
clean:
	rm -rf $(OBJ);
	rm -rf $(BIN);
	rm *.ppm;
	rm *.png;
