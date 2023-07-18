CC = gcc

BIN_DIR = bin
LIB_DIR = $(BIN_DIR)/libs
BLD_DIR = bin-int/$(TARGET)
SRC_DIR = $(TARGET)/src

NAME = $(TARGET_BIN_NAME)
SOURCES = $(shell find $(SRC_DIR) -type f -name *.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BLD_DIR)/%.o, $(SOURCES))
INCLUDE = -I./include


# === GENERIC RULES ===========================================================
executable : $(OBJECTS)
	@echo "   $(CC) -o $(BIN_DIR)/$(NAME) $(OBJECTS) -L./$(LIB_DIR) $(LIBS)"
	@$(CC) -o $(BIN_DIR)/$(NAME) $(OBJECTS) -L./$(LIB_DIR) $(LIBS)

library : $(OBJECTS)
	@echo "   $(CC) -shared -o $(LIB_DIR)/lib$(NAME).so $(OBJECTS) -L./$(LIB_DIR) $(LIBS)"
	@$(CC) -shared -o $(LIB_DIR)/lib$(NAME).so $(OBJECTS) -L./$(LIB_DIR) $(LIBS)

$(BLD_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "   $(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<"
	@$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<


# === Miscellaneous Rules =====================================================
.PHONY : clean
clean:
	@echo "   rm -rf ./$(BLD_DIR)/*"
	@rm -rf ./$(BLD_DIR)/*
	