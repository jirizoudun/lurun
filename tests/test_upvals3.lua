function upper(x)
  function a()
    return x + 3;
  end

  x = x + 100

  function b()
    return x + 10
  end

  return a() + b();
end

print(upper(5))
print(upper(7))