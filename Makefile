# PROJECT CONFIG

СС=gcc
CC_FLAGS=-c -g
LD=gcc
LD_FLAGS=

PROJECT_NAME=ft_ssl
SRC_DIRS=main main/tools main/md5 main/sha256
INCLUDE_DIRS=main
EXE_NAME=ft_ssl

# SCRIPTED CONFIG

LOG=$(info [$(PROJECT_NAME)] $1)
DEBUG=$(info [DEBUG] $1)
SILENT=@

SRC=$(foreach SRC_DIR,$(SRC_DIRS),$(wildcard $(SRC_DIR)/*.c))

BUILD_DIR=build
OBJ_DIRS=$(addprefix $(BUILD_DIR)/,$(SRC_DIRS))
OBJ=$(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))

DEP=$(OBJ:.o=.d)

CC_FLAGS+=-c $(addprefix -I,$(INCLUDE_DIRS))
LD_FLAGS+=

EXE=$(BUILD_DIR)/$(EXE_NAME)

VPATH=$(SRC_DIRS)

# TEMPLATES

define OBJ_DIR_TEMPLATE
$(1):
	$$(call LOG,Creating object directory $$@)
	$$(SILENT) mkdir -p $$@
endef

define OBJ_TEMPLATE
$(BUILD_DIR)/$(1)/%.o: $(1)/%.c
	$$(call LOG,Compiling C object $$<)
	$$(SILENT) $$(CC) $$(CC_FLAGS) $$< -o $$@ -MMD
endef

# TARGETS

all: $(EXE)

$(EXE): $(BUILD_DIR) $(OBJ_DIRS) $(OBJ)
	$(call LOG,Linking executable)
	$(SILENT) $(LD) $(LD_FLAGS) $(OBJ) -o $@

$(BUILD_DIR):
	$(call LOG,Creating build directory)
	$(SILENT) mkdir $@

$(foreach OBJ_DIR,$(OBJ_DIRS),$(eval $(call OBJ_DIR_TEMPLATE,$(OBJ_DIR))))

$(foreach SRC_DIR,$(SRC_DIRS),$(eval $(call OBJ_TEMPLATE,$(SRC_DIR))))

clean:
	$(call LOG,Deleting object)
	$(SILENT) rm -r $(BUILD_DIR)/**/*.o

fclean:
	$(call LOG,Deleting build directory)
	$(SILENT) rm -rf $(BUILD_DIR)

re: fclean all

# FINAL

-include $(DEP)

.PHONY: all clean fclean re debug