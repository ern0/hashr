#### Makefile for hashr ################################

PLATFORM=$(shell uname -s)-$(shell uname -p)
CC=gcc -std=c99
SRC=src
OBJ=/tmp/hashr/${PLATFORM}
BIN=bin/${PLATFORM}

########################################################

${BIN}/hashr-server: \
		Makefile ${OBJ} ${BIN} \
		${SRC}/main.c \
		${OBJ}/Server.o
	${CC} \
		${SRC}/main.c \
		${OBJ}/Utils.o \
		${OBJ}/Server.o \
		-o ${BIN}/hashr-server

${OBJ}/Utils.o: \
		Makefile ${OBJ} \
		${SRC}/Utils.h \
		${SRC}/Utils.c
	${CC} -c \
		${SRC}/Utils.c \
		-o ${OBJ}/Utils.o

${OBJ}/Server.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${SRC}/Server.h \
		${SRC}/Server.c
	${CC} -c \
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