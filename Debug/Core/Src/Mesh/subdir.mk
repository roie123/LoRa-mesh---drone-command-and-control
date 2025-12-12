################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: mine
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Mesh/packet.c \
../Core/Src/Mesh/xPacket_recovery_task.c 

OBJS += \
./Core/Src/Mesh/packet.o \
./Core/Src/Mesh/xPacket_recovery_task.o 

C_DEPS += \
./Core/Src/Mesh/packet.d \
./Core/Src/Mesh/xPacket_recovery_task.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Mesh/%.o Core/Src/Mesh/%.su Core/Src/Mesh/%.cyclo: ../Core/Src/Mesh/%.c Core/Src/Mesh/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Mesh

clean-Core-2f-Src-2f-Mesh:
	-$(RM) ./Core/Src/Mesh/packet.cyclo ./Core/Src/Mesh/packet.d ./Core/Src/Mesh/packet.o ./Core/Src/Mesh/packet.su ./Core/Src/Mesh/xPacket_recovery_task.cyclo ./Core/Src/Mesh/xPacket_recovery_task.d ./Core/Src/Mesh/xPacket_recovery_task.o ./Core/Src/Mesh/xPacket_recovery_task.su

.PHONY: clean-Core-2f-Src-2f-Mesh

