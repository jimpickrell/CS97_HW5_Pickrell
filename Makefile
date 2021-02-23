# Make x86-64 random byte generators.

# Copyright 2015, 2020 Paul Eggert

# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

# Optimization level.  Change this -O2 to -Og or -O0 or whatever.
OPTIMIZE =

# The C compiler and its options.
CC = gcc
CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -fanalyzer \
  -march=native -mtune=native -mrdrnd

# The archiver command, its options and filename extension.
TAR = tar
TARFLAGS = --xz --transform 's,^,randall/,'
TAREXT = txz

#default: all

#randall: randall.c
#	$(CC) $(CFLAGS) $@.c -o $@

# $@ matches the target; $< matches the first dependent
# hello.exe: hello.o
#	gcc -o $@ $<

all: cpu_check.o output.o rand64-hw.o rand64-sw.o randall.o options.o
	gcc -o randall cpu_check.o output.o rand64-hw.o rand64-sw.o randall.o options.o

cpu_check.o: cpu_check.c 
	gcc -c cpu_check.c -o cpu_check.o 

output.o: output.c 
	gcc -c output.c -o output.o 

rand64-hw.o: rand64-hw.c 
	gcc -c  -mrdrnd rand64-hw.c -o rand64-hw.o 

rand64-sw.o: rand64-sw.c 
	gcc -c rand64-sw.c -o rand64-sw.o 

randall.o: randall.c 
	gcc -c randall.c -o randall.o 

options.o: options.c
	gcc -c options.c -o options.o

check: all
#	a=$(./randall 5 | wc -c); 
#	if [ $a -eq 5 ];
#		then echo "success"; 
#		else echo "fail" 
#	fi
	./check_script


#assignment: randall-assignment.$(TAREXT)
##assignment-files = COPYING Makefile randall.c#
#	assignment-files = Makefile *.c

#randall-assignment.$(TAREXT): $(assignment-files)
#	$(TAR) $(TARFLAGS) -cf $@ $(assignment-files)
#	tar -cvf $@ Makefile *.c *.txt

#submission: randall-submission.$(TAREXT)
#submission-files = $(assignment-files) \
##  notes.txt # More files should be listed here, as needed.
#randall-submission.$(TAREXT): $(submission-files)
#	$(TAR) $(TARFLAGS) -cf $@ $(submission-files)

myfiles = \
	options.c options.h \
	randall.c randall.h \
	rand64-hw.c rand64-hw.h \
	rand64-sw.c rand64-sw.h \
	output.c output.h \
	notes.txt \
	MAKEFILE

submission: $(myfiles)
	tar -czf randall-submission.tgz $(myfiles)

gitsubmission: randall-submission.tgz
	tar -czf randall-git.tgz .git


.PHONY: default clean assignment submission

clean:
	rm -f *.o *.$(TAREXT) randall
