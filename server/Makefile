#### Makefile for hashr ################################

CC=gcc -c -std=c99
CL=gcc -std=c99
SRC=src
OBJ=/tmp/hashr
BIN=bin

########################################################

${BIN}/hashr-server: \
		Makefile \
		${SRC}/main.c \
		${OBJ}/Server.o
	${CL} \
		${SRC}/main.c \
		${OBJ}/Utils.o \
		${OBJ}/Server.o \
		-o ${BIN}/hashr-server

${OBJ}/Utils.o: \
		Makefile \
		${SRC}/Utils.h \
		${SRC}/Utils.c
	${CC} \
		${SRC}/Utils.c \
		-o ${OBJ}/Utils.o

${OBJ}/Server.o: \
		Makefile \
		${OBJ}/Utils.o \
		${SRC}/Server.h \
		${SRC}/Server.c
	${CC} \
		${SRC}/Server.c \
		-o ${OBJ}/Server.o

########################################################

${BIN}:
	mkdir -p ${BIN}

${OBJ}:
	mkdir -p ${OBJ}

clean: ${OBJ}
	rm -rf ${OBJ}/*.o

all: clean ${BIN}/hashr-server