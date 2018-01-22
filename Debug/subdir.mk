################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../copy.c \
../main.c \
../output.c \
../struct.c \
../tpool.c \
../value.c 

OBJS += \
./copy.o \
./main.o \
./output.o \
./struct.o \
./tpool.o \
./value.o 

C_DEPS += \
./copy.d \
./main.d \
./output.d \
./struct.d \
./tpool.d \
./value.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/tong60514/cuda-workspace/algorithm/datastruct/include -I/home/tong60514/cuda-workspace/algorithm/datatype/include -I/home/tong60514/cuda-workspace/algorithm/include -I/home/tong60514/cuda-workspace/algorithm/search/include -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

tpool.o: ../tpool.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/tong60514/cuda-workspace/algorithm/datastruct/include -I/home/tong60514/cuda-workspace/algorithm/datatype/include -I/home/tong60514/cuda-workspace/algorithm/include -I/home/tong60514/cuda-workspace/algorithm/search/include -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"tpool.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


