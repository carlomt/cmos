ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTLDFLAGS  := $(shell root-config --ldflags)

# Linux with egcs, gcc 2.9x, gcc 3.x (>= RedHat 5.2)
CXX           = g++ 
CXXFLAGS      = -O -Wall  -fPIC -DROOT_INTERFACE -I.
LD            = g++ 
LDFLAGS       = -O -Wall -fPIC $(ROOTLDFLAGS)
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

OBJS    = Frame.o CMOSDict.o
HEADERS = Frame.h

default: prova.o ${OBJS} ${HEADERS}
		$(LD) $(LDFLAGS) $(LIBS) $(GLIBS) prova.o ${OBJS} -o prova.x 	

prova.o: prova.C ${HEADERS}    
		 ${CXX} ${CXXFLAGS} -c prova.C

Frame.o: Frame.C
		${CXX} $(CXXFLAGS) -c Frame.C



CMOSDict.cpp: ${HEADERS} CMOSLinkDef.h
		${ROOTSYS}/bin/rootcint -f CMOSDict.cpp -c -p ${HEADERS}

clean:
		rm -f *.x *.a *.o *.so *.pcm *.d CMOSDict.cpp
