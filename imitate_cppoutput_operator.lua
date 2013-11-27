-----------------------------------
--用lua实现类似于c++中的输出操作符<<
--来自：http://lua-users.org/wiki/CustomOperators
-----------------------------------

local CustomOp = {}
function CustomOp:__div(b)
	return getmetatable(self.a)["__" .. self.op](self.a,b)
end
setmetatable(CustomOp, {__call =
	function(class,a,op)
		return setmetatable({a = a,op = op},CustomOp) --注意函数setmetatable返回的是要设置的元表的table
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
	return self  --这样可以连续执行输出操作了
end
setmetatable(osstream,{__call = 
	function(class,file)
		file = file or io.output()
		return setmetatable({file = file},osstream)
	end
})

cout = osstream()
endl = "\n"

--例子
local _=cout / '<<' / 'hello' / '<<' / ',world!' / '<<' / endl
