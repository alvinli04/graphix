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
all: $(OBJ_FILES) lex.yy.c y.tab.c y.tab.h
	mkdir -p $(BIN)
	mkdir -p $(IMG)
	g++ -std=c++17 -o $(BIN_FILES) $^
	g++ -std=c++17 -o mdl lex.yy.c y.tab.c $(OBJ_FILES) -lm
	./mdl face.mdl

# Making Object Files
$(OBJ)/%.o: $(SRC)/%.cpp
	mkdir -p $(OBJ)
	g++ -std=c++17 -o $@ -c $<

# Lex and yacc stuff
lex.yy.c: mdl.l y.tab.h 
	flex -I mdl.l

y.tab.c: mdl.y symtab.hpp parser.hpp
	bison -d -y mdl.y

y.tab.h: mdl.y 
	bison -d -y mdl.y

# Cleaning Files
clean:
	rm -rf $(OBJ);
	rm -rf $(BIN);
	rm -rf $(IMG);
	rm y.tab.c y.tab.h;
	rm lex.yy.c;
	rm -rf mdl.dSYM;
