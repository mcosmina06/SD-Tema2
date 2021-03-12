#***NUME Mandru, PRENUME Cosmina***
#****GRUPA 313CB, SERIA CB**********
CC = gcc
CFLAGS = -g -Wall 
HEADERS = 
OBJECTS = tema.o
EXEC = tema2

.PHONY = default build clean

default: build

build: tema2

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

run: build
	./$(EXEC) ${ARGS}

clean:
	-rm -f $(OBJECTS)
	-rm -f $(EXEC)
