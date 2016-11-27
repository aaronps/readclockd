EXE= readclockd
OBJS= readclock.o clocklog.o cfgread.o checkvalues.o log.o

CC= cc

all: install/$(EXE)


install/$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS)

.SUFFIXES: .c .o .h

.c.o: 
	$(CC) -c $*.c

clean:
	rm $(OBJS)

