################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../matrix.c \
../output.c \
../polynomial.c \
../strsearch.c \
../zvalue.c 

OBJS += \
./matrix.o \
./output.o \
./polynomial.o \
./strsearch.o \
./zvalue.o 

C_DEPS += \
./matrix.d \
./output.d \
./polynomial.d \
./strsearch.d \
./zvalue.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


