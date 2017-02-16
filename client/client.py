#!/usr/bin/env python3

import sys
import os
import time
import signal
from readline import *
from threading import *

from socket import *
from select import *
from packet import *
from command import *


class Client(Thread):


	def __init__(self):

		Thread.__init__(self)
		self.fakeAbort = False
		self.counter = 0

		self.responseTimeout = 4


	def fatal(self,message,abort = False):

		print("FATAL: " + message)

		if not abort: sys.exit(2)

		self.fakeAbort = True
		os.kill( os.getpid(),signal.SIGINT )


	def parseLine(self,line):
		return ["help"]


	def parseArgs(self):

		self.host = sys.argv[1]
		if self.host == None: self.host = "localhost"
		self.port = int( sys.argv[2] )
		if self.port == 0: self.port = 8000


	def connect(self):

		self.sock = socket(AF_INET,SOCK_STREAM)
		try: self.sock.connect((self.host,self.port))
		except: self.fatal("can not connect to " + self.host + ":" + str(self.port))
		
		try: self.session = int( self.sock.recv(99) )
		except: self.session = 0
		if self.session == 0: self.fatal("connection rejected by the server")


	def welcome(self):
		print(
			"hashr-client"
			" - type \"help\" for help"
			", \"quit\" for quit"
			" (life is simple)"
		)


	def run(self):

		while True:

			command = Command()
			command.setCommand("INFO")
			self.fireCommand( command )

			time.sleep(1)


	def fireCommand(self,command):

		command.setCounter(self.counter)
		self.counter += 1;

		self.fireRaw( command.render() )


	def fireRaw(self,data):

		try: 
			self.sock.send(data)
		except BrokenPipeError:	
			self.fatal("server is not connected",True)
			return None

		ready = select([self.sock],[],[],self.responseTimeout)
		if ready[0]: result = self.sock.recv(9999)
		else: result = None

		print(result)


	def main(self):

		self.parseArgs()
		self.connect()
		self.welcome()

		self.setDaemon(True)		
		self.start()

		while True:

			try: 
				line = input("hashr-" + str(self.session) + "> ")
			except EOFError: 
				break
			except KeyboardInterrupt: 
				if not self.fakeAbort: print("- abort")
				break

				words = self.parseLine(line)
				processCommand(words)

			print(line)

			command = Command()
			command.setCommand("QUIT")
			self.fireCommand( command )

		try: self.sock.shutdown(0)
		except: pass
		
		# resp = sock.recv(20000)
		# print(resp)


if __name__ == "__main__":
	(Client()).main()