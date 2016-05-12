.PHONY: clean all

.PRECIOUS: *.o

all: main.nes

clean:
	@rm -fv *.nes

reset.o: reset.s
	ca65 reset.s

neslib.o: neslib.s
	ca65 neslib.s

%.o: %.c
	cc65 -Oi $< --add-source -t nes
	ca65 $*.s
	# rm $*.s

%.nes: %.o reset.o neslib.o
	ld65 -o $@ reset.o neslib.o -C nes.cfg $< nes.lib
	rm neslib.o reset.o
	mv main.nes ../emesh.nes
