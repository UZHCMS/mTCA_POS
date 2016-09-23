########################################################################
TOOL_DIR:=$(BPIXELTOOLS)/tools

EXE	=	digitalfed

CC	=	g++

#COPTS	=	-fPIC -D LINUX -Wall -std=c++0x -g -O1 -w -Wall -pedantic -std=c++11 -fPIC
COPTS	=	 -g -O1 -w -Wall -pedantic -std=c++11 -fPIC
#COPTS	=	-g -fPIC -DLINUX -Wall 

#FLAGS	=	-Wall -s
FLAGS   =       -g -O1 -w -Wall -pedantic -std=c++11 -fPIC
#FLAGS	=	-Wall

#DEPLIBS	=       /opt/xdaq/lib/libCAENVME.so /usr/lib/libcurses.a
DEPLIBS	=       /opt/xdaq/lib/libCAENVME.so

LibraryPaths  = $(LibraryDirs:%=-L%)

LIBS	=	 -L/opt/xdaq/lib -lttcutils -lttcttcci -lpthread -ldl -lboost_program_options -lxcept -lxdaq -ltoolbox -lxdata -luuid -lpeer -lxgi -llog4cplus -lxerces-c -lcgicc -lxoap -lconfig -lasyncresolv -llogxmlappender -llogudpappender -lmimetic -lHALUtilities -lGenericHAL -lCAENVME -lCAENLinuxBusAdapter -lDummyBusAdapters -L$(TOOL_DIR)/lib -lSockets -L/opt/cactus/lib -lcactus_extern_pugixml -lcactus_uhal_log -lcactus_uhal_grammars -lcactus_uhal_uhal -lboost_system -lcactus_amc13_amc13 -L/home/cmspixel/FEDtool/lib/ -lPh1_Interface -lPh1_Description -lPh1_System -lPh1_Utils -lPh1_Amc13 -L/home/cmspixel/FEDtool 

INCLUDEDIR =	-I. -I/opt/xdaq/include -I/opt/xdaq/include/ttc/ttcci -I ../tools/include -I/opt/cactus/include/ -I/home/cmspixel/FEDtool -I/home/cmspixel/FEDtool/AMC13

OBJS	=	digitalfed.o

INCLUDES =	

#########################################################################

all	:	$(EXE)

clean	:
		/bin/rm -f $(OBJS) $(EXE)

$(EXE)	:	$(OBJS) 
		/bin/rm -f $(EXE)
		$(CC) -std=c++11 $(FLAGS)  -o $(EXE) $(OBJS) $(DEPLIBS) $(LIBS) -lrt 

$(OBJS)	:	digitalfed.cpp

%.o	:	%.cpp
		$(CC) $(COPTS) $(INCLUDEDIR) -D LINUX -c -o $@ $<

