--a = 100
local a = 100
local foo = function()
	print(a)
end

local f,msg = load(foo)  --whether a is local, always output 100
--local f,msg = load("local b = 200  print(a, b)") --if a is local, output nil, 200, or output 100, 200
--local f,msg = load(string.dump(foo)) --if a is local, output nil, or output 100
if f then
	f()
else
	print("erro:", msg)
end

