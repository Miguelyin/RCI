# Compiler
CC=gcc
# Compiler flags
# CFLAGS=-Wall -Wextra -std=c11 
CFLAGS= -g -Wall -Wextra 
# Source files
SRCS=src/main.c src/protocolo_top.c src/protocolo_enc.c src/UDP_Client.c src/response_handler.c src/select.c src/TCP_Server.c src/TCP_Client.c src/interface.c src/protocolo_chat.c
# Object files
OBJS=$(patsubst src/%.c,obj/%.o,$(SRCS))
# Output executable
TARGET=COR

# Default target
all: $(TARGET)



# Compile each source file into an object file
obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $^ -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)
