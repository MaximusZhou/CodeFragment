#! /usr/local/bin/lua

function gen_foo()
	local i = 0
	return function ()
		i = i + 1 
		print(i)
	end
end

f1 = gen_foo()
f2 = gen_foo()

f1()  --1
f1()  --2

f2()  --1
f2()  --2
