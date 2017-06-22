CXXFLAGS=-std=c++11  -pedantic -Wall -Wextra -O -Wctor-dtor-privacy -Wnon-virtual-dtor -Woverloaded-virtual -Wsign-promo
CXXFLAGS+=$(shell pkg-config --cflags gtkmm-3.0)
LDLIBS+=$(shell pkg-config --libs gtkmm-3.0)
CXXFLAGS+=$(shell sdl-config --cflags )
LDLIBS+=$(shell sdl-config --libs ) -lSDL_mixer

miscom: miscom.cc game.o outmissile.o inmissile.o explosion.o city.o dome.o ufo.o
game.o: game.cc game.h outmissile.h inmissile.h explosion.h ufo.h
outmissile.o: outmissile.cc outmissile.h
inmissile.o: inmissile.cc inmissile.h
explosion.o: explosion.cc explosion.h
dome.o: dome.cc dome.h
city.o: city.cc city.h
ufo.o: ufo.cc ufo.h

zip:
	tar -czvf cs120-assignment-9-dlee140-kareem-jxing3.tar.gz ./*

clean:
	rm -rf miscom *.o
