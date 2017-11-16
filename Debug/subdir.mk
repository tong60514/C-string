################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c \
../matrix.c \
../output.c \
../polynomial.c \
../strsearch.c \
../struct.c \
../tpool.c \
../value.c \
../vector.c 

OBJS += \
./main.o \
./matrix.o \
./output.o \
./polynomial.o \
./strsearch.o \
./struct.o \
./tpool.o \
./value.o \
./vector.o 

C_DEPS += \
./main.d \
./matrix.d \
./output.d \
./polynomial.d \
./strsearch.d \
./struct.d \
./tpool.d \
./value.d \
./vector.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/tong60514/cuda-workspace/algorithm/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


