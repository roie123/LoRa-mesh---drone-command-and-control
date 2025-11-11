################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: mine
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LoRa/LoRa.c 

OBJS += \
./Core/Src/LoRa/LoRa.o 

C_DEPS += \
./Core/Src/LoRa/LoRa.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/LoRa/%.o Core/Src/LoRa/%.su Core/Src/LoRa/%.cyclo: ../Core/Src/LoRa/%.c Core/Src/LoRa/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-LoRa

clean-Core-2f-Src-2f-LoRa:
	-$(RM) ./Core/Src/LoRa/LoRa.cyclo ./Core/Src/LoRa/LoRa.d ./Core/Src/LoRa/LoRa.o ./Core/Src/LoRa/LoRa.su

.PHONY: clean-Core-2f-Src-2f-LoRa

