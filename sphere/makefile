all : sphere1.exe\
      sphere2.exe\
      sphere3.exe\
      sphere4.exe\
      sphere5.exe\
      sphere6.exe\

LIBRARIES = alleg
CC = gcc
OPTIONS = \
        -O2\
        -ffast-math\
        -fomit-frame-pointer\
        -s\
        -D__GTHREAD_HIDE_WIN32API

LIBS = $(addprefix -l,$(LIBRARIES))

%exe : %o
	$(CC) -s -o $@ $< $(LIBS)

%o : %c
	$(CC) -c $(OPTIONS) $<

