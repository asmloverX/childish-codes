# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

CC = cc -G 0
INCDIR =-I../userprog -I../threads
CFLAGS = -c $(INCDIR)
LDFLAGS = -N -T 0
# gcc LDFLAGS = -N -Ttext 0

AS=as
ASFLAGS = $(INCDIR)

all: halt shell matmult sort

halt: halt.o start.o
	ld $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell: shell.o start.o
	ld $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort: sort.o start.o
	ld $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult: matmult.o start.o
	ld $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult
