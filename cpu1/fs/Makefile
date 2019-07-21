include ../config.mk

LIBNAME	 = fs.o

OBJS	 = ./fat/src/ff.o
OBJS	+= ./fat/src/diskio.o
OBJS	+= ./fat/src/option/ccsbcs.o
OBJS	+= ./fat/src/option/syscall.o

all:	$(LIBNAME)

$(LIBNAME): $(OBJS)
	@$(LD) -r -o $@ $(OBJS)
	
clean:
	@rm -rf $(OBJS) $(LIBNAME) .depend	

#########################################################################
.depend :	$(OBJS:.o=.c)
	@$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@
#########################################################################
