#### Makefile for hashr ################################

PLATFORM=$(shell uname -s)-$(shell uname -p)
CC=gcc -std=c11
TC=g++
SRC=src
TST=test
OBJ=/tmp/hashr/${PLATFORM}
BIN=bin/${PLATFORM}

########################################################

${BIN}/hashr-server: \
		Makefile ${OBJ} ${BIN} \
		${SRC}/main.c \
		${OBJ}/Logger.o \
		${OBJ}/Server.o \
		${OBJ}/HashTable.o \
		${OBJ}/Connection.o \
		${OBJ}/Packet.o \
		${OBJ}/Command.o 
	${CC} \
		${SRC}/main.c \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/Server.o \
		${OBJ}/HashTable.o \
		${OBJ}/Connection.o \
		${OBJ}/Packet.o \
		${OBJ}/Command.o \
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

${OBJ}/HashTable.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${SRC}/HashTable.h \
		${SRC}/HashTable.c
	${CC} -c \
		${SRC}/HashTable.c \
		-o ${OBJ}/HashTable.o

${OBJ}/Connection.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${SRC}/Connection.h \
		${SRC}/Connection.c
	${CC} -c \
		${SRC}/Connection.c \
		-o ${OBJ}/Connection.o

${OBJ}/Packet.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/Connection.o \
		${SRC}/Packet.h \
		${SRC}/Packet.c
	${CC} -c \
		${SRC}/Packet.c \
		-o ${OBJ}/Packet.o

${OBJ}/Command.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${SRC}/Command.h \
		${SRC}/Command.c
	${CC} -c \
		${SRC}/Command.c \
		-o ${OBJ}/Command.o

########################################################

test: \
	${BIN}/test-Utils \
	${BIN}/test-Packet \

${BIN}/test-Utils: \
		Makefile ${OBJ} ${BIN} \
		${TST}/test-Utils.cpp \
		${OBJ}/Utils.o
	${TC} \
		${TST}/test-Utils.cpp \
		${OBJ}/Utils.o \
		-o ${BIN}/test-Utils

${BIN}/test-Packet: \
		Makefile ${OBJ} ${BIN} \
		${TST}/test-Packet.cpp \
		${OBJ}/Packet.o \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/Command.o 
	${TC} \
		${TST}/test-Packet.cpp \
		${OBJ}/Packet.o \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/Command.o \
		-o ${BIN}/test-Packet

########################################################

${BIN}:
	mkdir -p ${BIN}

${OBJ}:
	mkdir -p ${OBJ}

clean: ${OBJ}
	rm -rf ${OBJ}/*.o
	rm -rf ${BIN}/*.o

all: clean ${BIN}/hashr-server