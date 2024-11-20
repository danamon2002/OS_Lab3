CC = gcc

# Flags
CFLAGS = -Wall -Wextra -std=c99 -pthread

TARGET = Lab_3
SRCS = Lab_3.c MMS.c User.c
HEADERS = MMS.h User.h

# Make all sources into objects
OBJS = $(SRCS:.c=.o)

# Build target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compile source files into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean artifacts with 'make clean'
clean:
	rm -f $(TARGET) $(OBJS)
