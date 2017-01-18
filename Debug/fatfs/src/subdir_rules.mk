################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
fatfs/src/diskio.obj: ../fatfs/src/diskio.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/beniamino/Documents/WorkSpace/Ccs7/EmbeddedUSB" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=CLASS_IS_TM4C123 --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="fatfs/src/diskio.d" --obj_directory="fatfs/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

fatfs/src/ff.obj: ../fatfs/src/ff.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/beniamino/Documents/WorkSpace/Ccs7/EmbeddedUSB" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=CLASS_IS_TM4C123 --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="fatfs/src/ff.d" --obj_directory="fatfs/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

fatfs/src/usbdisk.obj: ../fatfs/src/usbdisk.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/beniamino/Documents/WorkSpace/Ccs7/EmbeddedUSB" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=CLASS_IS_TM4C123 --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="fatfs/src/usbdisk.d" --obj_directory="fatfs/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


