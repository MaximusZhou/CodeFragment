require("sym_coroutine")

function foo1(n)
	print("1: foo1 received value "..n)
	n = coro.transfer(foo2,n + 10)

	print("2: foo1 received value "..n)
	n = coro.transfer(coro.main,n + 10)

	print("3: foo1 received value "..n)
	coro.transfer(coro.main,n + 10)
end

function foo2(n)
	print("1: foo2 received value "..n)
	n = coro.transfer(coro.main,n + 10)

	print("2: foo2 received value "..n)
	coro.transfer(foo1,n + 10)
end
function main()
	foo1 = coro.create(foo1)
	foo2 = coro.create(foo2)

	local n = coro.transfer(foo1,0)
	print("1: main received value "..n)

	n = coro.transfer(foo2,n + 10)
	print("2: main received value "..n)

	n = coro.transfer(foo1,n + 10)
	print("3: main received value "..n)
end

--��main��Ϊ������(Э��)
coro.main = main

--��coro.main��Ϊ��ǰЭ��
coro.current = coro.main

--��ʼִ��������(Э��)
coro.main()
