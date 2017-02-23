#### Makefile for hashr ################################

PLATFORM=$(shell uname -s)-$(shell uname -p)
CC=gcc -std=c11 -O3
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
		${OBJ}/HashTable.o \
		${OBJ}/HashItem.o \
		${OBJ}/Hasher.o \
		${OBJ}/Connection.o \
		${OBJ}/Packet.o \
		${OBJ}/Command.o \
		${OBJ}/Search.o \
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
		${OBJ}/HashTable.o \
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
		${OBJ}/HashItem.o \
		${SRC}/HashTable.h \
		${SRC}/HashTable.c
	${CC} -c \
		${SRC}/HashTable.c \
		-o ${OBJ}/HashTable.o

${OBJ}/Search.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${SRC}/Search.h \
		${SRC}/Search.c
	${CC} -c \
		${SRC}/Search\.c \
		-o ${OBJ}/Search.o

${OBJ}/HashItem.o: \
		Makefile ${OBJ} \
		${OBJ}/Hasher.o \
		${OBJ}/Search.o \
		${SRC}/HashItem.h \
		${SRC}/HashItem.c
	${CC} -c \
		${SRC}/HashItem.c \
		-o ${OBJ}/HashItem.o

${OBJ}/Hasher.o: \
		Makefile ${OBJ} \
		${SRC}/Hasher.h \
		${SRC}/Hasher.c
	${CC} -c \
		${SRC}/Hasher.c \
		-o ${OBJ}/Hasher.o

${OBJ}/Connection.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/Packet.o \
		${SRC}/Connection.h \
		${SRC}/Connection.c
	${CC} -c \
		${SRC}/Connection.c \
		-o ${OBJ}/Connection.o

${OBJ}/Packet.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/Command.o \
		${SRC}/Packet.h \
		${SRC}/Packet.c
	${CC} -c \
		${SRC}/Packet.c \
		-o ${OBJ}/Packet.o

${OBJ}/Command.o: \
		Makefile ${OBJ} \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/Search.o \
		${SRC}/Command.h \
		${SRC}/Command.c
	${CC} -c \
		${SRC}/Command.c \
		-o ${OBJ}/Command.o

########################################################

test: \
	${BIN}/test-Utils \
	${BIN}/test-Packet_scan \
	${BIN}/test-Packet_build \
	${BIN}/test-HashTable \

${BIN}/test-Utils: \
		Makefile ${OBJ} ${BIN} \
		${TST}/test-Utils.cpp \
		${OBJ}/Utils.o
	${TC} \
		${TST}/test-Utils.cpp \
		${OBJ}/Utils.o \
		-o ${BIN}/test-Utils

${BIN}/test-Packet_scan: \
		Makefile ${OBJ} ${BIN} \
		${TST}/test-Packet_scan.cpp \
		${OBJ}/Server.o \
		${OBJ}/Utils.o
	${TC} \
		${TST}/test-Packet_scan.cpp \
		${OBJ}/Packet.o \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/Server.o \
		${OBJ}/Connection.o \
		${OBJ}/HashTable.o \
		${OBJ}/HashItem.o \
		${OBJ}/Hasher.o \
		${OBJ}/Command.o \
		${OBJ}/Search.o \		
		-o ${BIN}/test-Packet_scan

${BIN}/test-Packet_build: \
		Makefile ${OBJ} ${BIN} \
		${TST}/test-Packet_build.cpp \
		${OBJ}/Server.o \
		${OBJ}/Utils.o
	${TC} \
		${TST}/test-Packet_build.cpp \
		${OBJ}/Packet.o \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/Server.o \
		${OBJ}/Connection.o \
		${OBJ}/HashTable.o \
		${OBJ}/HashItem.o \
		${OBJ}/Hasher.o \
		${OBJ}/Command.o \
		${OBJ}/Search.o \		
		-o ${BIN}/test-Packet_build

${BIN}/test-HashTable: \
		Makefile ${OBJ} ${BIN} \
		${TST}/test-HashTable.cpp \
		${OBJ}/HashTable.o \
		${OBJ}/Utils.o
	${TC} \
		${TST}/test-HashTable.cpp \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/HashTable.o \
		${OBJ}/HashItem.o \
		${OBJ}/Hasher.o \
		${OBJ}/Search.o \		
		-o ${BIN}/test-HashTable

${BIN}/test-Search: \
		Makefile ${OBJ} ${BIN} \
		${TST}/test-Search.cpp \
		${OBJ}/Logger.o \
		${OBJ}/HashTable.o \
		${OBJ}/HashItem.o \
		${OBJ}/Hasher.o \
		${OBJ}/Search.o \
		${OBJ}/Utils.o
	${TC} \
		${TST}/test-Search.cpp \
		${OBJ}/Utils.o \
		${OBJ}/Logger.o \
		${OBJ}/HashTable.o \
		${OBJ}/HashItem.o \
		${OBJ}/Hasher.o \
		${OBJ}/Search.o \
		-o ${BIN}/test-Search

########################################################

${BIN}:
	mkdir -p ${BIN}

${OBJ}:
	mkdir -p ${OBJ}

clean: ${OBJ}
	rm -rf ${OBJ}/*.o
	rm -rf ${BIN}/*.o

all: clean ${BIN}/hashr-server