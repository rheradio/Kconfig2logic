DIRS=Kconfig2Logic Logic2BDD counter looseVars extractVariables randomconf

UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
	CONFPARM=--libdir=`pwd`/../lib --includedir=`pwd`/../include CFLAGS="-g -O3" CXXFLAGS="-O3 -std=c++11 -g" --enable-shared --enable-obj --enable-dddmp
    endif
    ifeq ($(UNAME_S),Darwin)
        CONFPARM=--libdir=`pwd`/../lib --includedir=`pwd`/../include CFLAGS="-g -O2" CC=clang CXX=clang++ CXXFLAGS="-O2 -std=c++11 -stdlib=libc++ -g" --enable-shared --enable-obj --enable-dddmp
    endif

all:
	cd cudd-3.0.0 && touch configure Makefile.am Makefile.in aclocal.m4  config.h.in && ./configure $(CONFPARM)
	$(MAKE) -C cudd-3.0.0
	$(MAKE) -C cudd-3.0.0 install
	@for i in $(DIRS); do  $(MAKE) -C $$i; done
clean:
	@for i in $(DIRS); do $(MAKE) -C $$i clean; done
	$(MAKE) -C cudd-3.0.0 clean

