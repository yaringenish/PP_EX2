# Target executable name
TARGET = part_III.out

# Source files
SRCS = main.c SyncGraph/graph.c BFS/parallelBFS.c ThreadPool/ThreadPool.c ThreadPool/TaskQueue.c

# Compiler and flags
CC = gcc
# CFLAGS = place flags if needed
CFLAGS=-pthread
# Default target builds the executable
$(TARGET): $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) -o $@

# Clean target removes the executable
clean:
	rm -f $(TARGET)

