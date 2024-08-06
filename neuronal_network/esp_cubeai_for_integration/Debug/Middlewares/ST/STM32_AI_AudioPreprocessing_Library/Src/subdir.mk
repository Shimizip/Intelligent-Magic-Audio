################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/common_tables.c \
../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/dct.c \
../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.c \
../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.c \
../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/window.c 

OBJS += \
./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/common_tables.o \
./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/dct.o \
./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.o \
./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.o \
./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/window.o 

C_DEPS += \
./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/common_tables.d \
./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/dct.d \
./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.d \
./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.d \
./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/window.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/%.o Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/%.su Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/%.cyclo: ../Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/%.c Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -D__FPU_PRESENT -DUSE_HAL_DRIVER -DARMCM7 -DARM_MATH_CM7 -DDEBUG -DSTM32F722xx -c -I../Core/Inc -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/Drivers/CMSIS/Include" -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/Drivers/CMSIS/DSP/Include" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/X-CUBE-AI/App" -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Inc" -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src" -I../Middlewares/ST/AI/Inc -I../X-CUBE-AI/App -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_AI_AudioPreprocessing_Library-2f-Src

clean-Middlewares-2f-ST-2f-STM32_AI_AudioPreprocessing_Library-2f-Src:
	-$(RM) ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/common_tables.cyclo ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/common_tables.d ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/common_tables.o ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/common_tables.su ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/dct.cyclo ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/dct.d ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/dct.o ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/dct.su ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.cyclo ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.d ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.o ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/feature_extraction.su ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.cyclo ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.d ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.o ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/mel_filterbank.su ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/window.cyclo ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/window.d ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/window.o ./Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src/window.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_AI_AudioPreprocessing_Library-2f-Src

