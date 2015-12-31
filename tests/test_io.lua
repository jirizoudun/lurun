f = io.open("test.txt", "w")
f.write(f, "hello world\n")
f.write(f, "line no.2\n")
f.write(f, "Bye sweet world!\n")
io.close(f)

g = io.open("test.txt", "r")
print(g.read(g))
print(g.read(g))
print(g.read(g))
io.close(g)

