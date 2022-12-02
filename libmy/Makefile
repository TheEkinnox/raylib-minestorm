CFLAGS = -Iinclude -Wall -Wextra -Wshadow -pedantic
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d
OUTPUT = -o $@
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
DEP_DIR = $(BUILD_DIR)/deps
LIB = libmy.a
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(patsubst $(SRC_DIR)/%.c, $(DEP_DIR)/%.d, $(SRCS))

.PHONY : clean all re
all : $(LIB)

clean :
	rm -rf $(BUILD_DIR) $(LIB)

re : clean all

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(DEP_DIR)/%.d
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DEP_DIR)
	gcc -c $< $(OUTPUT) $(DEP_FLAGS) $(CFLAGS)

$(LIB) : $(OBJS)
	@mkdir -p $(BUILD_DIR)
	ar rcs $@ $?

$(DEPS):

include $(wildcard $(DEPS))
