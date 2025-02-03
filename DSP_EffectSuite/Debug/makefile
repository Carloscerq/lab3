################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/carlos/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./src/aic3204.obj" \
"./src/buttons.obj" \
"./src/dma.obj" \
"./src/dsp_effects.obj" \
"./src/lcd.obj" \
"./src/main.obj" \
"./src/oled.obj" \
"./src/state_machine.obj" \
"./src/vectors_dma.obj" \
"./src/effects/autowah.obj" \
"./src/effects/phaser.obj" \
"./src/effects/reverbs.obj" \
"../VC5502.cmd" \
$(GEN_CMDS__FLAG) \
-lrts55x.lib \
-lezdsp5502bsl.lib \
-lcsl5502x.lib \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/subdir_vars.mk
-include src/effects/subdir_vars.mk
-include subdir_rules.mk
-include src/subdir_rules.mk
-include src/effects/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
DSP_EffectSuite.out 

EXE_OUTPUTS__QUOTED += \
"DSP_EffectSuite.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "DSP_EffectSuite.out"

# Tool invocations
DSP_EffectSuite.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C5500 Linker'
	"/home/carlos/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 -z -m"DSP_EffectSuite.map" --stack_size=0x200 --heap_size=0x400 -i"/home/carlos/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/lib" -i"/home/carlos/workspace_v12/lib" -i"/home/carlos/workspace_v12/C55xxCSL/lib" -i"/home/carlos/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/include" --reread_libs --display_error_number --warn_sections --xml_link_info="DSP_EffectSuite_linkInfo.xml" --rom_model --sys_stacksize=0x200 -o "DSP_EffectSuite.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "src/aic3204.obj" "src/buttons.obj" "src/dma.obj" "src/dsp_effects.obj" "src/lcd.obj" "src/main.obj" "src/oled.obj" "src/state_machine.obj" "src/vectors_dma.obj" "src/effects/autowah.obj" "src/effects/phaser.obj" "src/effects/reverbs.obj" 
	-$(RM) "src/vectors_dma.d" 
	-$(RM) "src/aic3204.d" "src/buttons.d" "src/dma.d" "src/dsp_effects.d" "src/lcd.d" "src/main.d" "src/oled.d" "src/state_machine.d" "src/effects/autowah.d" "src/effects/phaser.d" "src/effects/reverbs.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

