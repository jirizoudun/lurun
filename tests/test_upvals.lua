function add3 (x)        -- f1
  return function (y)    -- f2
    return function (z) return x + y + z end    -- f3
  end
end

print(add3(1)(2)(3))