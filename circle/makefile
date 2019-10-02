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

LIBRARIES = alleg \
            m

#----------------------------------------------------------------------------
CC = gcc

OPTIONS = \
        -O2\
        -ffast-math\
        -fomit-frame-pointer\
        -s

ALLEGRO_MODULES=allegro
ALLEGRO_LIBS = $(addsuffix -5, $(ALLEGRO_MODULES))
LIBS = -lm
LIBS += `pkg-config --libs $(ALLEGRO_LIBS)`


%exe : %o
	$(CC) -s -o $@ $< $(LIBS)

%o : %c
	$(CC) -c $(OPTIONS) $<

