################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HAL_CAN_INIT.c \
../src/HAL_UART_out.c \
../src/_write.c \
../src/main.c \
../src/stm32f429i_discovery.c \
../src/stm32f4xx_hal_msp.c \
../src/stm32f4xx_it.c 

OBJS += \
./src/HAL_CAN_INIT.o \
./src/HAL_UART_out.o \
./src/_write.o \
./src/main.o \
./src/stm32f429i_discovery.o \
./src/stm32f4xx_hal_msp.o \
./src/stm32f4xx_it.o 

C_DEPS += \
./src/HAL_CAN_INIT.d \
./src/HAL_UART_out.d \
./src/_write.d \
./src/main.d \
./src/stm32f429i_discovery.d \
./src/stm32f4xx_hal_msp.d \
./src/stm32f4xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DARM_MATH_CM4 -DTRACE -DSTM32F429xx -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/STM32F4XX" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


