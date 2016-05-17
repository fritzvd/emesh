.PHONY: clean all

.PRECIOUS: *.o

all: main.nes

clean:
	@rm -fv *.nes

crt0.o: base/crt0.s
	ca65 base/crt0.s

%.o: %.c
	cc65 -Oi $< --add-source -t nes
	ca65 $*.s
	rm $*.s

%.nes: %.o crt0.o
	ld65 -o $@ base/crt0.o -C base/nes.cfg $< base/nes.lib
	rm *.o
	mv main.nes ../emesh.nes
