from packet import *


class Command:

	def __init__(self):

		self.cmd = None
		self.counter = None
		
		self.requestKey = None
		self.requestValue = None
		
		self.searchPattern = None
		self.searchMaxResults = None

		self.reorgMethod = None
		self.reorgCapacity = None
		

	def setCommand(self,cmd):
		self.cmd = cmd
		return self

	def setCounter(self,counter):
		self.counter = counter
		return self

	def getCounter(self):
		return self.counter


	def setRequestKey(self,key):
		self.requestKey = key
		return self

	def setRequestValue(self,value):
		self.requestValue = value
		return self


	def setSearchPattern(self,pattern):
		self.searchPattern = pattern
		return self

	def setSearchMaxResults(self,maxres):
		self.searchMaxResults = maxres
		return self


	def setReorgMethod(self,method):
		self.reorgMethod = method
		return self

	def setReorgCapacity(self,capacity):
		self.reorgCapacity = capacity
		return self


	def render(self):

		packet = Packet()

		if self.cmd is not None:
			token = self.getCommandToken()
			packet.addIntBlock("CMND",token)

		if self.counter is not None:
			packet.addIntBlock("cntr",self.counter)

		if self.requestKey is not None: 
			packet.addTextBlock("QKEY",self.requestKey)

		if self.requestValue is not None:
			packet.addTextBlock("QVAL",self.requestValue)

		if self.searchPattern is not None:
			packet.addTextBlock("SPAT",self.searchPattern)

		if self.searchMaxResults is not None:
			packet.addIntBlock("SMAX",self.searchMaxResults)

		if self.reorgMethod is not None:
			packet.addTextBlock("RMET",self.reorgMethod)

		if self.reorgCapacity is not None:
			packet.addIntBlock("RCAP",self.reorgCapacity)

		return packet.render()


	def getCommandToken(self):
		
		if self.cmd == "beat": return 0
		if self.cmd == "info": return 1
		if self.cmd == "get": return 2
		if self.cmd == "set": return 4
		if self.cmd == "del": return 5
		if self.cmd == "zap": return 6
		if self.cmd == "ksearch": return 11
		if self.cmd == "kcount": return 12
		if self.cmd == "vsearch": return 13
		if self.cmd == "vcount": return 14
		if self.cmd == "search": return 15
		if self.cmd == "count": return 16
		if self.cmd == "reorg": return 21
		if self.cmd == "dump": return 22
		if self.cmd == "freebeer": return 99
