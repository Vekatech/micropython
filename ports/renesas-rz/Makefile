# Select the board to build for:
ifdef BOARD_DIR
# Custom board path - remove trailing slash and get the final component of
# the path as the board name.
BOARD ?= $(notdir $(BOARD_DIR:/=))
else
# If not given on the command line, then default to VK_RZG2LC.
BOARD ?= VK_RZG2LC
BOARD_DIR ?= boards/$(BOARD)
endif

ifeq ($(wildcard $(BOARD_DIR)/.),)
$(error Invalid BOARD specified: $(BOARD_DIR))
endif

# If the build directory is not given, make it reflect the board name.
BUILD ?= build-$(BOARD)

include ../../py/mkenv.mk
-include mpconfigport.mk
include $(BOARD_DIR)/mpconfigboard.mk

FSP_BOARD_NAME ?= $(shell echo $(BOARD) | tr '[:upper:]' '[:lower:]')

# Files that are generated and needed before the QSTR build.
#QSTR_GENERATED_HEADERS = $(BUILD)/pins_qstr.h

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h
#QSTR_GLOBAL_DEPENDENCIES += mpconfigboard_common.h $(BOARD_DIR)/mpconfigboard.h $(QSTR_GENERATED_HEADERS)

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 1

# File containing description of content to be frozen into firmware.
#FROZEN_MANIFEST ?= boards/manifest.py

# include py core make definitions
include $(TOP)/py/py.mk
#include $(TOP)/extmod/extmod.mk

MCU_SERIES_UPPER = $(shell echo $(MCU_SERIES) | tr '[:lower:]' '[:upper:]')
CMSIS_MCU_LOWER = $(shell echo $(CMSIS_MCU) | tr '[:upper:]' '[:lower:]')

LD_DIR=boards
CMSIS_DIR=lib/cmsis/inc
HAL_DIR=lib/rzg-fsp
STARTUP_FILE ?= $(HAL_DIR)/rzg/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.o
SYSTEM_FILE ?= $(HAL_DIR)/rzg/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.o
S_STARTUP_FILE ?= $(HAL_DIR)/rzg/fsp/src/bsp/cmsis/Device/RENESAS/Source/startups.o
S_SYSTEM_FILE ?= $(HAL_DIR)/rzg/fsp/src/bsp/cmsis/Device/RENESAS/Source/systems.o

# Select the cross compile prefix
CROSS_COMPILE ?= arm-none-eabi-

INC += -imacros boards/compiler_barrier.h
INC += -I.
INC += -Ifsp_cfg
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(TOP)/$(CMSIS_DIR)
INC += -I$(TOP)/$(HAL_DIR)
INC += -I$(TOP)/$(HAL_DIR)/rzg/fsp/inc
INC += -I$(TOP)/$(HAL_DIR)/rzg/fsp/inc/api
INC += -I$(TOP)/$(HAL_DIR)/rzg/fsp/inc/instances
INC += -I$(TOP)/$(HAL_DIR)/rzg/fsp/src/bsp/mcu/rzg2l
INC += -I$(TOP)/$(HAL_DIR)/rzg/fsp/src/bsp/cmsis/Device/RENESAS/Include
INC += -I$(BOARD_DIR)/rzg_gen
#INC += -I$(BOARD_DIR)/rzg_cfg/driver
INC += -I$(BOARD_DIR)/rzg_cfg/fsp_cfg
INC += -I$(BOARD_DIR)/rzg_cfg/fsp_cfg/bsp

CFLAGS += -D$(CMSIS_MCU)
CFLAGS += -DRZ_HAL_H='<$(CMSIS_MCU)_hal.h>'
CFLAGS += -DRZ_CFG_H='<$(FSP_BOARD_NAME)_conf.h>'

# Basic Cortex-M flags
CFLAGS_CORTEX_M = -mthumb

# Select hardware floating-point support
SUPPORTS_HARDWARE_FP_SINGLE = 0
SUPPORTS_HARDWARE_FP_DOUBLE = 0

# Options for particular MCU series
CFLAGS_MCU_RZG2LC = $(CFLAGS_CORTEX_M) -mcmse -mtune=cortex-m33 -mcpu=cortex-m33+nodsp+nofp

ASFLAGS += $(CFLAGS_CORTEX_M) -mcpu=cortex-$(MCU_SERIES)
CFLAGS += $(INC) -Wall -Wpointer-arith -Werror -Wdouble-promotion -Wfloat-conversion -std=gnu99 -nostdlib $(CFLAGS_EXTRA)
CFLAGS += $(CFLAGS_MCU_$(CMSIS_MCU))
CFLAGS += $(COPT)
CFLAGS += -I$(BOARD_DIR)

# Configure floating point support
#ifeq ($(MICROPY_FLOAT_IMPL),double)
#CFLAGS += -DMICROPY_FLOAT_IMPL=MICROPY_FLOAT_IMPL_DOUBLE
#else
#ifeq ($(MICROPY_FLOAT_IMPL),none)
#CFLAGS += -DMICROPY_FLOAT_IMPL=MICROPY_FLOAT_IMPL_NONE
#else
#CFLAGS += -DMICROPY_FLOAT_IMPL=MICROPY_FLOAT_IMPL_FLOAT
#CFLAGS += -fsingle-precision-constant
#endif
#endif

LDFLAGS += -nostartfiles -nostdlib -L $(LD_DIR) $(addprefix -T,$(LD_FILES)) -Wl,-Map=$(@:.elf=.map),--cref
LDFLAGS += -Wl,--defsym=_estack_reserve=8 $(CFLAGS_MCU_RZG2LC)
LIBS += "$(shell $(CC) $(CFLAGS) -print-libgcc-file-name)"

# Remove uncalled code from the final image.
CFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += -Wl,--gc-sections

LD = $(CC)

CSUPEROPT = -Os # save some code space

#Debugging or Optimization
CFLAGS += -g  # always include debug info in the ELF
ifeq ($(DEBUG), 1)
#CFLAGS += -DPENDSV_DEBUG
COPT = -Og
# Disable text compression in debug builds
#MICROPY_ROM_TEXT_COMPRESSION = 0
else
COPT += -Os -DNDEBUG
#CFLAGS += -fdata-sections -ffunction-sections
endif

# Flags for optional C++ source code
CXXFLAGS += $(filter-out -Wmissing-prototypes -Wold-style-definition -std=gnu99,$(CFLAGS))

# TODO make this common -- shouldn't be using these "private" vars from py.mk
ifneq ($(SRC_CXX)$(SRC_USERMOD_CXX)$(SRC_USERMOD_LIB_CXX),)
LIBSTDCPP_FILE_NAME = "$(shell $(CXX) $(CXXFLAGS) -print-file-name=libstdc++.a)"
LDFLAGS += -L"$(shell dirname $(LIBSTDCPP_FILE_NAME))"
endif

# Options for mpy-cross
MPY_CROSS_FLAGS += -march=armv7m

SHARED_SRC_C += $(addprefix shared/,\
	libc/string0.c \
	libc/printf.c \
	readline/readline.c \
	runtime/pyexec.c \
	runtime/stdout_helpers.c \
	)

#	netutils/dhcpserver.c \
#	netutils/netutils.c \
#	netutils/trace.c \
#	runtime/gchelper_native.c \
#	runtime/interrupt_char.c \
#	runtime/mpirq.c \
#	runtime/softtimer.c \
#	runtime/sys_stdio_mphal.c \
#	timeutils/timeutils.c \

SRC_C = \
	main.c \
	rz_hal.c \
	uart.c \
	$(BUILD)/_frozen_mpy.c \

SRC_C += $(addprefix $(BOARD_DIR)/rzg_gen/,\
	common_data.c \
	hal_data.c \
	vector_data.c \
	)

SRC_C += $(addprefix $(BOARD_DIR)/src/,\
	pin_data.c \
	)

SRC_O += \
	$(STARTUP_FILE) \
	$(SYSTEM_FILE) \
	$(S_STARTUP_FILE) \
	$(S_SYSTEM_FILE) \

#SRC_O += \
#	shared/runtime/gchelper_thumb2.o

HAL_SRC_C += $(addprefix $(HAL_DIR)/rzg/fsp/src/bsp/mcu/all/,\
	bsp_clocks.c \
	bsp_common.c \
	bsp_delay.c \
	bsp_group_irq.c \
	bsp_guard.c \
	bsp_io.c \
	bsp_irq.c \
	bsp_irqs.c \
	bsp_sbrk.c \
	bsp_security.c \
	)

HAL_SRC_C += $(addprefix $(HAL_DIR)/rzg/fsp/src/,\
	r_ioport/r_ioport.c \
	r_scif_uart/r_scif_uart.c \
	)

#CFLAGS_FSP = -Wno-unused-variable -Wno-unused-function
#$(BUILD)/$(HAL_DIR)/rzg/fsp/src/r_sci_uart/r_sci_uart.o: CFLAGS += $(CFLAGS_FSP)

OBJ += $(PY_CORE_O)
#OBJ += $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(LIB_SRC_C:.c=.o))
#OBJ += $(LIBM_O)
OBJ += $(addprefix $(BUILD)/, $(SHARED_SRC_C:.c=.o))
#OBJ += $(addprefix $(BUILD)/, $(DRIVERS_SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(HAL_SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_CXX:.cpp=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_O))

ifeq ($(CMSIS_MCU),RZG2LC)
$(BUILD)/shared/runtime/gchelper_thumb2.o: $(TOP)/shared/runtime/gchelper_thumb2.s
	$(ECHO) "AS $<"
	$(Q)$(AS) $(ASFLAGS) -o $@ $<
endif

# This file contains performance critical functions so turn up the optimisation
# level.  It doesn't add much to the code size and improves performance a bit.
# Don't use -O3 with this file because gcc tries to optimise memset in terms of itself.
$(BUILD)/shared/libc/string0.o: COPT += -O2

all: $(TOP)/lib/fsp/README.md $(BUILD)/firmware.hex $(BUILD)/firmware.bin

# For convenience, automatically fetch required submodules if they don't exist
$(TOP)/$(HAL_DIR)/README.md:
	$(ECHO) "fsp submodule not found, fetching it now..."
	(cd $(TOP) && git submodule update --init $(HAL_DIR))

define GENERATE_ELF
	$(ECHO) "LINK $(1)"
	$(Q)$(LD) $(LDFLAGS) -o $(1) $(2) $(LDFLAGS_MOD) $(LIBS)
	$(Q)$(SIZE) $(1)
endef

define GENERATE_BIN
	$(ECHO) "GEN $(1)"
	$(Q)$(OBJCOPY) -O binary -j .text.secure_vector $(2) $(1:.bin=_secure_vector.bin)
	$(Q)$(OBJCOPY) -O binary -j .text.non_secure_vector $(2) $(1:.bin=_non_secure_vector.bin)
	$(Q)$(OBJCOPY) -O binary -j .text.secure_code $(2) $(1:.bin=_secure_code.bin)
	$(Q)$(OBJCOPY) -O binary -j .text.non_secure_code -j .data $(2) $(1:.bin=_non_secure_code.bin)
endef

define GENERATE_HEX
	$(ECHO) "GEN $(1)"
	$(Q)$(OBJCOPY) -O ihex $(2) $(1)
endef

.PHONY:

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(call GENERATE_BIN,$@,$^)

$(BUILD)/firmware.hex: $(BUILD)/firmware.elf
	$(call GENERATE_HEX,$@,$^)

$(BUILD)/firmware.elf: $(OBJ)
	$(call GENERATE_ELF,$@,$^)

FILE2H = $(TOP)/tools/file2h.py

# List of sources for qstr extraction
#SRC_QSTR += $(SRC_C) $(SRC_CXX) $(SHARED_SRC_C)
SRC_QSTR += shared/readline/readline.c shared/runtime/pyexec.c

$(BUILD)/_frozen_mpy.c: $(TOP)/tests/frozen/frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

CMSIS_MCU_HDR = $(CMSIS_DIR)/$(CMSIS_MCU_LOWER).h

include $(TOP)/py/mkrules.mk
