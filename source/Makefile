CXX = gcc
CFLAGS = 
CXXFLAGS = $(CFLAGS)
LFLAGS = 
LIBS =
EXECUTABLES = PhantomLimb.$(BE_HOST_CPU)

ifeq ($(OSTYPE),beos)
  ifeq ($(BE_HOST_CPU),ppc)
    CXX = mwcc
  else
    CFLAGS += -Wall -W -Wno-multichar -fno-exceptions -O3
    LIBS = -lbe -lnet -lroot -lmedia -lgame 
    ifeq ($(shell ls 2>/dev/null -1 /boot/develop/headers/be/bone/bone_api.h), /boot/develop/headers/be/bone/bone_api.h)
      CFLAGS += -I/boot/develop/headers/be/bone -DBONE
      LIBS = -nodefaultlibs -lbind -lsocket -lbe -lroot -L/boot/beos/system/lib -lmedia -lgame
    endif   
  endif
endif

VPATH = .

# Makes all the programs that can be made using just cross-platform code
all : $(EXECUTABLES)

PhantomLimb.$(BE_HOST_CPU) : PhantomApp.o PhantomWindow.o PhantomPlayer.o PhantomView.o LRUEntry.o
	$(CXX) $(LFLAGS) -o $@ $^ $(LIBS)
	xres -o $@ Phantom.rsrc
	mimeset -F $@	

clean :
	rm -f *.o *.xSYM $(EXECUTABLES)
