ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTLDFLAGS  := $(shell root-config --ldflags)

DEBUG=OFF

# Linux with egcs, gcc 2.9x, gcc 3.x (>= RedHat 5.2)
CXX           = g++ 
CXXFLAGS      = -O -Wall  -fPIC -DROOT_INTERFACE -I.  -g 
LD            = g++ 
LDFLAGS       = -O -Wall -fPIC $(ROOTLDFLAGS)  -g 
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

ifeq ($(DEBUG),ON)
CXXFLAGS	+= -DDEBUG
endif

SOURCES	= Frame.C Seed.C SeedList.C
OBJS    = Frame.o Seed.o SeedList.o Analisi.o  CMOSDict.o
HEADERS = Frame.h Seed.h SeedList.h Analisi.h  MyDebugOut.h

default: prova2.o ${OBJS} ${HEADERS}
		$(LD) $(LDFLAGS) $(LIBS) $(GLIBS) prova2.o ${OBJS} -o prova.x 	

prova2.o: prova2.C ${HEADERS}    
		 ${CXX} ${CXXFLAGS} -c prova2.C

%.o: %.C 
		${CXX} ${CXXFLAGS} -c  $<

%.o: %.cxx
		${CXX} $(CXXFLAGS) -c $<

%.x: %.cpp ${OBJS} ${HEADERS}
		${CXX} $(CXXFLAGS)  $(LDFLAGS)  $(LIBS) $(GLIBS) ${OBJS}  -D__OFNAME__='"$@"' $< -o $@

CMOSDict.cxx: ${HEADERS} CMOSLinkDef.h
		${ROOTSYS}/bin/rootcint -f $@ -c  $(CXXFLAGS) -p $^

libCMOS.so: ${OBJS}
#		${LD} -shared -o$@ ${LDFLAGS} $(CXXFLAGS) -I$(ROOTSYS)/include $^
#		g++ -shared -o$@ `root-config --ldflags` $(CXXFLAGS) -I$(ROOTSYS)/include $^
		${LD} ${SOFLAGS} ${LDFLAGS} $(LIBS) $(GLIBS) ${OBJS}  -o $@ 

clean:
		rm -f *.x *.a *.o *.so *.pcm *.d CMOSDict.cxx
