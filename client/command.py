from packet import *


class Command:

	def __init__(self):

		self.cmd = None
		self.counter = None
		
		self.requestKey = None
		self.requestValue = None
		
		self.searchPattern = None
		self.searchLimitStart = None
		self.searchLimitItems = None

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

	def setSearchLimit(self,start,items):
		if start < 0: start = 0
		if items < 0: items = -1
		self.searchLimitStart = start
		self.searchLimitItems = items


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

		if self.searchLimitStart is not None:
			packet.addIntBlock("LIMS",self.searchLimitStart)

		if self.searchLimitItems is not None:
			packet.addIntBlock("LIMI",self.searchLimitItems)

		if self.reorgMethod is not None:
			packet.addIntBlock("RMET",self.reorgMethod)

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


	def getMethodName(self,method = None):

		if method is None: method = self.reorgMethod

		if method == 0: return "debug0"
		if method == 1: return "debug1"
		if method == 2: return "additive"
		if method == 3: return "xor"
		if method == 4: return "rotating"
		if method == 5: return "Bernstein"
		if method == 6: return "modified bernstein"
		if method == 7: return "shift-add-xor"
		if method == 8: return "Fowler-Noll-Vo"
		if method == 9: return "one-at-a-time"
		if method == 10: return "elf"

