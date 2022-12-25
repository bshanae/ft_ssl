# PROJECT CONFIG

СС=gcc
CC_FLAGS=-g -Wall
LD=gcc
LD_FLAGS=

PROJECT_NAME=ft_ssl

MAIN_SRC_DIRS=main main/md5 main/sha256
MAIN_INCLUDE_DIRS=main
MAIN_EXE_NAME=ft_ssl

TEST_SRC_DIRS=main/md5 main/sha256 test test/tools test/md5_tests test/sha256_tests
TEST_INCLUDE_DIRS=test main
TEST_EXE_NAME=ft_ssl_tests

# DEBUG

DEBUG=$(info [DEBUG] $1)
SILENT=@

# CONSTANTS

BUILD_DIR=build
BUILD_OBJ_DIR=build/obj
BUILD_EXE_DIR=build

# DYNAMIC CONFIG

MAIN_LOG=$(info [$(PROJECT_NAME)] $1)
TEST_LOG=$(info [$(PROJECT_NAME):test] $1)

MAIN_SRC=$(foreach MAIN_SRC_DIR,$(MAIN_SRC_DIRS),$(wildcard $(MAIN_SRC_DIR)/*.c))
MAIN_OBJ=$(addprefix $(BUILD_OBJ_DIR)/,$(MAIN_SRC:.c=.o))
MAIN_EXE=$(BUILD_EXE_DIR)/$(MAIN_EXE_NAME)
MAIN_CC_FLAGS=$(CC_FLAGS) $(addprefix -I,$(MAIN_INCLUDE_DIRS))
MAIN_LD_FLAGS=$(LD_FLAGS)

TEST_SRC=$(foreach TEST_SRC_DIR,$(TEST_SRC_DIRS),$(wildcard $(TEST_SRC_DIR)/*.c))
TEST_OBJ=$(addprefix $(BUILD_OBJ_DIR)/,$(TEST_SRC:.c=.o))
TEST_EXE=$(BUILD_EXE_DIR)/$(TEST_EXE_NAME)
TEST_CC_FLAGS=$(CC_FLAGS) $(addprefix -I,$(TEST_INCLUDE_DIRS))
TEST_LD_FLAGS=$(LD_FLAGS)

DEP=$(MAIN_OBJ:.o=.d) $(TEST_OBJ:.o=.d)
VPATH=$(MAIN_SRC_DIRS) $(TEST_SRC_DIRS)

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

debug:
	$(call DEBUG,cc: $(CC))
	$(call DEBUG,main src: $(MAIN_SRC))
	$(call DEBUG,main obj: $(MAIN_OBJ))
	$(call DEBUG,test src: $(TEST_SRC))
	$(call DEBUG,test obj: $(TEST_OBJ))

# TARGETS : MAIN

$(MAIN_EXE): $(MAIN_OBJ)
	$(call MAIN_LOG,Linking executable)
	$(SILENT) $(LD) $(MAIN_LD_FLAGS) $(MAIN_OBJ) -o $@

$(foreach MAIN_SRC_DIR,$(MAIN_SRC_DIRS),$(eval $(call MAIN_OBJ_TEMPLATE,$(MAIN_SRC_DIR))))

# TARGETS : TEST

$(TEST_EXE): $(TEST_OBJ)
	$(call TEST_LOG,Linking executable)
	$(SILENT) $(LD) $(TEST_LD_FLAGS) $(TEST_OBJ) -o $@

$(foreach TEST_SRC_DIR,$(TEST_SRC_DIRS),$(eval $(call TEST_OBJ_TEMPLATE,$(TEST_SRC_DIR))))

# FINAL

-include $(DEP)
.PHONY: all clean test debug