-----------------------------------
--��luaʵ��������c++�е����������<<
--���ԣ�http://lua-users.org/wiki/CustomOperators
-----------------------------------

local CustomOp = {}
function CustomOp:__div(b)
	return getmetatable(self.a)["__" .. self.op](self.a,b)
end
setmetatable(CustomOp, {__call =
	function(class,a,op)
		return setmetatable({a = a,op = op},CustomOp) --ע�⺯��setmetatable���ص���Ҫ���õ�Ԫ���table
	end
})

function enable_cution_ops(mt)
	function mt:__div(op)
		return CustomOp(self,op)
	end
end

osstream = {}
osstream.__index = osstream
enable_cution_ops(osstream)
function osstream:write(s)
	io.write(s)
end
osstream['__<<'] = function(self,s)
	self:write(s)
	return self  --������������ִ�����������
end
setmetatable(osstream,{__call = 
	function(class,file)
		file = file or io.output()
		return setmetatable({file = file},osstream)
	end
})

cout = osstream()
endl = "\n"

--����
local _=cout / '<<' / 'hello' / '<<' / ',world!' / '<<' / endl
