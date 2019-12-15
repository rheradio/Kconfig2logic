
ifeq ($(OS),Windows_NT)
    CCFLAGS += -D WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        CCFLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            CCFLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            CCFLAGS += -D IA32
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
       CXXFLAGS=-DKBUILD_NO_NLS -Wno-format-security -O3 -g -I. -I/usr/local/include  -L/opt/local/lib  
    endif
    ifeq ($(UNAME_S),Darwin)
        CXXFLAGS=-DKBUILD_NO_NLS -I/opt/local/include -I.  -L/opt/local/lib  -Wno-format-security -fcolor-diagnostics -O3 -Wno-deprecated-register -g 
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CCFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CCFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CCFLAGS += -D ARM
    endif
endif

../bin/randomconf: zconf.l zconf.y zconf.gperf
	flex -L -P zconf -o zconf.lex.c zconf.l
	bison -t -l -b zconf -p zconf zconf.y
	gperf -t --output-file zconf.hash.c -a -C -E -g -k '1,3,$$' -P -t zconf.gperf
	$(CC) $(CXXFLAGS) -o $@ conf.c zconf.tab.c -I. $(LDFLAGS)

.PHONY: clean

clean:
	-@rm -f ../bin/randomconf

