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
		${OBJ}/Logger.o \
		${OBJ}/Server.o \
		${OBJ}/ClientConnection.o \
		-o ${BIN}/hashr-server

${OBJ}/Utils.o: \
		Makefile ${OBJ} \
		${SRC}/Utils.h \
		${SRC}/Utils.c
	${CC} -c \
		${SRC}/Utils.c \
		-o ${OBJ}/Utils.o

${OBJ}/Logger.o: \
		Makefile ${OBJ} \
		${SRC}/Logger.h \
		${SRC}/Logger.c
	${CC} -c \
		${SRC}/Logger.c \
		-o ${OBJ}/Logger.o

${OBJ}/Server.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/ClientConnection.o \
		${SRC}/Server.h \
		${SRC}/Server.c
	${CC} -c \
		${SRC}/Server.c \
		-o ${OBJ}/Server.o

${OBJ}/ClientConnection.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${SRC}/ClientConnection.h \
		${SRC}/ClientConnection.c
	${CC} -c \
		${SRC}/ClientConnection.c \
		-o ${OBJ}/ClientConnection.o

########################################################

${BIN}:
	mkdir -p ${BIN}

${OBJ}:
	mkdir -p ${OBJ}

clean: ${OBJ}
	rm -rf ${OBJ}/*.o

all: clean ${BIN}/hashr-server