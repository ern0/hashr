
class Parser:

	def __init__(self,data):
		self.data = data
		self.counter = None


	def getCounter(self):
		return self.counter


	def render(self):
		self.parse(True)


	def parse(self,render = False):
		
		if self.data[0:4] != b"HSHr":
			if render: print("invalid header: ",end="")
			self.renderAuto(self.data[0:4])
			return

		self.counter = None
		ptr = 4
		multi = False
		keyc = 0
		valc = 0
		kfound = False
		vfound = False
		while True:
			chunkType = self.data[ptr:ptr + 4]
			if chunkType == b"endm": break
			ptr += 4
			if ptr > len(self.data): break
			chunkLength = self.getInt(self.data[ptr:ptr + 4])
			ptr += 4
			chunkData = self.data[ptr:ptr + chunkLength]
			ptr += chunkLength

			if chunkType == b"AKEY":
				if kfound: multi = True
				kfound = True
			
			if chunkType == b"AVAL":
				if vfound: multi = True
				vfound = True

		ptr = 4
		while True:
			chunkType = self.data[ptr:ptr + 4]
			
			if chunkType == b"endm": break
			ptr += 4
			if ptr > len(self.data): break

			chunkLength = self.getInt(self.data[ptr:ptr + 4])
			ptr += 4

			chunkData = self.data[ptr:ptr + chunkLength]
			ptr += chunkLength

			if chunkType == b"cntr": 
				self.counter = self.getInt(chunkData)
				continue

			if not render: continue
			
			isInt = True
			desc = chunkType.decode("utf-8")			

			if chunkType == b"AKEY":
				isInt = False
				desc = "key"
				if multi: 
					desc += "[" + str(keyc) + "]"
					keyc += 1
			if chunkType == b"AVAL": 
				isInt = False
				desc = "value"
				if multi:
					desc += "[" + str(valc) + "]"
					valc += 1

			if len(chunkData) < 4: isInt = False

			if chunkType == b"STAT": desc = "status"
			if chunkType == b"NCON": desc = "connections"
			if chunkType == b"METD": desc = "hash method"
			if chunkType == b"CPTY": desc = "capacity"
			if chunkType == b"NELM": desc = "elements"
			if chunkType == b"ZAPD": desc = "zapped"
			if chunkType == b"SRES": desc = "match count"

			print("  " + desc + ": ",end="")
			for i in range(0,15 - len(desc)): print(" ",end="")

			if isInt:
				self.renderInt(chunkData[:4])
				
				if chunkType == b'STAT': 
					print(" ",end="")
					self.renderText(chunkData[4:])

			else:
				self.renderAuto(chunkData)

			print("")


	def getInt(self,data):

		r = data[0] << 24
		r |= data[1] << 16
		r |= data[2] << 8
		r |= data[3]

		return r


	def renderInt(self,data):
		v = self.getInt(data)
		print("%1d" % v,end="")


	def renderAuto(self,data):

		bin = False
		for b in data:
			if b < 32: bin = True
			if b > 126: bin = True
			if bin: break

		if bin: self.renderBin(data)
		else: self.renderText(data)


	def renderText(self,data):
		text = data.decode("utf-8") 
		print('"' + text + '"',end="")


	def renderBin(self,data):

		ptr = 0
		length = len(data)
		print();
		while True:

			print("    %08x: " % (ptr),end="")
			for i in range(0,8):
				if (ptr + i) < length:
					c = data[ptr + i]
					print("%2x " % c,end="")
				else:
					print("   ",end="")

			ptr += 8
			if ptr > length: break
			print("")

