################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32_AI_AudioPreprocessing_Library/Src/common_tables.c \
../STM32_AI_AudioPreprocessing_Library/Src/dct.c \
../STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.c \
../STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.c \
../STM32_AI_AudioPreprocessing_Library/Src/window.c 

OBJS += \
./STM32_AI_AudioPreprocessing_Library/Src/common_tables.o \
./STM32_AI_AudioPreprocessing_Library/Src/dct.o \
./STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.o \
./STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.o \
./STM32_AI_AudioPreprocessing_Library/Src/window.o 

C_DEPS += \
./STM32_AI_AudioPreprocessing_Library/Src/common_tables.d \
./STM32_AI_AudioPreprocessing_Library/Src/dct.d \
./STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.d \
./STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.d \
./STM32_AI_AudioPreprocessing_Library/Src/window.d 


# Each subdirectory must supply rules for building sources it contributes
STM32_AI_AudioPreprocessing_Library/Src/%.o STM32_AI_AudioPreprocessing_Library/Src/%.su STM32_AI_AudioPreprocessing_Library/Src/%.cyclo: ../STM32_AI_AudioPreprocessing_Library/Src/%.c STM32_AI_AudioPreprocessing_Library/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -D__FPU_PRESENT -DUSE_HAL_DRIVER -DARMCM7 -DARM_MATH_CM7 -DDEBUG -DSTM32F722xx -c -I../Core/Inc -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/Drivers/CMSIS/Include" -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/Drivers/CMSIS/DSP/Include" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/X-CUBE-AI/App" -I../Middlewares/ST/AI/Inc -I../X-CUBE-AI/App -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/STM32_AI_AudioPreprocessing_Library/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-STM32_AI_AudioPreprocessing_Library-2f-Src

clean-STM32_AI_AudioPreprocessing_Library-2f-Src:
	-$(RM) ./STM32_AI_AudioPreprocessing_Library/Src/common_tables.cyclo ./STM32_AI_AudioPreprocessing_Library/Src/common_tables.d ./STM32_AI_AudioPreprocessing_Library/Src/common_tables.o ./STM32_AI_AudioPreprocessing_Library/Src/common_tables.su ./STM32_AI_AudioPreprocessing_Library/Src/dct.cyclo ./STM32_AI_AudioPreprocessing_Library/Src/dct.d ./STM32_AI_AudioPreprocessing_Library/Src/dct.o ./STM32_AI_AudioPreprocessing_Library/Src/dct.su ./STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.cyclo ./STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.d ./STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.o ./STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.su ./STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.cyclo ./STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.d ./STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.o ./STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.su ./STM32_AI_AudioPreprocessing_Library/Src/window.cyclo ./STM32_AI_AudioPreprocessing_Library/Src/window.d ./STM32_AI_AudioPreprocessing_Library/Src/window.o ./STM32_AI_AudioPreprocessing_Library/Src/window.su

.PHONY: clean-STM32_AI_AudioPreprocessing_Library-2f-Src

