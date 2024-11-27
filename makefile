# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Target executable name
TARGET = Lab_3

# Source files
SRCS = Lab_3.c MMS.c

# Header files (optional, for clarity)
HEADERS = MMS.h

# Build the target
$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Clean up build artifacts
clean:
	rm -f $(TARGET)

# old makefile for single-thread:

# CC = gcc
#
# # Flags
# CFLAGS = -Wall -Wextra -std=c99 -pthread
#
# TARGET = MMS
# SRCS = MMS.c
# HEADERS = MMS.h
#
# # Make all sources into objects
# OBJS = $(SRCS:.c=.o)
#
# # Build target
# $(TARGET): $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
#
# # Compile source files into object files
# %.o: %.c $(HEADERS)
# 	$(CC) $(CFLAGS) -c $< -o $@
#
# # Clean artifacts with 'make clean'
# clean:
# 	rm -f $(TARGET) $(OBJS)
