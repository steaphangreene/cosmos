TSTR:=  $(shell date -u +"%Y%m%d%H%M")
VERSION:=	pre$(TSTR)

#BIN:=	cosmos.$(shell uname)-$(shell arch)
BIN:=	cosmos.Local
BTAR:=	cosmos_binary.tar.gz
BTARU:=	cosmos_binary.tar
BTARD:=	cosmos_binary-$(TSTR).tar.gz
DTAR:=	cosmos_data.tar.gz
DTARU:=	cosmos_data.tar
DTARD:=	cosmos_data-$(TSTR).tar.gz
BINS:=	cosmos.Linux-i686 cosmos.Linux-i586 \
	cosmos.Linux-sparc64 cosmos.SunOS-sun4 cosmos.Darwin-ppc \
	cosmos.Linux-ppc cosmos.exe \

COUNTPS:=	$(shell grep ';p();' gui.cpp | grep -v '//.*;p();' | wc -l | sed 's- --g')

# Debugging settings
#CC:=	gcc$(COSMOS_CTAIL) -DVERSION=\"$(VERSION)\" -DCOUNTPS=$(COUNTPS) -DDEBUG=SDL_INIT_NOPARACHUTE -pipe -Wall `sdl-config --cflags` -g $(COSMOS_FLAGS)
#CCC:=	g++$(COSMOS_CTAIL) -DVERSION=\"$(VERSION)\" -DCOUNTPS=$(COUNTPS) -DDEBUG=SDL_INIT_NOPARACHUTE -pipe -Wall `sdl-config --cflags` -g $(COSMOS_FLAGS)
#LIBS:=	`sdl-config --libs` -lefence $(COSMOS_LIBS) `gcc$(COSMOS_CTAIL) -print-file-name=libstdc++.a`
#STRIP:=	touch

# Production settings
CC:=	gcc$(COSMOS_CTAIL) -DVERSION=\"$(VERSION)\" -DCOUNTPS=$(COUNTPS) -O2 -pipe -Wall `sdl-config --cflags` $(COSMOS_FLAGS)
CCC:=	g++$(COSMOS_CTAIL) -DVERSION=\"$(VERSION)\" -DCOUNTPS=$(COUNTPS) -O2 -pipe -Wall `sdl-config --cflags` $(COSMOS_FLAGS)
LIBS:=	`sdl-config --libs` $(COSMOS_LIBS) `gcc$(COSMOS_CTAIL) -print-file-name=libstdc++.a`
STRIP:=	strip

OBJS:=	main.o data.o dict.o gui.o audio.o fonts.o graphics.o \
	gui_page_galaxy.o gui_page_system.o gui_page_planet.o \
	gui_panel_colony.o gui_panel_fleet.o gui_panel_ship.o \
	sobject.o ship.o fleet.o position.o game.o player.o colony.o \
	galaxy.o system.o planet.o satellite.o \
	techtree.o tech_tiny.o \

WCC:=   win32-gcc -DVERSION=\"$(VERSION)\" -DCOUNTPS=$(COUNTPS) -O2 -pipe -Wall `win32-exec sdl-config --cflags` -s
WLIBS:= `win32-exec sdl-config --libs` -lstdc++
WOBJS:= $(shell echo $(OBJS) | sed 's-\.o-.win32_o-g')

local:	$(BIN)

clean:	.
	rm -f *.o renderer/*.o *.win32_o deps.mk

demo_ships:	demo_ships.cpp renderer/renderer.o renderer/linear.o graphics_ship.o
	$(CCC) -o $@ demo_ships.cpp renderer/linear.o renderer/renderer.o graphics_ship.o $(LIBS) -L/usr/X11R6/lib -lGL -lGLU

%.o:	%.cpp 
	$(CCC) -c $< -o $@

%.win32_o:	%.cpp %.o
	$(WCC) -c $< -o $@

dep:	deps.mk

deps.mk:	*.cpp *.h data/*.h
	gcc -MM `sdl-config --cflags` *.cpp > deps.mk

include deps.mk

cosmos.Windows:	$(WOBJS)
	$(WCC) -o $@ $(WOBJS) $(WLIBS)

$(BIN):	$(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)
	$(STRIP) $@

########THIS IS WHERE IT SPLITS OFF FROM THE LOCAL FILE!

all:	cosmos_src.tar.gz $(BINS) graphics/*.raw

#This has to be dependant on all - because it gets the version info!
data.o:	data.cpp *.cpp *.h data/*.h
data.cpp:	data.t CREDITS
	cat data.t \
		| sed "s#CREDITS#`cat CREDITS | tr '\n' '+'`#g" \
		| sed 's-+-\\n"+"-g' | tr '+' '\n' \
		> data.cpp

dict.cpp:	dict.t dict.txt
	csplit -f dict.t. dict.t '/DICT/'
	cat dict.t.00 \
		| sed s#SIZE#`wc -l dict.txt | cut -c1-8 | sed "s- --g"`#g \
		> dict.cpp
	cat dict.txt | tr '\n' '#' \
		| sed 's-^-@"-g' | sed 's-#-",#@"-g' | sed 's-,#@"$$-#-g' \
		| tr '@' '\t' | tr '#' '\n' >> dict.cpp
	tail +2 dict.t.01 >> dict.cpp
	rm -f dict.t.*

cosmos.exe:	cosmos_src.tar.gz $(OBJS)
	scp cosmos_src.tar.gz marine:
	ssh marine "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp VERSION=$(VERSION) -j2 cosmos.Windows"
	scp marine:cosmos/cosmos.Windows ./$@
	ssh marine rm -rf cosmos cosmos_src.tar.gz

cosmos.Linux-i586:	cosmos_src.tar.gz $(OBJS)
	scp cosmos_src.tar.gz reactor:
	ssh reactor "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp VERSION=$(VERSION) -j2 local"
	scp reactor:cosmos/cosmos.Local ./$@
	ssh reactor rm -rf cosmos cosmos_src.tar.gz

cosmos.Linux-i686:	cosmos_src.tar.gz $(OBJS)
	scp cosmos_src.tar.gz reserve:
	ssh reserve "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp VERSION=$(VERSION) -j2 local"
	scp reserve:cosmos/cosmos.Local ./$@
	ssh reserve rm -rf cosmos cosmos_src.tar.gz

cosmos.Linux-sparc64:	cosmos_src.tar.gz $(OBJS)
	scp cosmos_src.tar.gz beta:
	ssh beta "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp VERSION=$(VERSION) -j2 local"
	scp beta:cosmos/cosmos.Local ./$@
	ssh beta rm -rf cosmos cosmos_src.tar.gz

cosmos.SunOS-sun4:	cosmos_src.tar.gz $(OBJS)
	scp cosmos_src.tar.gz topaz:
	ssh topaz "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp VERSION=$(VERSION) -j8 local"
	scp topaz:cosmos/cosmos.Local ./$@
	ssh topaz rm -rf cosmos cosmos_src.tar.gz

cosmos.Darwin-ppc:	cosmos_src.tar.gz $(OBJS)
	scp cosmos_src.tar.gz inkhead-darwin:
	ssh inkhead-darwin "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp VERSION=$(VERSION) local"
	scp inkhead-darwin:cosmos/cosmos.Local ./$@
	ssh inkhead-darwin rm -rf cosmos cosmos_src.tar.gz

cosmos.Linux-ppc:	cosmos_src.tar.gz $(OBJS)
	scp cosmos_src.tar.gz inkhead-ppc:
	ssh inkhead-ppc "tar xzf cosmos_src.tar.gz; cd cosmos/; make -f make.tmp VERSION=$(VERSION) local"
	scp inkhead-ppc:cosmos/cosmos.Local ./$@
	ssh inkhead-ppc rm -rf cosmos cosmos_src.tar.gz

#cosmos.Linux-ppc:	cosmos.Local
#	cp -av cosmos.Local $@

$(BTAR):	cosmos $(BINS)
	rm -f $(BTAR)
	cd .. ; tar chvf cosmos/$(BTARU) cosmos/cosmos cosmos/cosmos.*-* \
		cosmos/cosmos.exe cosmos/README-SDL.txt cosmos/sdl.dll
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

cosmos_src.tar.gz:	make.tmp *.cpp *.[cht] data/*.h
	rm -f cosmos_src.tar.gz
	cd .. ; tar chvf cosmos/cosmos_src.tar \
		cosmos/*.[hct] cosmos/*.cpp cosmos/data cosmos/make.tmp \
		cosmos/cosmos cosmos/CREDITS cosmos/TODO \
		cosmos/*.csh
	gzip -9 cosmos_src.tar

backup:	tar
	scp ~/c/archive/cosmos.$(TSTR).tar.gz stea@reactor:backup/
	scp ~/c/archive/cosmos.$(TSTR).tar.gz stea@megatron:backup/
	scp ~/c/archive/cosmos.$(TSTR).tar.gz stea@opal:backup/

tar:	archive
archive:	.
	cd .. ; tar chvf ~/c/archive/cosmos.$(TSTR).tar \
		cosmos/*.[hct] cosmos/*.cpp cosmos/data cosmos/Makefile \
		cosmos/cosmos cosmos/cosmos.* cosmos/CREDITS cosmos/dict.txt \
		cosmos/*.pov cosmos/graphics cosmos/*.csh
	gzip -9	~/c/archive/cosmos.$(TSTR).tar

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
