TSTR:=  $(shell date --utc +"%Y%m%d%H%M")

BIN:=	cosmos.$(shell uname)-$(shell arch)
BTAR:=	cosmos_binary.tar.gz
BTARD:=	cosmos_binary-$(TSTR).tar.gz
DTAR:=	cosmos_data.tar.gz
DTARD:=	cosmos_data-$(TSTR).tar.gz

# Debugging settings
#CC:=	gcc -pipe -Wall `sdl-config --cflags` -g
#LIBS:=	`sdl-config --libs` -lefence

# Production settings
CC:=	gcc -O2 -pipe -Wall `sdl-config --cflags` -s
LIBS:=	`sdl-config --libs`

OBJS:=	main.o gui.o gui_galaxy.o gui_system.o gui_planet.o \
	game.o galaxy.o system.o planet.o satellite.o techtree.o \
	audio.o fonts.o graphics.o
# graphics_intro.o

WCC:=   win32-gcc -O2 -pipe -Wall `win32-exec sdl-config --cflags` -s
WLIBS:= `win32-exec sdl-config --libs` -lstdc++
WOBJS:= $(shell echo $(OBJS) | sed 's-\.o-.win32_o-g')

basic:	unix

all:	unix win

unix:	$(BIN)

win:	cosmos.exe

clean:	.
	rm -f *.o *.win32_o

distrib:	$(BTAR) $(DTAR)

$(BTAR):	cosmos $(BIN) cosmos.exe
	cd .. ; tar czhvf cosmos/$(BTAR) cosmos/cosmos cosmos/cosmos.* \
		cosmos/README-SDL.txt cosmos/sdl.dll
	rm -f cosmos_binary-*
	ln $(BTAR) $(BTARD)

$(DTAR):	sounds/* graphics/*
	cd .. ; tar czhvf cosmos/$(DTAR) cosmos/graphics cosmos/sounds/*
	rm -f cosmos_data-*
	ln $(DTAR) $(DTARD)

upload:	.buploaded .duploaded
	ssh warp "cd public_html/cosmos; make"

.buploaded:	$(BTAR)
	scp cosmos_binary-*.tar.gz warp:public_html/cosmos/
	touch .buploaded

.duploaded:	$(DTAR)
	scp cosmos_data-*.tar.gz warp:public_html/cosmos/
	touch .duploaded

tar:	archive
archive:	.
	cd .. ; tar czhvf ~/c/archive/cosmos.$(TSTR).tar.gz \
		cosmos/*.[hc] cosmos/*.cpp cosmos/data cosmos/Makefile \
		cosmos/cosmos cosmos/cosmos.* \
		cosmos/CREDITS \
		cosmos/sounds cosmos/graphics cosmos/*.csh

$(BIN):	$(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

cosmos.exe:	$(WOBJS)
	$(WCC) -o $@ $(WOBJS) $(WLIBS)

%.o:	%.cpp 
	$(CC) -c $<

%.win32_o:	%.cpp %.o
	$(WCC) -c $< -o $@

dep:	deps.mk

deps.mk:	*.cpp *.h data/*.h
	gcc -MM `sdl-config --cflags` *.cpp > deps.mk

include deps.mk
