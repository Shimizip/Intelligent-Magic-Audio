################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../X-CUBE-AI/App/network_1.c \
../X-CUBE-AI/App/network_1_data.c \
../X-CUBE-AI/App/network_1_data_params.c 

OBJS += \
./X-CUBE-AI/App/network_1.o \
./X-CUBE-AI/App/network_1_data.o \
./X-CUBE-AI/App/network_1_data_params.o 

C_DEPS += \
./X-CUBE-AI/App/network_1.d \
./X-CUBE-AI/App/network_1_data.d \
./X-CUBE-AI/App/network_1_data_params.d 


# Each subdirectory must supply rules for building sources it contributes
X-CUBE-AI/App/%.o X-CUBE-AI/App/%.su X-CUBE-AI/App/%.cyclo: ../X-CUBE-AI/App/%.c X-CUBE-AI/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DARMCM7 -DARM_MATH_CM7 -D__FPU_PRESENT -DDEBUG -DSTM32F722xx -c -I../Core/Inc -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/Drivers/CMSIS/Include" -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/Drivers/CMSIS/DSP/Include" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/X-CUBE-AI/App" -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Inc" -I"/Users/leon/STM32CubeIDE/workspace_1.14.0/nn-test/Middlewares/ST/STM32_AI_AudioPreprocessing_Library/Src" -I../Middlewares/ST/AI/Inc -I../X-CUBE-AI/App -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-X-2d-CUBE-2d-AI-2f-App

clean-X-2d-CUBE-2d-AI-2f-App:
	-$(RM) ./X-CUBE-AI/App/network_1.cyclo ./X-CUBE-AI/App/network_1.d ./X-CUBE-AI/App/network_1.o ./X-CUBE-AI/App/network_1.su ./X-CUBE-AI/App/network_1_data.cyclo ./X-CUBE-AI/App/network_1_data.d ./X-CUBE-AI/App/network_1_data.o ./X-CUBE-AI/App/network_1_data.su ./X-CUBE-AI/App/network_1_data_params.cyclo ./X-CUBE-AI/App/network_1_data_params.d ./X-CUBE-AI/App/network_1_data_params.o ./X-CUBE-AI/App/network_1_data_params.su

.PHONY: clean-X-2d-CUBE-2d-AI-2f-App

