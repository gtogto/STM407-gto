################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Internet/DHCP/dhcp.c 

OBJS += \
./Core/Internet/DHCP/dhcp.o 

C_DEPS += \
./Core/Internet/DHCP/dhcp.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Internet/DHCP/dhcp.o: ../Core/Internet/DHCP/dhcp.c Core/Internet/DHCP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"../Core/Ethernet" -I"../Core/Ethernet/W5100" -I"../Core/Internet/DHCP" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Internet/DHCP/dhcp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

