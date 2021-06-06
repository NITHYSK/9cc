OBJDIR	= obj
SRCDIR	= src
CC	= gcc
TARGET	= 9cc

CFLAGS=-std=c11 -g -static
SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJS=$(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(subst $(SRCDIR)/,,$(SOURCES))))

INCLUDE	= include
IDFLAG	= -I./$(INCLUDE)
LIBDIR	= lib
LIB	= $(LIBDIR)/lib9cc.a
#LDFLAGS	= -L./$(LIBDIR) -l9cc
LDFLAGS	=

$(TARGET):	$(OBJS)	#$(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(CC) -o $@ -c $< $(IDFLAG)

test:		$(TARGET)
	./test.sh

test1:		$(TARGET)
	./test1.sh

$(LIB):		$(OBJS)
	ar rcs $(LIB) $(patsubst $(OBJDIR)/main.o,,$(OBJS))

libinstall:	$(LIB)
	cp $< /usr/local/lib/mylib/$(subst $(LIBDIR)/,,$<)

libupdate:	$(OBJS)	$(LIB)
	make libinstall

clean:
	rm -f 9cc tmp* lib/*.a obj/*.o

.PHONY: test clean

