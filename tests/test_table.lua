t1 = {1,2,3}
t2 = t1

print(type(t1))
print(type(t2))
print(t1 == t2)

t1["test"] = "asdf"
print(t2["test"])
