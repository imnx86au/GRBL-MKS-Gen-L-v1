################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../doc/ \
../doc/Threading/ \
../grbl


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../grbl/timekeeper.c \
../grbl/main.c \
../grbl/motion_control.c \
../grbl/gcode.c \
../grbl/spindle_control.c \
../grbl/coolant_control.c \
../grbl/serial.c \
../grbl/protocol.c \
../grbl/stepper.c \
../grbl/eeprom.c \
../grbl/settings.c \
../grbl/planner.c \
../grbl/nuts_bolts.c \
../grbl/limits.c \
../grbl/print.c \
../grbl/probe.c \
../grbl/report.c \
../grbl/system.c \
../grbl/sleep.c \
../grbl/jog.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
grbl/timekeeper.o \
grbl/main.o \
grbl/motion_control.o \
grbl/gcode.o \
grbl/spindle_control.o \
grbl/coolant_control.o \
grbl/serial.o \
grbl/protocol.o \
grbl/stepper.o \
grbl/eeprom.o \
grbl/settings.o \
grbl/planner.o \
grbl/nuts_bolts.o \
grbl/limits.o \
grbl/print.o \
grbl/probe.o \
grbl/report.o \
grbl/system.o \
grbl/sleep.o \
grbl/jog.o

OBJS_AS_ARGS +=  \
grbl/timekeeper.o \
grbl/main.o \
grbl/motion_control.o \
grbl/gcode.o \
grbl/spindle_control.o \
grbl/coolant_control.o \
grbl/serial.o \
grbl/protocol.o \
grbl/stepper.o \
grbl/eeprom.o \
grbl/settings.o \
grbl/planner.o \
grbl/nuts_bolts.o \
grbl/limits.o \
grbl/print.o \
grbl/probe.o \
grbl/report.o \
grbl/system.o \
grbl/sleep.o \
grbl/jog.o

C_DEPS +=  \
grbl/timekeeper.d \
grbl/main.d \
grbl/motion_control.d \
grbl/gcode.d \
grbl/spindle_control.d \
grbl/coolant_control.d \
grbl/serial.d \
grbl/protocol.d \
grbl/stepper.d \
grbl/eeprom.d \
grbl/settings.d \
grbl/planner.d \
grbl/nuts_bolts.d \
grbl/limits.d \
grbl/print.d \
grbl/probe.d \
grbl/report.d \
grbl/system.d \
grbl/sleep.d \
grbl/jog.d

C_DEPS_AS_ARGS +=  \
grbl/timekeeper.d \
grbl/main.d \
grbl/motion_control.d \
grbl/gcode.d \
grbl/spindle_control.d \
grbl/coolant_control.d \
grbl/serial.d \
grbl/protocol.d \
grbl/stepper.d \
grbl/eeprom.d \
grbl/settings.d \
grbl/planner.d \
grbl/nuts_bolts.d \
grbl/limits.d \
grbl/print.d \
grbl/probe.d \
grbl/report.d \
grbl/system.d \
grbl/sleep.d \
grbl/jog.d

OUTPUT_FILE_PATH +=GRBL-L-Mega.elf

OUTPUT_FILE_PATH_AS_ARGS +=GRBL-L-Mega.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler









































grbl/%.o: ../grbl/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.150\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atmega2560 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.150\gcc\dev\atmega2560" -c -std=gnu99 -DF_CPU=16000000 -mmcu=atmega2560 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="GRBL-L-Mega.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -mmcu=atmega2560 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.150\gcc\dev\atmega2560"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "GRBL-L-Mega.elf" "GRBL-L-Mega.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "GRBL-L-Mega.elf" "GRBL-L-Mega.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "GRBL-L-Mega.elf" > "GRBL-L-Mega.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "GRBL-L-Mega.elf" "GRBL-L-Mega.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "GRBL-L-Mega.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "GRBL-L-Mega.elf" "GRBL-L-Mega.a" "GRBL-L-Mega.hex" "GRBL-L-Mega.lss" "GRBL-L-Mega.eep" "GRBL-L-Mega.map" "GRBL-L-Mega.srec" "GRBL-L-Mega.usersignatures"
	