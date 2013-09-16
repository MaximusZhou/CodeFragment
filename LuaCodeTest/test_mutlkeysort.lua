

s = {{a = 2,b = 5},{a = 1,b = 6},{a = 2,b = 6},{a = 1,b = 3}}

function  tool(A,B)
 	return (A.a > B.a) or (A.a == B.a  and A.b > B.b)
end

table.sort(s,tool)

for _,v in pairs(s) do
	for k,t in pairs(v) do
		print(k,t)
	end
end
