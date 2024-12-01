PROGRAM = mandelbrot

all: $(PROGRAM) $(PROGRAM).hex

$(PROGRAM).hex: $(PROGRAM).p
	p2hex $<

.s.p:
	asl -cpu 1802 -i /usr/local/share/lcc1802/include -L $<

.c.s:
	lcc -DLCC -S -O $<

clean:; rm -f $(PROGRAM) *.{s,p,lst,hex}
