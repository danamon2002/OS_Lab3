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
