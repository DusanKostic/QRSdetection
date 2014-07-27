CC = gcc # C compiler
CFLAGS = -fPIC -Wextra -g # C flags
LDFLAGS = -shared -fopenmp # linking flags
RM = rm -f # rm command
TARGET_LIB = libqrs.so # target lib
 
SRCS =  $(shell echo ./*.c) # source files
OBJS = $(SRCS:.c=.o)
 
.PHONY: all
all: ${TARGET_LIB}
 
$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^
 
$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@
 
include $(SRCS:.c=.d)
 
.PHONY: clean
clean:
	${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)
