CC = g++
CFLAGS = -Wall -Wextra -pedantic -ldl -Iincludes/ -lGL -lglfw 
TARGET = main
OBJ_DIR = bin/
SRC_DIR = src/

SRC_FILES = $(shell find $(SRC_DIR) -type f -name '*.cpp')
SRC = $(SRC_FILES)
OBJS = $(SRC_FILES:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

.PHONY: result

result: $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(CFLAGS) includes/glad/glad.c $?

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp | $(OBJ_DIR)
	$(CC) -c -o $@ $(CFLAGS) $<

run: $(TARGET)
	@./$(TARGET)

clean:
	rm -rf $(OBJ_DIR)
	rm main