-----------------------------------
--测试Lua函数保存为字符串和二进制形式
-----------------------------------

local func_string = 
[[
function ()
	print("Test function dump")
end
]]

local temp_string = string.format(" return { fun = %s }",func_string)

local func = loadstring(temp_string)

print("func",func)  --output:func   function: 005CAF78

local temp_table = func()

print("temp_table",temp_table) --output: temp_table      table: 005CB580

temp_table.fun() --output: Test function dump


local BinaryFunc = string.dump(temp_table.fun)

print("BinaryFunc",BinaryFunc)  --a string  containing a binary representation of the given function

local OrgialFunc = loadstring(BinaryFunc) --for lua 5.1,can also use load in lua 5.2

OrgialFunc() --output:  Test function dump
