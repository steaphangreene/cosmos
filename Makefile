TSTR:=  $(shell date --utc +"%Y%m%d%H%M")

BIN:=	cosmos.$(shell uname)-$(shell arch)
BTAR:=	cosmos_binary.tar.gz
BTARD:=	cosmos_binary-$(TSTR).tar.gz
DTAR:=	cosmos_data.tar.gz
DTARD:=	cosmos_data-$(TSTR).tar.gz

# Debugging settings
#CC:=	gcc -DDEBUG=SDL_INIT_NOPARACHUTE -pipe -Wall `sdl-config --cflags` -g $(COSMOS_FLAGS)
#LIBS:=	`sdl-config --libs` -lefence $(COSMOS_LIBS)

# Production settings
CC:=	gcc -O2 -pipe -Wall `sdl-config --cflags` -s $(COSMOS_FLAGS)
LIBS:=	`sdl-config --libs` $(COSMOS_LIBS)

OBJS:=	main.o gui.o gui_galaxy.o gui_system.o gui_planet.o \
	game.o galaxy.o system.o planet.o satellite.o techtree.o \
	audio.o fonts.o graphics.o
# graphics_intro.o

WCC:=   win32-gcc -O2 -pipe -Wall `win32-exec sdl-config --cflags` -s
WLIBS:= `win32-exec sdl-config --libs` -lstdc++
WOBJS:= $(shell echo $(OBJS) | sed 's-\.o-.win32_o-g')

basic:	$(BIN)

all:	$(BIN) cosmos.exe srctar cosmos.Linux-i586 \
	cosmos.Linux-sparc64 cosmos.SunOS-sun4 \
#	cosmos.Darwin-ppc \
#	cosmos.Linux-ppc

cosmos.Linux-i586:	$(OBJS)
	scp cosmos_src.tar.gz reactor:
	ssh reactor "tar xzf cosmos_src.tar.gz; cd cosmos/; make"
	scp reactor:cosmos/$@ .
	ssh reactor rm -rf cosmos cosmos_src.tar.gz

cosmos.Linux-sparc64:	$(OBJS)
	scp cosmos_src.tar.gz alpha:
	ssh alpha "tar xzf cosmos_src.tar.gz; cd cosmos/; make"
	scp alpha:cosmos/$@ .
	ssh alpha rm -rf cosmos cosmos_src.tar.gz

cosmos.SunOS-sun4:	$(OBJS)
	scp cosmos_src.tar.gz topaz:
	ssh topaz "tar xzf cosmos_src.tar.gz; cd cosmos/; make"
	scp topaz:cosmos/$@ .
	ssh topaz rm -rf cosmos cosmos_src.tar.gz

cosmos.Darwin-ppc:	$(OBJS)
	scp -P 2222 cosmos_src.tar.gz inkhead.org:
	ssh -p 2222 inkhead.org "tar xzf cosmos_src.tar.gz; cd cosmos/; make"
	scp -P 2222 inkhead.org:cosmos/$@ .
	ssh -p 2222 inkhead.org rm -rf cosmos cosmos_src.tar.gz

cosmos.Linux-ppc:	$(OBJS)
	scp -P 2222 cosmos_src.tar.gz 10.0.0.164:
	ssh -p 2222 10.0.0.164 "tar xzf cosmos_src.tar.gz; cd cosmos/; make"
	scp -P 2222 10.0.0.164:cosmos/$@ .
	ssh -p 2222 10.0.0.164 rm -rf cosmos cosmos_src.tar.gz

clean:	.
	rm -f *.o *.win32_o

distrib:	$(BTAR) $(DTAR)

$(BTAR):	cosmos cosmos.*
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

srctar:	.
	cd .. ; tar czhvf cosmos/cosmos_src.tar.gz \
		cosmos/*.[hc] cosmos/*.cpp cosmos/data cosmos/Makefile \
		cosmos/cosmos cosmos/CREDITS cosmos/*.csh

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
