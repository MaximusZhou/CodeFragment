function foo(...)
	print("newcall")
	for _,v in ipairs(arg or {}) do
		io.write("    ",v,",")
	end
	print("\nendcall")
end

foo()
foo("2")
foo("1","ab")

--[[output:
newcall

endcall
newcall
    2,
endcall
newcall
	1,    ab,
endcall
--]]
