################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board.c \
../book.c \
../data.c \
../eval.c \
../evolution.c \
../main.c \
../search.c \
../writer.c 

OBJS += \
./board.o \
./book.o \
./data.o \
./eval.o \
./evolution.o \
./main.o \
./search.o \
./writer.o 

C_DEPS += \
./board.d \
./book.d \
./data.d \
./eval.d \
./evolution.d \
./main.d \
./search.d \
./writer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


