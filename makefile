CC=gcc
CFLAGS=-O

OBJS= dove.obj winproc.obj pdlgproc.obj sdlgproc.obj process.obj \
 slider.obj rbutton.obj sbutton.obj efield.obj control.obj fdlgproc.obj \
 combobox.obj listbox.obj

%.obj: %.cc
	$(CC) $(CFLAGS) -c $<

dove.exe: $(OBJS) dove.def dove.res
	gcc $(CFLAGS) -static $(OBJS) dove.def -o dove.exe
	rc dove.res

dove.obj: dove.cc
winproc.obj: winproc.cc slider.h rbutton.h sbutton.h
slider.obj:  slider.cc slider.h control.h
combobox.obj:  combobox.cc combobox.h listbox.h efield.h control.h
efield.obj:  efield.cc efield.h control.h
listbox.obj:  listbox.cc listbox.h control.h
rbutton.obj: rbutton.cc rbutton.h control.h
sbutton.obj: sbutton.cc sbutton.h control.h
control.obj: control.cc control.h
process.obj: process.cc process.h
sdlgproc.obj: sdlgproc.cc dove.h sbutton.h efield.h
pdlgproc.obj: pdlgproc.cc dove.h sbutton.h efield.h control.h rbutton.h slider.h
fdlgproc.obj: fdlgproc.cc dove.h combobox.h listbox.h efield.h
dove.res: dove.rc dove2.ico
	rc -r dove

clean:
	del *.obj *.s *.map *.res *.exe
