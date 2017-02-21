#!/usr/bin/env python3

import sys
import os
import time
import signal
from readline import *
from threading import *
from socket import *
from select import *
import shlex

from packet import *
from command import *
from parser import *


class ParameterNumberException(Exception): pass
class ParameterValueException(Exception): pass
class UserExitException(Exception): pass


class Client(Thread):


	def printHelp(self):
		print(
"""This is a client for Hashr, the example key-value database.
Usage:
  """ + os.path.basename(sys.argv[0]) + """ [host [port]] [-c <command>]
    If no command is specified, CLI client will be started.
Available commands:
  help - voila
  quit, exit, q - leave client
  info, i - retrieve some info from server
  set, s <key> <value> - add or update an item
    use "@filename" as value to store a file
  get, g <key> - show item
  del, d <key> - delete item
  zap - delete all items
  results - show max. number of search item results
  results <n> - set max. number of search item results
  ksearch <pattern> - search in keys 
  vsearch <pattern> - search in values
  search <pattern> - search pattern in keys and values
  reorg <method> <capacity> - reorganize storage
    capacity will be rounded up to power of 2
Debug commands:
  i1 - invalid command: 'freebeer'
  i2 - missing parameters: 'set x'
  i3 - too many parameters: 'get x y'
  dump, z - server dumps full hash table to stdout
     does not work in daemon mode"""
		)


	def __init__(self):

		Thread.__init__(self)
		self.fakeAbort = False
		self.counter = 0

		self.responseTimeout = 4
		self.commandMutex = Lock()

		self.singleShotWords = []
		self.searchMaxResults = 5


	def fatal(self,message,abort = False):

		print("FATAL: " + message)

		if not abort: sys.exit(2)

		self.fakeAbort = True
		os.kill( os.getpid(),signal.SIGINT )


	def parseArgs(self):

		found = None
		item = 0
		for word in sys.argv[1:]:
			item += 1
			if word == "-c": 
				found = item
				continue
			if found is not None: self.singleShotWords.append(word)

		if found is None: found = 99

		self.host = None
		if found > 1: 
			try: self.host = sys.argv[1]
			except: pass
		if self.host == None: self.host = "localhost"

		self.port = 0
		if found > 2: 
			try: self.port = int( sys.argv[2] )
			except: pass
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
			" (life is easy)"
		)


	def run(self):

		command = Command()
		command.setCommand("beat")

		while True:

			self.performCommand( command )
			time.sleep(1)


	def performCommand(self,command):

		self.commandMutex.acquire()
		command.setCounter(self.counter)
		self.counter += 1;
		self.commandMutex.release()

		self.fireRequest( command.render() )
		result = self.waitResponse( command.getCounter() )

		return result


	def fireRequest(self,data):

		try: 
			self.sock.send(data)
		except BrokenPipeError:	
			self.fatal("server is not connected",True)
			return None


	def waitResponse(self,counter):

		ready = select([self.sock],[],[],self.responseTimeout)
		if ready[0]: result = self.sock.recv(9999)
		else: result = None

		# todo: check counter

		return result


	def parseLine(self,line):

		result = shlex.split(line)
		return result


	def parmNumCheck(self,words,num):

		parms = len(words) - 1
		if parms == num: return

		if parms < num: print("missing parameter")
		if parms > num: print("too many parameters")
		raise ParameterNumberException


	def validNum(self,numStr,minValue = 0):
		
		try: num = int(numStr)
		except ValueError: raise ParameterValueException
		if num < minValue: raise ParameterValueException

		return num


	def processCliCommand(self,words):

		try:

			if len(words) == 0: return None
			cmd = words[0].lower()

			if cmd == "help" or cmd == 'h':
				self.printHelp()
				return None

			elif cmd == "quit" or cmd == 'q':
				raise UserExitException
			
			elif cmd == "exit":
				raise UserExitException

			elif cmd == "results":

				if len(words) != 1: 
					self.parmNumCheck(words,1)
					self.searchMaxResults = self.validNum(words[1],0)
				
				if self.searchMaxResults == 0: res = "all"
				else: res = str(self.searchMaxResults)

				print("search result item number: " + res)
				return None

			command = Command()
			command.setCommand(cmd)

			if cmd == "info" or cmd == 'i':
				command.setCommand("info")
				self.parmNumCheck(words,0)

			elif cmd == "get" or cmd == 'g':
				command.setCommand("get")
				self.parmNumCheck(words,1)
				command.setRequestKey(words[1])

			elif cmd == "set" or cmd == 's':
				command.setCommand("set")
				self.parmNumCheck(words,2)
				command.setRequestKey(words[1])
				command.setRequestValue(words[2])

			elif cmd == "del" or cmd == 'd':
				command.setCommand("del")
				self.parmNumCheck(words,1)
				command.setRequestKey(words[1])

			elif cmd == "zap":
				self.parmNumCheck(words,0)

			elif cmd == "ksearch":
				self.parmNumCheck(words,1)
				command.setSearchFilter("key")
				command.setSearchPattern(words[1])
				command.setSearchMaxResults(self.searchMaxResults)

			elif cmd == "vsearch":
				self.parmNumCheck(words,1)
				command.setSearchFilter("value")
				command.setSearchPattern(words[1])
				command.setSearchMaxResults(self.searchMaxResults)

			elif cmd == "search":
				self.parmNumCheck(words,1)
				command.setSearchFilter("all")
				command.setSearchPattern(words[1])
				command.setSearchMaxResults(self.searchMaxResults)

			elif cmd == "reorg":
				self.parmNumCheck(words,2)
				command.setReorgMethod(words[1])
				command.setReorgCapacity( self.validNum(words[2],8) )

			elif cmd == "dump" or cmd == 'z':
				command.setCommand("dump")
				self.parmNumCheck(words,0)

			elif cmd == "i1":
				command.setCommand("freebeer")
				self.parmNumCheck(words,0)

			elif cmd == "i2":
				command.setCommand("set")
				self.parmNumCheck(words,0)

			elif cmd == "i3":
				command.setCommand("get")
				self.parmNumCheck(words,0)
				command.setRequestKey("getkey")
				command.setRequestValue("getvaluewtf")

			else:
				print("invalid command, type \"help\" for help")
				return None

			return self.performCommand(command)

		except ParameterNumberException: 
			return None

		except ParameterValueException:
			print("invalid parameter value")
			return None

		except FileNotFoundException:
			print("file not found")
			return None


	def performSingleShot(self):

		if len(self.singleShotWords) == 0: return False

		response = self.processCliCommand(self.singleShotWords)
		parser = Parser(response)
		parser.render()

		return True


	def cleanup(self):
		try: self.sock.shutdown(0)
		except: pass


	def cliLoop(self):

		while True:

			try: 
				line = input("hashr-" + str(self.session) + "> ")
			except EOFError: 
				break
			except KeyboardInterrupt: 
				if not self.fakeAbort: print(" - abort")
				break

			words = self.parseLine(line)
			try:
				response = self.processCliCommand(words)
			except UserExitException:
				break

			if response == None: continue

			parser = Parser(response)
			parser.render()


	def main(self):

		self.parseArgs()
		self.connect()
		if not self.performSingleShot():
			self.welcome()
			self.setDaemon(True)		
			self.start()

			self.cliLoop()

		self.cleanup()


if __name__ == "__main__":
	(Client()).main()
