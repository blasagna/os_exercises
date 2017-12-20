CC = gcc
CFLAGS = -Wall
RM = rm -rf

all: cost_system_call cost_context_switch

cost_system_call: cost_system_call.c
	$(CC) $(CFLAGS) -o cost_system_call $^

cost_context_switch: cost_context_switch.c
	$(CC) $(CFLAGS) -o cost_context_switch $^

clean:
	$(RM) cost_system_call cost_context_switch
	$(RM) *.o
	$(RM) *.output
