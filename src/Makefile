CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-as
LD      = arm-none-eabi-ld
AR      = arm-none-eabi-ar
RM      = rm
SIZE    = arm-none-eabi-size
STRIP   = arm-none-eabi-strip -s
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
MKDIR   = mkdir

BIN=openmv

# Directories
TOP_DIR=$(shell pwd)
BUILD=$(TOP_DIR)/build
CMSIS_DIR=cmsis
STHAL_DIR=sthal
STUSB_DIR=stusb
FATFS_DIR=fatfs
MICROPY_DIR=micropython
CC3K_DIR=cc3k
OMV_DIR=omv

# Target
TARGET ?= OPENMV1

# Debugging/Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -O0 -ggdb3
else
CFLAGS += -O2 -ggdb3 -DNDEBUG
endif

# Compiler Flags
CFLAGS += -std=gnu99 -Wall -Werror -mlittle-endian -mthumb -nostartfiles -mabi=aapcs-linux -fdata-sections -ffunction-sections
CFLAGS += -fsingle-precision-constant -Wdouble-promotion -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -DARM_MATH_CM4 -D__FPU_PRESENT=1 -D__FPU_USED=1 -DUSE_USB_FS -DUSE_DEVICE_MODE -DUSE_USB_OTG_ID=0 -DSTM32F407xx -DHSE_VALUE=12000000

CFLAGS += -I. -Iinclude
CFLAGS += -I$(TOP_DIR)/$(CMSIS_DIR)/include/
CFLAGS += -I$(TOP_DIR)/$(CMSIS_DIR)/include/st
CFLAGS += -I$(TOP_DIR)/$(STHAL_DIR)/include/
CFLAGS += -I$(TOP_DIR)/$(FATFS_DIR)/include/
CFLAGS += -I$(TOP_DIR)/$(CC3K_DIR)/include/

CFLAGS += -I$(BUILD)/$(MICROPY_DIR)/
CFLAGS += -I$(TOP_DIR)/$(MICROPY_DIR)/py/
CFLAGS += -I$(TOP_DIR)/$(MICROPY_DIR)/stmhal/
CFLAGS += -I$(TOP_DIR)/$(MICROPY_DIR)/stmhal/usbdev/
CFLAGS += -I$(TOP_DIR)/$(MICROPY_DIR)/stmhal/usbdev/core/inc/
CFLAGS += -I$(TOP_DIR)/$(MICROPY_DIR)/stmhal/usbdev/class/cdc_msc_hid/inc/
CFLAGS += -I$(TOP_DIR)/$(MICROPY_DIR)/stmhal/boards/$(TARGET)/

CFLAGS += -I$(TOP_DIR)/$(OMV_DIR)/
CFLAGS += -I$(TOP_DIR)/$(OMV_DIR)/py/
CFLAGS += -I$(TOP_DIR)/$(OMV_DIR)/img/

# Linker Flags

LDFLAGS = -mcpu=cortex-m4 -mabi=aapcs-linux -mthumb -mlittle-endian -mfloat-abi=hard \
          -mfpu=fpv4-sp-d16 -nostdlib -Wl,-T$(OMV_DIR)/stm32f407.ld -Wl,--gc-sections

# Sources
#SRC_C = $(addprefix $(CMSIS_DIR)/src/st/,\
#	system_stm32f4xx.c \
#	)


OBJ += $(wildcard $(BUILD)/$(CMSIS_DIR)/src/st/*.o)
OBJ += $(wildcard $(BUILD)/$(CMSIS_DIR)/src/dsp/CommonTables/*.o)
#OBJ += $(wildcard $(BUILD)/$(CMSIS_DIR)/src/dsp/ComplexMathFunctions/*.o)
#OBJ += $(wildcard $(BUILD)/$(CMSIS_DIR)/src/dsp/ControllerFunctions/*.o)
OBJ += $(wildcard $(BUILD)/$(CMSIS_DIR)/src/dsp/FastMathFunctions/*.o)
#OBJ += $(wildcard $(BUILD)/$(CMSIS_DIR)/src/dsp/FilteringFunctions/*.o)
OBJ += $(wildcard $(BUILD)/$(CMSIS_DIR)/src/dsp/MatrixFunctions/*.o)
#OBJ += $(wildcard $(BUILD)/$(CMSIS_DIR)/src/dsp/StatisticsFunctions/*.o)
#OBJ += $(wildcard $(BUILD)/$(CMSIS_DIR)/src/dsp/SupportFunctions/*.o)
#OBJ += $(wildcard $(BUILD)/$(CMSIS_DIR)/src/dsp/TransformFunctions/*.o)

OBJ += $(wildcard $(BUILD)/$(STHAL_DIR)/src/*.o)
OBJ += $(wildcard $(BUILD)/$(CC3K_DIR)/src/*.o)
OBJ += $(wildcard $(BUILD)/$(FATFS_DIR)/src/*.o)
OBJ += $(wildcard $(BUILD)/$(FATFS_DIR)/src/option/*.o)

OBJ += $(wildcard $(BUILD)/$(MICROPY_DIR)/py/*.o)
OBJ += $(addprefix $(BUILD)/$(MICROPY_DIR)/,\
	stm32f4xx_it.o          \
	system_stm32f4xx.o      \
	string0.o               \
	usbd_conf.o             \
	usbd_desc_cdc_msc.o     \
	usbd_cdc_interface.o    \
	usbd_msc_storage.o      \
	pendsv.o                \
	bufhelper.o             \
	usb.o                   \
	printf.o                \
	gchelper.o              \
	gccollect.o             \
	pybstdio.o              \
	readline.o              \
	pyexec.o                \
	help.o                  \
	input.o                 \
	uart.o                  \
	import.o                \
	lexerfatfs.o            \
	flash.o                 \
	diskio.o                \
	storage.o               \
	file.o                  \
	math.o                  \
	mathsincos.o            \
	rtc.o                   \
	pin.o                   \
	pin_named_pins.o        \
	timer.o                 \
	servo.o                 \
	extint.o                \
	adc.o                   \
	modstm.o                \
	modtime.o               \
	pins_$(TARGET).o        \
    )

OBJ += $(addprefix $(BUILD)/$(MICROPY_DIR)/usbdev/, \
	core/src/usbd_core.o                            \
	core/src/usbd_ctlreq.o                          \
	core/src/usbd_ioreq.o                           \
	class/cdc_msc_hid/src/usbd_cdc_msc_hid.o        \
	class/cdc_msc_hid/src/usbd_msc_bot.o            \
	class/cdc_msc_hid/src/usbd_msc_scsi.o           \
	class/cdc_msc_hid/src/usbd_msc_data.o           \
	)

OBJ += $(addprefix $(BUILD)/$(OMV_DIR)/,    \
	main.o                                  \
	xalloc.o                                \
	array.o                                 \
	usbdbg.o                                \
	systick.o                               \
	sccb.o                                  \
	ov9650.o                                \
	ov2640.o                                \
	sensor.o                                \
	led.o                                   \
	rng.o                                   \
	sdcard.o                                \
	stm32f407_hal_msp.o                     \
	)

OBJ += $(addprefix $(BUILD)/$(OMV_DIR)/img/,\
	blob.o                                  \
	fmath.o                                 \
	haar.o                                  \
	imlib.o                                 \
	integral.o                              \
	kmeans.o                                \
	lab.o                                   \
	median.o                                \
	point.o                                 \
	ppm.o                                   \
	rectangle.o                             \
	surf.o                                  \
	template.o                              \
   )

OBJ += $(addprefix $(BUILD)/$(OMV_DIR)/py/, \
	py_led.o                                \
	py_sensor.o                             \
	py_image.o                              \
	py_time.o                               \
	py_clock.o                              \
	py_wlan.o                               \
	py_socket.o                             \
	)

###################################################
#Export Variables
export CC
export AS
export LD
export AR
export SIZE
export OBJCOPY
export OBJDUMP
export MKDIR
export CFLAGS
export LDFLAGS
export TOP_DIR
export BUILD
###################################################

all: | $(BUILD)
	$(MAKE) -C $(CMSIS_DIR)          BUILD=$(BUILD)/$(CMSIS_DIR)
	$(MAKE) -C $(STHAL_DIR)          BUILD=$(BUILD)/$(STHAL_DIR)
	$(MAKE) -C $(FATFS_DIR)          BUILD=$(BUILD)/$(FATFS_DIR)
	$(MAKE) -C $(CC3K_DIR)           BUILD=$(BUILD)/$(CC3K_DIR)
	$(MAKE) -C $(MICROPY_DIR)/stmhal BUILD=$(BUILD)/$(MICROPY_DIR) BOARD=$(TARGET) V=1
	$(MAKE) -C $(OMV_DIR)            BUILD=$(BUILD)/$(OMV_DIR)
	$(CC) $(LDFLAGS) $(OBJ) $(LIB) -o $(BUILD)/$(BIN).elf
	$(OBJCOPY) -Obinary $(BUILD)/$(BIN).elf $(BUILD)/$(BIN).bin
	$(SIZE) $(BUILD)/$(BIN).elf

size:
	$(SIZE) $(BUILD)/$(BIN).elf
$(BUILD):
	$(MKDIR) -p $@

clean:
	$(RM) -fr $(BUILD)

flash::
	dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -s 0x08000000 -D $(BUILD)/$(BIN).bin
