################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
fft.obj: ../fft.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/beniamino/Documents/WorkSpace/Ccs7/EmbeddedUSB" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=CLASS_IS_TM4C123 --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="fft.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

fir.obj: ../fir.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/beniamino/Documents/WorkSpace/Ccs7/EmbeddedUSB" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=CLASS_IS_TM4C123 --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="fir.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

fix_fft.obj: ../fix_fft.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/beniamino/Documents/WorkSpace/Ccs7/EmbeddedUSB" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=CLASS_IS_TM4C123 --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="fix_fft.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/beniamino/Documents/WorkSpace/Ccs7/EmbeddedUSB" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=CLASS_IS_TM4C123 --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

tm4c123gh6pm_startup_ccs.obj: ../tm4c123gh6pm_startup_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/beniamino/Documents/WorkSpace/Ccs7/EmbeddedUSB" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=CLASS_IS_TM4C123 --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="tm4c123gh6pm_startup_ccs.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

usbDiskController.obj: ../usbDiskController.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/beniamino/Documents/WorkSpace/Ccs7/EmbeddedUSB" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.0.LTS/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=CLASS_IS_TM4C123 --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="usbDiskController.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


