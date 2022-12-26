# PROJECT CONFIG

СС=gcc
CC_FLAGS=-g -Wall
LD=gcc
LD_FLAGS=

PROJECT_NAME=ft_ssl

MAIN_SRC_DIRS=main main/md5 main/sha256
MAIN_INCLUDE_DIRS=main lib/libft/include
MAIN_EXE_NAME=ft_ssl

TEST_SRC_DIRS=main/md5 main/sha256 test/algo test/algo/tools test/algo/md5_tests test/algo/sha256_tests
TEST_INCLUDE_DIRS=test/algo main lib/libft/include
TEST_EXE_NAME=ft_ssl_algo_tests
ADDITIONAL_TEST=python3 test/cli/ft_ssl_cli_tests.py

LIBFT_PROJECT=lib/libft
LIBFT_DIR=lib/libft/build
LIBFT_NAME=ft

# TOOLS

DEBUG=$(info [DEBUG] $1)
SILENT=@

# CONSTANTS

BUILD_DIR=build
BUILD_OBJ_DIR=build/obj
BUILD_DEP_DIR=build/dep
BUILD_EXE_DIR=build

# DYNAMIC CONFIG

MAIN_LOG=$(info [$(PROJECT_NAME)] $1)
TEST_LOG=$(info [$(PROJECT_NAME):test] $1)

MAIN_SRC=$(foreach MAIN_SRC_DIR,$(MAIN_SRC_DIRS),$(wildcard $(MAIN_SRC_DIR)/*.c))
MAIN_OBJ=$(addprefix $(BUILD_OBJ_DIR)/,$(MAIN_SRC:.c=.o))
MAIN_EXE=$(BUILD_EXE_DIR)/$(MAIN_EXE_NAME)
MAIN_CC_FLAGS=$(CC_FLAGS) $(addprefix -I,$(MAIN_INCLUDE_DIRS))
MAIN_LD_FLAGS=$(LD_FLAGS) -L $(LIBFT_DIR) -l $(LIBFT_NAME)

TEST_SRC=$(foreach TEST_SRC_DIR,$(TEST_SRC_DIRS),$(wildcard $(TEST_SRC_DIR)/*.c))
TEST_OBJ=$(addprefix $(BUILD_OBJ_DIR)/,$(TEST_SRC:.c=.o))
TEST_EXE=$(BUILD_EXE_DIR)/$(TEST_EXE_NAME)
TEST_CC_FLAGS=$(CC_FLAGS) $(addprefix -I,$(TEST_INCLUDE_DIRS))
TEST_LD_FLAGS=$(LD_FLAGS) -L $(LIBFT_DIR) -l $(LIBFT_NAME)

DEP=$(addprefix $(BUILD_DEP_DIR)/,$(MAIN_SRC:.c=.d)) $(addprefix $(BUILD_DEP_DIR)/,$(TEST_SRC:.c=.d))

# TEMPLATES

define MAIN_OBJ_TEMPLATE
$(BUILD_OBJ_DIR)/$(1)/%.o: $(1)/%.c
	$$(call MAIN_LOG,Compiling C object $$<)
	$$(SILENT) mkdir -p $$(dir $$@)
	$$(SILENT) $$(CC) $$(MAIN_CC_FLAGS) -c $$< -o $$@ -MMD
endef

define TEST_OBJ_TEMPLATE
$(BUILD_OBJ_DIR)/$(1)/%.o: $(1)/%.c
	$$(call TEST_LOG,Compiling C object $$<)
	$$(SILENT) mkdir -p $$(dir $$@)
	$$(SILENT) $$(CC) $$(TEST_CC_FLAGS) -c $$< -o $$@ -MMD
endef

# TARGETS : COMMON

all: $(MAIN_EXE) $(TEST_EXE)

clean:
	$(call MAIN_LOG,Deleting build)
	$(SILENT) rm -r $(BUILD_DIR)

libs:
	$(call MAIN_LOG,Building libs)
	$(SILENT) make -C $(LIBFT_PROJECT)

libclean:
	$(call MAIN_LOG,Cleaning libs)
	$(SILENT) make clean -C $(LIBFT_PROJECT)

debug:
	$(call DEBUG,cc: $(CC))
	$(call DEBUG,main src: $(MAIN_SRC))
	$(call DEBUG,main obj: $(MAIN_OBJ))
	$(call DEBUG,test src: $(TEST_SRC))
	$(call DEBUG,test obj: $(TEST_OBJ))

# TARGETS : MAIN

$(MAIN_EXE): libs $(MAIN_OBJ)
	$(call MAIN_LOG,Linking executable)
	$(SILENT) $(LD) $(MAIN_LD_FLAGS) $(MAIN_OBJ) -o $@

$(foreach MAIN_SRC_DIR,$(MAIN_SRC_DIRS),$(eval $(call MAIN_OBJ_TEMPLATE,$(MAIN_SRC_DIR))))

# TARGETS : TEST

test: $(TEST_EXE)
	@$(TEST_EXE)
	@$(ADDITIONAL_TEST)

$(TEST_EXE): libs $(TEST_OBJ)
	$(call TEST_LOG,Linking executable)
	$(SILENT) $(LD) $(TEST_LD_FLAGS) $(TEST_OBJ) -o $@

$(foreach TEST_SRC_DIR,$(TEST_SRC_DIRS),$(eval $(call TEST_OBJ_TEMPLATE,$(TEST_SRC_DIR))))

# FINAL

-include $(DEP)
.PHONY: all clean test libs libclean debug
VPATH=$(MAIN_SRC_DIRS) $(TEST_SRC_DIRS)