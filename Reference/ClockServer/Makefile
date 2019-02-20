# Makefile: simple clock server with corresponding client
# Rainer Doemer, 2/16/15 (updated 2/20/17)

#FLAGS	= -Wall -ansi -O2
FLAGS	= -Wall -ansi -g
#FLAGS	= -Wall -ansi -g -DDEBUG

all: ClockClient ClockServer

clean:
	rm -rf ClockClient ClockServer

test:
	@echo "To run the examples, first start the server in one terminal,"
	@echo "then start one (or multiple) client(s) in another."
	@echo
	@echo "For example:"
	@echo "crystalcove% ./ClockServer 10000"
	@echo "crystalcove% ./ClockClient crystalcove 10000"

ClockClient: ClockClient.c
	gcc ClockClient.c $(FLAGS) -o ClockClient

ClockServer: ClockServer.c
	gcc ClockServer.c $(FLAGS) -o ClockServer

# EOF
