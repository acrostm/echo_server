# Makefile

# Compiler
CC = gcc

# Source file
SERVER = echo_server.c

# Executable name
EXECUTABLE = echo_server

# Default target
all: $(EXECUTABLE)

# Rule to build the executable
$(EXECUTABLE): $(SERVER)
	$(CC) $(SERVER) -o $(EXECUTABLE)

# Rule to clean up
clean:
	rm -f $(EXECUTABLE)
