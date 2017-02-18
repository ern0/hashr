#### Makefile for hashr ################################

PLATFORM=$(shell uname -s)-$(shell uname -p)
CC=gcc -std=c99
TC=g++
SRC=src
TST=test
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
		${OBJ}/Connection.o \
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
		${OBJ}/Connection.o \
		${SRC}/Server.h \
		${SRC}/Server.c
	${CC} -c \
		${SRC}/Server.c \
		-o ${OBJ}/Server.o

${OBJ}/Connection.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${SRC}/Connection.h \
		${SRC}/Connection.c
	${CC} -c \
		${SRC}/Connection.c \
		-o ${OBJ}/Connection.o

########################################################

test: \
	${BIN}/test-Utils \
	${BIN}/test-Connection \

${BIN}/test-Utils: \
		Makefile ${OBJ} ${BIN} \
		${TST}/test-Utils.cpp \
		${OBJ}/Utils.o
	${TC} \
		${TST}/test-Utils.cpp \
		${OBJ}/Utils.o \
		-o ${BIN}/test-Utils

${BIN}/test-Connection: \
		Makefile ${OBJ} ${BIN} \
		${TST}/test-Connection.cpp \
		${OBJ}/Connection.o \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o
	${TC} \
		${TST}/test-Connection.cpp \
		${OBJ}/Connection.o \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		-o ${BIN}/test-Connection

########################################################

${BIN}:
	mkdir -p ${BIN}

${OBJ}:
	mkdir -p ${OBJ}

clean: ${OBJ}
	rm -rf ${OBJ}/*.o

all: clean ${BIN}/hashr-server