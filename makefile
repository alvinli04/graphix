# Directories
SRC := ./src
OBJ := ./obj
BIN := ./bin
IMG := ./img

# Files
SRC_FILES := $(wildcard $(SRC)/*.cpp)
OBJ_FILES := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRC_FILES))
BIN_FILES := $(BIN)/graphix

# Making Executable And Running
all: $(OBJ_FILES)
	python3 src/compiler/script.py pic.mdl
	mkdir -p $(BIN)
	mkdir -p $(IMG)
	g++ -std=c++17 -o $(BIN_FILES) $^
	$(BIN_FILES)

# Making Object Files
$(OBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(OBJ)
	g++ -std=c++17 -o $@ -c $<

# Cleaning Files
clean:
	rm -rf $(OBJ);
	rm -rf $(BIN);
	rm -rf $(IMG);
	rm src/compiler/mdl.sym;
	rm src/compiler/mdl.cmd;
	rm -rf src/compiler/__pycache__
	rm src/compiler/*out src/compiler/parsetab.py

test:
	rm -r src/compiler/*out src/compiler/parsetab.py
