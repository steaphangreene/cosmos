TSTR:=  $(shell date -u +"%Y%m%d%H%M")

BIN:=	cosmos.$(shell uname)-$(shell arch)
BTAR:=	cosmos_binary.tar.gz
BTARU:=	cosmos_binary.tar
BTARD:=	cosmos_binary-$(TSTR).tar.gz
DTAR:=	cosmos_data.tar.gz
DTARU:=	cosmos_data.tar
DTARD:=	cosmos_data-$(TSTR).tar.gz
BINS:=	        cosmos.exe cosmos.Linux-i686 cosmos.Linux-i586 \
	cosmos.Linux-sparc64 cosmos.SunOS-sun4 cosmos.Darwin-ppc
#	cosmos.Linux-ppc \


# Debugging settings
#CC:=	g++$(COSMOS_CTAIL) -DVERSION=\"pre$(TSTR)\" -DDEBUG=SDL_INIT_NOPARACHUTE -pipe -Wall `sdl-config --cflags` -g $(COSMOS_FLAGS)
#LIBS:=	`sdl-config --libs` -lefence $(COSMOS_LIBS)
#STRIP:=	touch

# Production settings
CC:=	g++$(COSMOS_CTAIL) -DVERSION=\"pre$(TSTR)\" -O2 -pipe -Wall `sdl-config --cflags` $(COSMOS_FLAGS)
LIBS:=	`sdl-config --libs` $(COSMOS_LIBS)
STRIP:=	strip

OBJS:=	main.o data.o gui.o audio.o fonts.o graphics.o \
	gui_page_galaxy.o gui_page_system.o gui_page_planet.o \
	gui_panel_colony.o gui_panel_fleet.o gui_panel_ship.o \
	sobject.o ship.o fleet.o game.o player.o colony.o \
	galaxy.o system.o planet.o satellite.o \
	techtree.o tech_tiny.o \

WCC:=   win32-gcc -DVERSION=\"pre$(TSTR)\" -O2 -pipe -Wall `win32-exec sdl-config --cflags` -s
WLIBS:= `win32-exec sdl-config --libs` -lstdc++
WOBJS:= $(shell echo $(OBJS) | sed 's-\.o-.win32_o-g')

local:	$(BIN)

clean:	.
	rm -f *.o *.win32_o

%.o:	%.cpp 
	$(CC) -c $<

dep:	deps.mk

deps.mk:	*.cpp *.h data/*.h
	gcc -MM `sdl-config --cflags` *.cpp > deps.mk

include deps.mk

$(BIN):	$(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)
	$(STRIP) $@

########THIS IS WHERE IT SPLITS OFF FROM THE LOCAL FILE!

all:	cosmos_src.tar.gz $(BINS) graphics/*.raw

#This has to be dependant on all - because it gets the version info!
data.o:	*.cpp *.h data/*.h

cosmos.Linux-i586:	cosmos_src.tar.gz $(OBJS)
	scp cosmos_src.tar.gz reactor:
	ssh reactor "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp TSTR=$(TSTR) -j2"
	scp reactor:cosmos/$@ .
	ssh reactor rm -rf cosmos cosmos_src.tar.gz

cosmos.Linux-sparc64:	cosmos_src.tar.gz $(OBJS)
	scp cosmos_src.tar.gz beta:
	ssh beta "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp TSTR=$(TSTR) -j2"
	scp beta:cosmos/$@ .
	ssh beta rm -rf cosmos cosmos_src.tar.gz

cosmos.SunOS-sun4:	cosmos_src.tar.gz $(OBJS)
	scp cosmos_src.tar.gz topaz:
	ssh topaz "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp TSTR=$(TSTR) -j8"
	scp topaz:cosmos/$@ .
	ssh topaz rm -rf cosmos cosmos_src.tar.gz

cosmos.Darwin-ppc:	cosmos_src.tar.gz $(OBJS)
	scp -P 2222 cosmos_src.tar.gz inkhead.org:
	ssh -p 2222 inkhead.org "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp TSTR=$(TSTR)"
	scp -P 2222 inkhead.org:cosmos/$@ .
	ssh -p 2222 inkhead.org rm -rf cosmos cosmos_src.tar.gz

cosmos.Linux-ppc:	cosmos_src.tar.gz $(OBJS)
	scp -P 2222 cosmos_src.tar.gz crazyduck.org:
	ssh -p 2222 crazyduck.org "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp TSTR=$(TSTR)"
	scp -P 2222 crazyduck.org:cosmos/$@ .
	ssh -p 2222 crazyduck.org rm -rf cosmos cosmos_src.tar.gz

$(BTAR):	cosmos $(BINS)
	rm -f $(BTAR)
	cd .. ; tar chvf cosmos/$(BTARU) cosmos/cosmos cosmos/cosmos.* \
		cosmos/README-SDL.txt cosmos/sdl.dll
	gzip -9 $(BTARU)
	rm -f cosmos_binary-*
	ln $(BTAR) $(BTARD)

$(DTAR):	sounds/* graphics/*
	rm -f $(DTAR)
	cd .. ; tar chvf cosmos/$(DTARU) cosmos/graphics cosmos/sounds/*
	gzip -9 $(DTARU)
	rm -f cosmos_data-*
	ln $(DTAR) $(DTARD)

upload:	.updatedweb

.updatedweb:	cosmos_src.tar.gz .buploaded .duploaded
	ssh warp "cd public_html/cosmos; make"
	touch .updatedweb

.buploaded:	$(BTAR)
	scp cosmos_binary-*.tar.gz warp:public_html/cosmos/
	touch .buploaded

.duploaded:	$(DTAR)
	scp cosmos_data-*.tar.gz warp:public_html/cosmos/
	touch .duploaded

make.tmp:	Makefile
	csplit -f make.tmp. Makefile /########/
	mv make.tmp.00 make.tmp
	rm -f make.tmp.*

cosmos_src.tar.gz:	make.tmp *.cpp *.[ch] data/*.h
	rm -f cosmos_src.tar.gz
	cd .. ; tar chvf cosmos/cosmos_src.tar \
		cosmos/*.[hc] cosmos/*.cpp cosmos/data cosmos/make.tmp \
		cosmos/cosmos cosmos/CREDITS cosmos/TODO cosmos/*.csh
	gzip -9 cosmos_src.tar

backup:	tar
	scp ~/c/archive/cosmos.$(TSTR).tar.gz stea@reactor:backup/
	scp ~/c/archive/cosmos.$(TSTR).tar.gz stea@megatron:backup/
	scp ~/c/archive/cosmos.$(TSTR).tar.gz stea@opal:backup/

tar:	archive
archive:	.
	cd .. ; tar chvf ~/c/archive/cosmos.$(TSTR).tar \
		cosmos/*.[hc] cosmos/*.cpp cosmos/data cosmos/Makefile \
		cosmos/cosmos cosmos/cosmos.* cosmos/CREDITS \
		cosmos/*.pov cosmos/graphics cosmos/*.csh
	gzip -9	~/c/archive/cosmos.$(TSTR).tar

cosmos.exe:	$(WOBJS)
	$(WCC) -o $@ $(WOBJS) $(WLIBS)

%.win32_o:	%.cpp %.o
	$(WCC) -c $< -o $@

graphics/planet00.raw:	planet00.pov
	povray -W768 -H768 planet00.pov
	convert planet00.png planet00.tga
	./tga2raw.csh planet00
	mv planet00.raw graphics/
	rm -f planet00.png
	rm -f planet00.tga

graphics/moon00.raw:	moon00.pov
	povray +W64 +H64 moon00.pov
	convert moon00.png moon00.tga
	./tga2raw.csh moon00
	mv moon00.raw graphics/
	rm -f moon00.png
	rm -f moon00.tga

graphics/moon01.raw:	moon01.pov
	povray +W64 +H64 moon01.pov
	convert moon01.png moon01.tga
	./tga2raw.csh moon01
	mv moon01.raw graphics/
	rm -f moon01.png
	rm -f moon01.tga

graphics/moon02.raw:	moon02.pov
	povray +W64 +H64 moon02.pov
	convert moon02.png moon02.tga
	./tga2raw.csh moon02
	mv moon02.raw graphics/
	rm -f moon02.png
	rm -f moon02.tga
