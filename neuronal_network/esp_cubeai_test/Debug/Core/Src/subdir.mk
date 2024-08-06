################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/audio_classification.c \
../Core/Src/featurescaler.c \
../Core/Src/main.c \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f7xx.c \
../Core/Src/test_audio_data.c \
../Core/Src/utils.c 

OBJS += \
./Core/Src/audio_classification.o \
./Core/Src/featurescaler.o \
./Core/Src/main.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f7xx.o \
./Core/Src/test_audio_data.o \
./Core/Src/utils.o 

C_DEPS += \
./Core/Src/audio_classification.d \
./Core/Src/featurescaler.d \
./Core/Src/main.d \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f7xx.d \
./Core/Src/test_audio_data.d \
./Core/Src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -D__FPU_PRESENT -DUSE_HAL_DRIVER -DARMCM7 -DARM_MATH_CM7 -DDEBUG -DSTM32F722xx -c -I../Core/Inc -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/esp_cubeai_test/Drivers/CMSIS/Include" -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/esp_cubeai_test/Drivers/CMSIS/DSP/Include" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/esp_cubeai_test/X-CUBE-AI/App" -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/esp_cubeai_test/STM32_AI_AudioPreprocessing_Library/Inc" -I../Middlewares/ST/AI/Inc -I../X-CUBE-AI/App -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/audio_classification.cyclo ./Core/Src/audio_classification.d ./Core/Src/audio_classification.o ./Core/Src/audio_classification.su ./Core/Src/featurescaler.cyclo ./Core/Src/featurescaler.d ./Core/Src/featurescaler.o ./Core/Src/featurescaler.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f7xx_hal_msp.cyclo ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_msp.su ./Core/Src/stm32f7xx_it.cyclo ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/stm32f7xx_it.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f7xx.cyclo ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o ./Core/Src/system_stm32f7xx.su ./Core/Src/test_audio_data.cyclo ./Core/Src/test_audio_data.d ./Core/Src/test_audio_data.o ./Core/Src/test_audio_data.su ./Core/Src/utils.cyclo ./Core/Src/utils.d ./Core/Src/utils.o ./Core/Src/utils.su

.PHONY: clean-Core-2f-Src

