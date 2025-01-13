################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"/home/carlos/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="/root/workspace_v12/lab3/DSP_EffectSuite" --include_path="/root/workspace_v12/lab3/DSP_EffectSuite/include" --include_path="/root/workspace_v12/include" --include_path="/root/workspace_v12/C55xxCSL/include" --include_path="/home/carlos/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="src/$(basename $(<F)).d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

src/%.obj: ../src/%.s55 $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"/home/carlos/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -v5502 --memory_model=large -g --include_path="/root/workspace_v12/lab3/DSP_EffectSuite" --include_path="/root/workspace_v12/lab3/DSP_EffectSuite/include" --include_path="/root/workspace_v12/include" --include_path="/root/workspace_v12/C55xxCSL/include" --include_path="/home/carlos/ti/ccs1281/ccs/tools/compiler/c5500_4.4.1/include" --define=c5502 --display_error_number --diag_warning=225 --ptrdiff_size=32 --preproc_with_compile --preproc_dependency="src/$(basename $(<F)).d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


