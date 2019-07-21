include ../config.mk

LIBNAME	= rtos.o

OBJS	 = heap_3.o
OBJS	+= port.o
OBJS	+= ./src/tasks.o
OBJS	+= ./src/list.o
OBJS	+= ./src/queue.o

all:	$(LIBNAME)

$(LIBNAME):	$(OBJS)
	@$(LD) -r -o $@ $(OBJS) 

clean:
	@rm -rf $(OBJS) $(LIBNAME) .depend	
	
#########################################################################
.depend :	$(OBJS:.o=.c)
	@$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@
#########################################################################
