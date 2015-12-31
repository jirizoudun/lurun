local str = "You look beautiful today!"
local i = 1
local c = string.sub(str, i, i)
while c ~= "" do
    print(c)
    i = i + 1
    c = string.sub(str, i, i)
end
