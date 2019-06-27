.PHONY: build clean

KEYS = -g -c
INCDIR = -I./
OUTDIR = $(HOME)/vb_outfile

build: vb_error/vb_error.o vb_timer/vb_timer.o vb_testbench/vb_testbench.o vb_io/vb_io.o
	mkdir -p $(OUTDIR)
	mkdir -p $(OUTDIR)/include
	mkdir -p $(OUTDIR)/include/vb_io
	mkdir -p $(OUTDIR)/include/vb_error
	mkdir -p $(OUTDIR)/include/vb_testbench
	mkdir -p $(OUTDIR)/include/vb_timer
	mkdir -p $(OUTDIR)/lib
	ar -r $(OUTDIR)/lib/libvb_testbench.a $^
	ar -r $(OUTDIR)/lib/libvb_io.a vb_error/vb_error.o vb_io/vb_io.o
	flex flex/lex.gen
	gcc lex.yy.c -ll -o $(OUTDIR)/vbllex
	cp -f $(PWD)/vb_error/vb_error.h $(OUTDIR)/include/vb_error/vb_error.h
	cp -f $(PWD)/vb_io/vb_io.h $(OUTDIR)/include/vb_io/vb_io.h
	cp -f $(PWD)/vb_testbench/vb_testbench.h $(OUTDIR)/include/vb_testbench/vb_testbench.h
	cp -f $(PWD)/vb_timer/vb_timer.h $(OUTDIR)/include/vb_timer/vb_timer.h

%.o: %.cpp
	g++ $(KEYS) $(INCDIR) $< -o $@

clean:
	rm vb_error/vb_error.o vb_timer/vb_timer.o vb_io/vb_io.o vb_testbench/vb_testbench.o
