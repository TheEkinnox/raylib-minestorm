CFLAGS = -Iinclude -I$(LIBMY_DIR)/include -Wall -Wextra -Wshadow -pedantic
DEP_FLAGS = -MT $(OBJ_DIR)/$(@F) -MMD -MP -MF $(DEP_DIR)/$(*F).d
LDFLAGS = -L$(LIBMY_DIR) -lmy -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
DEBUG_FLAGS = -ggdb3 -O0 -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG
SRC_DIR = src
LIBMY_DIR = libmy
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
DEP_DIR = $(BUILD_DIR)/deps
ASSETS_DIR = assets
LIB = $(LIBMY_DIR)/libmy.a
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(patsubst $(SRC_DIR)/%.c, $(DEP_DIR)/%.d, $(SRCS))
EXE_NAME = minestorm
EXE = $(BUILD_DIR)/$(EXE_NAME)

#Debug constants
DEBUG_BUILD_DIR = build/debug
DEBUG_EXE = $(DEBUG_BUILD_DIR)/$(EXE_NAME)_debug

#Release constants
RELEASE_BUILD_DIR = build/release
RELEASE_EXE = $(RELEASE_BUILD_DIR)/$(EXE_NAME)_release

.PHONY: clean all libmy re check debug release run dbgrun rlsrun gdb
all: libmy $(EXE)

libmy:
	$(MAKE) --directory=$(LIBMY_DIR)

clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) clean --directory=$(LIBMY_DIR)

re: clean all

run: all
	./$(EXE)

dbgrun: debug
	./$(DEBUG_EXE)

rlsrun: release
	./$(RELEASE_EXE)

gdb: debug
	gdb -q ./$(DEBUG_EXE)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: BUILD_DIR = $(DEBUG_BUILD_DIR)
debug: EXE = $(DEBUG_EXE)
debug: all

release: CFLAGS += $(RELEASE_FLAGS)
release: BUILD_DIR = $(RELEASE_BUILD_DIR)
release: EXE = $(RELEASE_EXE)
release: all

check: debug
	valgrind -s --leak-check=full --track-origins=yes --show-leak-kinds=all $(DEBUG_EXE)

$(EXE): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	gcc $(OBJS) -o $(EXE) $(CFLAGS) $(LDFLAGS)
	cp -r $(ASSETS_DIR) $(BUILD_DIR)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(DEP_DIR)/%.d
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DEP_DIR)
	gcc -c $< -o $(OBJ_DIR)/$(@F) $(DEP_FLAGS) $(CFLAGS)

$(DEPS):

include $(wildcard $(DEPS))
