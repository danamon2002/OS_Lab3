CC = gcc

# Flags
CFLAGS = -Wall -Wextra -std=c99 -pthread

TARGET = SingleThreadMMS
SRCS = SingleThreadMMS.c
# HEADERS = 

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
