#@Brief: Build tests for the library
#@Author: Javier Niño Sánchez
#@Date: 10/04/2025

#directories and compiler options 
SRC_DIR = source/
HEADER_DIR = include/
OBJ_DIR = objects/
TEST_DIR = tests/
COMP = gcc
CXXFLAGS = -std=c11 -Wall -g -Winline -pedantic -I$(HEADER_DIR)

#Globals
SOURCES 		 := $(wildcard $(SRC_DIR)*.c)
HEADERS 		 := $(wildcard $(HEADER_DIR)*.h)
TESTS				 := $(wildcard $(TEST_DIR)*.c)
SRC_OBJECTS  := $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SOURCES))
TEST_OBJECTS := $(patsubst $(TEST_DIR)%.c, $(OBJ_DIR)%.o, $(TESTS))

TEST_NAMES := UTF8ToUTF16 UTF32conversionsTo InputCharacter

all: obj $(TEST_NAMES)
#Make the directory for the objects
obj:
	@mkdir -p $(OBJ_DIR)
#Link it all together
$(TEST_NAMES): %: $(OBJ_DIR)%.o $(SRC_OBJECTS)
	$(COMP) $(CXXFLAGS) -o $@ $^
#Compile source code into objects
$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADERS)
	$(COMP) -c $(CXXFLAGS) -o $@ $<
$(OBJ_DIR)%.o: $(TEST_DIR)%.c $(HEADERS)
	$(COMP) -c $(CXXFLAGS) -o $@ $<
#Clean everything
clean:
	rm $(OBJ_DIR)* $(TEST_NAMES)
	rmdir $(OBJ_DIR)
.PHONY: all clean
