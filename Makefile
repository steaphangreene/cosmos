TSTR:=  $(shell date +"%Y%m%d%H%M")

CC:=	g++ -O2 -pipe -Wall `sdl-config --cflags`
LIBS:=	`sdl-config --libs`
OBJS:=	main.o gui.o \
	game.o galaxy.o system.o planet.o \
	audio.o fonts.o graphics.o
# graphics_intro.o

WCC:=   win32-gcc -O2 -pipe -Wall `win32-exec sdl-config --cflags`
WLIBS:= `win32-exec sdl-config --libs`
WOBJS:= $(shell echo $(OBJS) | sed 's-\.o-.win32_o-g')

basic:	x86

all:	x86 win

x86:	cosmos

win:	cosmos.exe

clean:	.
	rm -f cosmos cosmos.* *.o *.win32_o

distrib:	all
	cd .. ; tar czhvf cosmos_i386.tar.gz cosmos/cosmos \
		cosmos/sounds cosmos/graphics
	cd .. ; tar czhvf cosmos_win32.tar.gz cosmos/cosmos.exe \
		cosmos/sounds cosmos/graphics

upload:	distrib
	scp ../cosmos_*.tar.gz warp:public_html/cosmos/

tar:	archive
archive:	.
	cd .. ; tar czhvf ~/c/archive/cosmos.$(TSTR).tar.gz \
		cosmos/*.[hc] cosmos/*.cpp cosmos/data cosmos/Makefile \
		cosmos/sounds cosmos/graphics cosmos/*.csh

cosmos:	$(OBJS)
	$(CC) -s -o cosmos $(OBJS) $(LIBS)

cosmos.exe:	$(WOBJS)
	$(WCC) -s -o cosmos.exe $(WOBJS) $(WLIBS)

%.o:	%.cpp 
	$(CC) -c $<

%.win32_o:	%.cpp %.o
	$(WCC) -c $< -o $@

#AUTO-GENERATED DEPS BELOW (gcc -MM `sdl-config --cflags` *.cpp to regen)
audio.o: audio.cpp
fonts.o: fonts.cpp data/font22.h
galaxy.o: galaxy.cpp galaxy.h system.h planet.h
game.o: game.cpp game.h galaxy.h system.h planet.h
graphics.o: graphics.cpp graphics.h data/blank0.h data/blank1.h \
 data/star.h fonts.h
gui.o: gui.cpp gui.h game.h galaxy.h system.h planet.h audio.h \
 graphics.h
main.o: main.cpp gui.h game.h galaxy.h system.h planet.h fonts.h \
 audio.h
planet.o: planet.cpp planet.h
system.o: system.cpp system.h planet.h
