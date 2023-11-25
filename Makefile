# Makefile
# Compiler
CC = gcc

# Source files
SERVER_SOURCE = echo_server.c
CLIENT_SOURCE = echo_client.c

# Executables
SERVER_EXECUTABLE = echo_server
CLIENT_EXECUTABLE = echo_client

# Default target
all: $(SERVER_EXECUTABLE) $(CLIENT_EXECUTABLE)

# Rule to build the executables
$(SERVER_EXECUTABLE): $(SERVER_SOURCE)
	$(CC) $(SERVER_SOURCE) -o $(SERVER_EXECUTABLE)

$(CLIENT_EXECUTABLE): $(CLIENT_SOURCE)
	$(CC) $(CLIENT_SOURCE) -o $(CLIENT_EXECUTABLE)

# Rule to clean up
clean:
	rm -f $(SERVER_EXECUTABLE) $(CLIENT_EXECUTABLE)
