TSTR:=  $(shell date +"%Y%m%d%H%M")

CC:=	g++ -O2 -pipe -Wall `sdl-config --cflags`
LIBS:=	`sdl-config --libs`
OBJS:=	main.o gui.o audio.o game.o fonts.o graphics.o graphics_intro.o

WCC:=   win32-gcc -O2 -pipe -Wall `win32-exec sdl-config --cflags`
WLIBS:= `win32-exec sdl-config --libs`
WOBJS:= $(shell echo $(OBJS) | sed 's-\.o-.win32_o-g')

basic:	x86

all:	x86 win

x86:	cosmos

win:	cosmos.exe

clean:	.
	rm -f cosmos cosmos.* *.o *.win32_o

tar:	archive
archive:	.
	cd .. ; tar czhvf ~/c/archive/cosmos.$(TSTR).tar.gz \
		cosmos/*.[hc] cosmos/*.cpp cosmos/Makefile \
		cosmos/sounds cosmos/graphics cosmos/*.csh

cosmos:	$(OBJS)
	$(CC) -o cosmos $(OBJS) $(LIBS)

cosmos.exe:	$(WOBJS)
	$(WCC) -o cosmos.exe $(WOBJS) $(WLIBS)

%.o:	%.cpp 
	$(CC) -c $<

%.win32_o:	%.cpp %.o
	$(WCC) -c $< -o $@

#AUTO-GENERATED DEPS BELOW (gcc -MM `sdl-config --cflags` *.cpp to regen)
audio.o: audio.cpp
fonts.o: fonts.cpp font22.h
game.o: game.cpp game.h
graphics.o: graphics.cpp blank0.h blank1.h fonts.h
graphics_intro.o: graphics_intro.cpp intro.h
gui.o: gui.cpp gui.h game.h audio.h graphics.h
main.o: main.cpp gui.h game.h fonts.h audio.h
