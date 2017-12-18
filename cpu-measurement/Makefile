CC = gcc
CFLAGS = -Wall
RM = rm -rf

all: cost_system_call

cost_system_call: cost_system_call.c
	$(CC) $(CFLAGS) -o cost_system_call $^

clean:
	$(RM) cost_system_call
	$(RM) *.o
	$(RM) *.output
