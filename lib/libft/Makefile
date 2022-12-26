# CONFIG

LIB_NAME=ft
CC=gcc
CC_FLAGS=
LD=gcc
LD_FLAGS=
INCLUDE_DIRS=include
SRC_DIRS= src/std_char \
		  src/std_mem \
		  src/std_num \
		  src/std_string \
		  src/get_next_line \
		  src/vector \
		  src/ft_printf \
		  src/ft_printf/ftp_buffer \
		  src/ft_printf/ftp_control \
		  src/ft_printf/ftp_format_in \
		  src/ft_printf/ftp_format_data \
		  src/ft_printf/ftp_format_out \
		  src/ft_printf/ftp_format_out_whole \
		  src/ft_printf/ftp_format_out_real \
		  src/ft_printf/utils \
		  src/ft_printf/long

# TOOLS

LOG=$(info [libft] $1)
SILENT=@

# CONSTANTS

BUILD_DIR=build
BUILD_OBJ_DIR=build/obj
BUILD_DEP_DIR=build/dep
BUILD_LIB_DIR=build

# SETUP

CC_FLAGS+=$(addprefix -I,$(INCLUDE_DIRS))

LIB=$(BUILD_LIB_DIR)/lib$(LIB_NAME).a
SRC=$(foreach SRC_DIR,$(SRC_DIRS),$(wildcard $(SRC_DIR)/*.c))
OBJ=$(addprefix $(BUILD_OBJ_DIR)/,$(SRC:.c=.o))
DEP=$(addprefix $(BUILD_DEP_DIR)/,$(MAIN_SRC:.c=.d))

# TEMPLATES

define OBJ_TEMPLATE
$(BUILD_OBJ_DIR)/$(1)/%.o: $(1)/%.c
	$$(call LOG,Compiling C object $$<)
	$$(SILENT) mkdir -p $$(dir $$@)
	$$(SILENT) $$(CC) $$(CC_FLAGS) -c $$< -o $$@ -MMD
endef

# TARGETS

all: $(LIB)

clean:
	$(call LOG,Deleting build folder)
	$(SILENT) rm -rf $(BUILD_DIR)

$(LIB): $(OBJ)
	$(call LOG,Building library)
	$(SILENT) ar rc $(LIB) $(OBJ)
	$(SILENT) ranlib $(LIB)

$(OBJ_DIR): $(OBJ_DIR)
	$(call LOG,Creating build directory)
	$(SILENT) mkdir $(OBJ_DIR)

$(foreach SRC_DIR,$(SRC_DIRS),$(eval $(call OBJ_TEMPLATE,$(SRC_DIR))))

debug:
	$(call LOG,$(OBJ))

# FINAL

.PHONY: all clean debug
-include $(DEP)
VPATH=$(SRC_DIRS)
