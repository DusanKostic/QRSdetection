CC=gcc
CFLAGS=-c -Wall -I$(INCLUDES)
LDFLAGS= -fopenmp -L$(QRSLIB_PATH) -l$(QRSLIB_NAME) -lm
INCLUDES=../src
SOURCES=qrstest.c
QRSLIB_PATH=../src
QRSLIB_NAME=qrs
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=qrstest

.PHONY: all
all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f ${OBJECTS} ${EXECUTABLE}

.PHONY: run
run:
	./qrstest ../testdb/aaa/100.txt ./rpeaks_1.txt ./speaks_1.txt ./qrs_log.txt 1800000 250
