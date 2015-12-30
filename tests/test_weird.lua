u = {['@!#'] = 'qbert', [{}] = 1729, [6.28] = 'tau'}
print(u[6.28])

a = u['@!#']
b = u[{}]

function h(x) print(x.key1) end
h{key1 = 'Sonmi~451'}

res = {}
for key, val in pairs(u) do

    t = type(key)
    if res[t] == nil then
        res[t] = 0
    end
    res[t] = res[t] + 1
end

print("string", res["string"])
print("table", res["table"])
print("number", res["number"])
