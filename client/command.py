from packet import *


class Command:

	def __init__(self):

		self.cmd = None
		
		self.requestKey = None
		self.requestValue = None
		
		self.searchMode = None
		self.searchKey = None
		self.searchValue = None

		self.reorgMethod = None
		self.reorgCapacity = None
		
		self.responseStatus = None
		self.responseKey = None
		self.responseValue = None



	def setCommand(self,cmd):
		self.cmd = cmd
		return self


	def setRequestKey(self,key):
		self.requestKey = key
		return self

	def setRequestValue(self,value):
		self.requestValue = value
		return self


	def setSearchMode(self,mode):
		self.searchMode = mode
		return self

	def setSearchKey(self,key):
		self.searchKey = key
		return self

	def setSearchValue(self,value):
		self.searchValue = value
		return self


	def setReorgMethod(self,method):
		self.reorgMethod = method
		return self

	def setReorgCapacity(self,capacity):
		self.setReorgCapacity = capacity
		return self


	def getResponseStatus(self):
		return self.responseStatus

	def getResponseKey(self):
		return self.responseKey

	def getResponseValue(self):
		return self.responseValue


	def render(self):

		packet = Packet()
		if self.cmd is not None: 
			packet.addBlock("CMND",self.cmd)
		if self.requestKey is not None: 
			packet.addBlock("QKEY",self.requestKey)
		if self.requestValue is not None:
			packet.addBlock("QVAL",self.requestValue)

		return packet.render()
