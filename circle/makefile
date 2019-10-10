all : circ1.exe\
	  circ2.exe\
	  circ3.exe\
	  circ4.exe\
	  circ5.exe\
	  circ6.exe\
	  circ7.exe\
	  circ8.exe\
	  circ9.exe\
	  circ10.exe\
	  circ11.exe\
	  circ12.exe

# Comment out for linux, or run with `make WINDOWS=`
WINDOWS=1

#----------------------------------------------------------------------------
CC = gcc

ALLEGRO_MODULES=allegro
ALLEGRO_LIBS = $(addsuffix -5, $(ALLEGRO_MODULES))
LIBS = -lm

ifdef WINDOWS
	CFLAGS += -D__GTHREAD_HIDE_WIN32API
	LFLAGS += -Wl,--subsystem,windows
	LIBS += -lallegro_monolith
else
	ALLEGRO_MODULES=allegro allegro_primitives allegro_main 
	ALLEGRO_LIBS = $(addsuffix -5, $(ALLEGRO_MODULES))
	LIBS += `pkg-config --libs $(ALLEGRO_LIBS)`
endif

%exe : %o
	$(CC) -s -o $@ $< $(LIBS)

%o : %c
	$(CC) -c $(OPTIONS) $<

