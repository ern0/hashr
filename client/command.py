from packet import *


class Command:

	def __init__(self):

		self.cmd = None
		self.counter = None
		
		self.requestKey = None
		self.requestValue = None
		
		self.searchFilter = None
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


	def setSearchFilter(self,filter):
		self.searchFilter = filter
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
			packet.addTextBlock("CMND",self.cmd)

		if self.counter is not None:
			packet.addIntBlock("cntr",self.counter)

		if self.requestKey is not None: 
			packet.addTextBlock("QKEY",self.requestKey)

		if self.requestValue is not None:
			packet.addTextBlock("QVAL",self.requestValue)

		if self.searchFilter is not None:
			packet.addTextBlock("SFIL",self.searchFilter)

		if self.searchPattern is not None:
			packet.addTextBlock("SPAT",self.searchPattern)

		if self.searchMaxResults is not None:
			packet.addIntBlock("SMAX",self.searchMaxResults)

		if self.reorgMethod is not None:
			packet.addTextBlock("RMET",self.reorgMethod)

		if self.reorgCapacity is not None:
			packet.addIntBlock("RCAP",self.reorgCapacity)

		return packet.render()
