################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="C:/Users/felip/Documents/Projects/code_composer/DSP_EffectSuite" --include_path="C:/Users/felip/Documents/Projects/code_composer/DSP_EffectSuite/include" --include_path="C:/Users/felip/Documents/Projects/code_composer/include" --include_path="C:/Users/felip/Documents/Projects/code_composer/C55xxCSL/include" --include_path="C:/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="src/$(basename $(<F)).d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

src/%.obj: ../src/%.s55 $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="C:/Users/felip/Documents/Projects/code_composer/DSP_EffectSuite" --include_path="C:/Users/felip/Documents/Projects/code_composer/DSP_EffectSuite/include" --include_path="C:/Users/felip/Documents/Projects/code_composer/include" --include_path="C:/Users/felip/Documents/Projects/code_composer/C55xxCSL/include" --include_path="C:/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="src/$(basename $(<F)).d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


