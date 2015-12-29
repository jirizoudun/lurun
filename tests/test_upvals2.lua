function add3 (x)        -- f1
  return function (y)    -- f2
    return function (z) return x + y + z end    -- f3
  end
end

a = add3(1)(2)
print(a(3))
print(a(5))

b = add3(11)(2)
print(b(30))
print(b(50))

c = add3(11)(200)
print(b(300))
print(b(500))