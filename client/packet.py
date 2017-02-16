from block import *


class Packet:

	def __init__(self):
		self.blockList = []


	def _addStringBlock(self,type,data):

		block = Block()
		block.setType(bytearray(type,"UTF-8"))
		block.setString(data)

		self.blockList.append(block)


	def _addFileBlock(self,type,fnam):

		block = Block()
		block.setType(bytearray(type,"UTF-8"))

		file = open(fnam,mode="rb")
		data = file.read()
		file.close()
		block.setData(data)

		self.blockList.append(block)


	def addTextBlock(self,type,value):
		if value[0] == '@': self._addFileBlock(type,value[1:])
		else: self._addStringBlock(type,value)


	def addIntBlock(self,type,value):

		block = Block()
		block.setType(bytearray(type,"UTF-8"))
		block.setInt(value)

		self.blockList.append(block)


	def render(self):

		r = bytearray()
		r += b"HSHr"

		for block in self.blockList:
			r += block.render()

		r += b"endm"

		return r
