class Block:


	def setType(self,type):
		self.type = type


	def setString(self,stringValue):
		self.setData(bytearray(stringValue,"UTF-8"))


	def setInt(self,value):

		v = bytearray()
		v.append( (value >> 24) & 0xff );
		v.append( (value >> 16) & 0xff );
		v.append( (value >> 8) & 0xff );
		v.append( value & 0xff );

		self.setData(v)


	def setData(self,binaryValue):
		self.data = binaryValue


	def render(self):

		r = bytearray()
		
		for c in self.type: r.append(c)

		size = len(self.data)
		r.append( (size & 0xff000000) >> 24)
		r.append( (size & 0xff0000) >> 16)
		r.append( (size & 0xff00) >> 8)
		r.append( size & 0xff )

		for b in self.data: r.append(b)

		return r
