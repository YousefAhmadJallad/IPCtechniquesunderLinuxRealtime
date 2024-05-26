CC=gcc
CFLAGS=-g
LIBS=-lglut -lGL -lGLU -fno-stack-protector -lpthread -lm 
# x="this is paragraph one : ,1000! ; 55 % 33 , 18"

all: main opengl encoder sender masterSpy helpers spies receiver

main: main.c
	${CC} ${CFLAGS} main.c -o main $(LIBS)

opengl : opengl.c
	${CC} ${CFLAGS} opengl.c -o opengl $(LIBS)

encoder: encoder.c
	${CC} ${CFLAGS} encoder.c -o encoder $(LIBS)

sender: sender.c
	${CC} ${CFLAGS} sender.c -o sender $(LIBS)
# ./main 1 $(x)

masterSpy: masterSpy.c
	${CC} ${CFLAGS} masterSpy.c -o masterSpy $(LIBS)

helpers: helpers.c
	${CC} ${CFLAGS} helpers.c -o helpers $(LIBS)

spies: spies.c
	${CC} ${CFLAGS} spies.c -o spies $(LIBS)

receiver: receiver.c
	${CC} ${CFLAGS} receiver.c -o receiver $(LIBS)
run : main
	./main 5 5 2
clean:
	rm -f main
	rm -f encoder
	rm -f sender
	rm -f masterSpy
	rm -f helpers
	rm -f spies
	rm -f receiver
	rm -f spy.txt
	rm -f receiver.txt
	rm -f numOfColumns.txt
	rm -f open
	rm -f max_columns.txt
	rm -f collected.txt
	rm -f Mastercollected.txt
	rm -f opengl
# Here's how you can use this Makefile:

# Save the Makefile to a file named Makefile.
# Open a terminal and navigate to the directory containing the Makefile.
# Type make to build the executables parent, child, and game.
# Type make clean to remove the executables.
# Note that the clean target is provided to remove the executables after they have been built. This is useful to avoid cluttering the directory with unnecessary files.
