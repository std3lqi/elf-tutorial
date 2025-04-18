#
# This file is part of ELF Tutorial program.
#
# ELF Tutorial program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by the Free 
# Software Foundation, either version 3 of the License, or (at your option) any 
# later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT 
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with 
# this program. If not, see <https://www.gnu.org/licenses/>. 
#

FILES=./build/main.o ./build/elf-table.o ./build/window.o ./build/elf-reader.o ./build/elf-helper.o
INCLUDES=
CFLAGS=-fPIC -g

all: ${FILES}
	gcc -o main ${FILES} -lncurses

clean:
	rm -f ${FILES}

./build/main.o: src/main.c
	gcc ${CFLAGS} -o build/main.o -c src/main.c

./build/elf-reader.o: src/elf-reader.c
	gcc ${CFLAGS} -o build/elf-reader.o -c src/elf-reader.c

./build/elf-table.o: src/elf-table.c
	gcc ${CFLAGS} -o build/elf-table.o -c src/elf-table.c

./build/elf-helper.o: src/elf-helper.c
	gcc ${CFLAGS} -o build/elf-helper.o -c src/elf-helper.c

./build/window.o: src/window.c
	gcc ${CFLAGS} -o build/window.o -c src/window.c

