--
-- Created by IntelliJ IDEA.
-- User: zoudun
-- Date: 28/12/15
-- Time: 00:40
-- To change this template use File | Settings | File Templates.
--

-- table node:
childs = 'childs' -- table
keys = 'keys' -- table
parent = 'parent' -- table
is_leaf = 'isLeaf' -- bool
num_keys = 'numKeys' -- int
next = 'next' -- table

-- Order of the tree.
-- max order: order - 1
-- min order: ~order/2
order = 3

-- Helper for printig tree.
queue = nil

local function istable(t) return type(t) == 'table' end

print('LUA B+ tree key-value storage')

function init_node(node)
    if node == nil then
        node = {}
    end

    node[childs] = {}
    node[keys] = {}
    node[parent] = nil
    node[is_leaf] = true -- TODO really?
    node[num_keys] = 0
    node[next] = nil -- used only for queue
end

-- Creates node
function make_node()
    local n = {};
    init_node(n)
    n[is_leaf] = false
    return n
end

-- Creates leaf
function make_leaf()
    local n = make_node
    n[is_leaf] = true
    return n
end

-- Helper method for printing the tree out.
function enqueue(new_node)
    if(queue == nil) then
        queue = new_node
        queue[next] = nil
    else
        local c = queue
        while c[next] do
            c = c[next]
        end
        c[next] = new_node
        new_node[next] = nil
    end
end

-- Helper method for printing the tree out.
function dequeue()
    local n = queue
    queue = queue[next]
    n[next] = nil
    return n
end

-- Prints the bottom row of keys of the tree (with their respective pointers)
function print_leaves(root)
    local c = root
    if root == nil then
        print('Empty tree')
        return
    end
    -- Get first child
    while not c[is_leaf] do
        c = c[childs][0]
    end

    while true do
        for i=0,c[num_keys] - 1 do
            io.write(tostring(c[keys][i]))
            io.write(' - ')
            io.write(tostring(c[childs][i]))
        end

        if c[childs][order - 1] ~= nil then
            io.write(' | ')
            c = c[childs][order - 1]
        else
            break
        end
    end
    io.write('\n')
end

-- Utility function to give the height of the tree, which length in number of edges of the path from the root to any leaf.
function height(root)
    local h = 0
    local c = root
    while c[is_leaf] == false do
        assert(c[childs])
        assert(c[childs][0])
        c = c[childs][0]
        h = h + 1
    end
    return h
end

-- Utility function to give the length in edges of the path from any node to the root.
function path_to_root(root, node)
    local length = 0
    local c = node
    while c ~= root do
        assert(c[parent])
        c = c[parent]
        length = length + 1
    end
    return length
end

-- Prints the B+ tree in the command line in level (rank) order, with the keys in each node and the '|' symbol
-- to separate nodes. The values of the pointers corresponding to the keys also appear next to their respective
-- keys.
function print_tree(root)
    local n
    local i
    local rank = 0
    local new_rank

    if root == nil then
        print('Empty tree')
        return
    end

    queue = nil
    enqueue(root)
    while queue ~= nil do
        n = dequeue()
        if n[parent] ~= nil and n == n[parent][childs][0] then -- TODO checking
            new_rank = path_to_root(root, n)
            if new_rank ~= rank then
                rank = new_rank
                io.write('\n')
            end
        end
        io.write('(')
        if istable(n) then
            io.write(type(n))
        else
            io.write(tostring(n))
        end
        io.write(') ')

        for i=0,n[num_keys]-1 do
            if istable(n[childs][i]) then
                io.write(type(n[childs][i]))
            else
                io.write(tostring(n[childs][i]))
            end
            io.write(' ')
            io.write(tostring(n[keys][i]))
            io.write(' ')
        end

        if not n[is_leaf] then
            for i=0, n[num_keys] do
                enqueue(n[childs][i])
            end
            if istable(n[childs][n[num_keys]]) then
                io.write(type(n[childs][n[num_keys]]))
            else
                io.write(tostring(n[childs][n[num_keys]]))
            end
        else
            if istable(n[childs][order - 1]) then
                io.write(type(n[childs][order - 1]))
            else
                io.write(tostring(n[childs][order - 1]))
            end
        end
        io.write(' | ')
    end
    io.write('\n')
end

-- Traces the path from the root to a leaf, searching by key.  Displays information about the path.
-- Returns the leaf containing the given key.
function find_leaf(root, key)
    local pos = 0
    local c = root
    if c == nil then
        print("Empty tree")
        return c
    end

    while not c[is_leaf] do
        io.write('[')
        for i=0,c[num_keys]-1 do
            io.write(tostring(c[keys][i]))
            io.write(' ')
            pos = i
        end
        io.write(tostring(c[keys][pos]))
        io.write('] ')

        pos = 0
        while pos < c[num_keys] do
            if key >= c[keys][pos] then
                pos = pos + 1
            else
                break
            end
        end

        io.write(tostring(pos))
        io.write(' ->\n')
        c = c[childs][pos]
    end

    io.write('Leaf [')
    for i=0, c[num_keys]-2 do
        io.write(tostring(c[keys][i]))
        io.write(' ')
        pos = i
    end
    io.write(tostring(c[keys][pos]))
    io.write("] ->\n")

    return c
end

-- Finds keys and their pointers, if present, in the range specified by key_start and key_end, inclusive.  Places these in the arrays
-- returned_keys and returned_pointers, and returns the number of entries found.
function find_range(root, key_start, key_end, returned_keys, returned_childs)
    local i = 0
    local num_found
    local n = find_leaf(root, key_start)
    if n == nil then return 0 end

    while i < n[num_keys] and n[keys][i] <= key_start do i = i + 1 end
    if i == n[num_keys] then return 0 end

    while n ~= nil do
        while i < n[num_keys] and n[keys][i] <= key_end do
            returned_keys[num_found] = n[keys][i]
            returned_childs[num_found] = n[childs][i]
            num_found = num_found + 1
            i = i + 1
        end
        n = n[childs][order-1]
        i = 0
    end
    return num_found
end

-- Finds and returns the record to which a key refers.
function find(root, key)
    local pos = 0
    local c = find_leaf(root, key)
    if c == nil then return nil end
    for i=0, c[num_keys]-1 do
        print(c[keys][i], " ", i)
        if c[keys][i] == key then
            pos = i
            break
        end
    end
    if pos == c[num_keys] then
        return nil
    else
        return c[childs][pos]
    end
end

-- Finds the record under a given key and prints an appropriate message to stdout.
function find_and_print(root, key)
    local r = find(root, key)
    if r == nil then
        io.write("Record not found under key ")
        io.write(key)
        io.write("\n")
    else
        io.write("Record key ")
        io.write(key)
        io.write(", value ")
        io.write(tostring(r))
        io.write('\n')
    end
end

-- Finds and prints the keys, pointers, and values within a range of keys between key_start and key_end, including both bounds.
function find_and_print_range(root, key_start, key_end)
    local i
    local returned_keys
    local returned_childs
    local num_found = find_range(root, key_start, key_end, returned_keys, returned_childs)

    if num_found == 0 then
        print('None found')
    else
        for i=0, num_found - 1 do
            io.write("Key ")
            io.write(tostring(returned_keys[i]))
            io.write(" value ")
            io.write(type(returned_childs[i]))
            io.write("\n")
        end
    end
end

-- Finds the appropriate place to split a node that is too big into two.
function cut(l)
    return math.ceil(l/2)
end

-- INSERTION
-- Helper function used in insert_into_parent to find the index of the parent's pointer to the node to the left
-- of the key to be inserted.
function get_left_index(p, left)
    local left_index = 0
    while left_index <= p[num_keys] and p[childs][left_index] ~= left do
        left_index = left_index + 1
    end
    return left_index
end

-- Inserts a new pointer to a record and its corresponding key into a leaf. Returns the altered leaf.
function insert_into_leaf(leaf, key, child)
    local insertion_point = 0

    while insertion_point < leaf[num_keys] and leaf[keys][insertion_point] < key do
        insertion_point = insertion_point + 1
    end

    local i = leaf[num_keys]
    while i > insertion_point do
        -- [WIP]
        i = i - 1
    end
end

-- DEBUG part
a = {}
init_node(a)
print(type(a), 'a')
b = {}
init_node(b)
print(type(b), 'b')
c = {}
init_node(c)
print(type(c), 'c')
d = {}
init_node(d)
print(type(d), 'd')

enqueue(a)
enqueue(b)
enqueue(c)
enqueue(d)

print(type(a[next]))

print_leaves(a)

a[keys][0] = 2.5
a[childs][0] = b
a[childs][1] = c
a[is_leaf] = false
a[num_keys] = 1
b[parent] = a
b[keys][0] = 1
b[childs][0] = 'b1'
b[keys][1] = 2
b[childs][1] = 'b2'
b[num_keys] = 2
b[childs][2] = c
c[parent] = a
c[keys][0] = 3
c[childs][0] = 'c1'
c[keys][1] = 4
c[childs][1] = 'c2'
c[num_keys] = 2

print(height(a))
print(path_to_root(a, b))

print_tree(a)
print("")

find_and_print(a, 1)

--print(cut(4))
--print(cut(5))



