--
-- Created by IntelliJ IDEA.
-- User: zoudun
-- Date: 28/12/15
-- Time: 00:40
-- To change this template use File | Settings | File Templates.
--

-- table node:
descendants = 'descendants' -- table
keys = 'keys' -- table
parent = 'parent' -- table
is_leaf = 'isLeaf' -- bool
num_keys = 'numKeys' -- int
next = 'next' -- table

-- Order of the tree.
-- max order: order - 1
-- min order: ~order/2
order = 4

-- Helper for printig tree.
queue = nil

local function istable(t) return type(t) == 'table' end

print('LUA B+ tree key-value storage')

function init_node(node)
    if node == nil then
        node = {}
    end

    node[descendants] = {}
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
    local n = make_node()
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
    -- Get first descendant
    while not c[is_leaf] do
        c = c[descendants][0]
    end

    while true do
        for i=0,c[num_keys] - 1 do
            io.write(tostring(c[keys][i]))
            io.write(' <> ')
            io.write(tostring(c[descendants][i]))
            io.write("; ")
        end

        if c[descendants][order - 1] ~= nil then
            io.write('| ')
            c = c[descendants][order - 1]
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
        assert(c[descendants])
        assert(c[descendants][0])
        c = c[descendants][0]
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
        if n[parent] ~= nil and n == n[parent][descendants][0] then -- TODO checking
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
            if istable(n[descendants][i]) then
                io.write(type(n[descendants][i]))
            else
                io.write(tostring(n[descendants][i]))
            end
            io.write(' ')
            io.write(tostring(n[keys][i]))
            io.write(' ')
        end

        if not n[is_leaf] then
            for i=0, n[num_keys] do
                enqueue(n[descendants][i])
            end
            if istable(n[descendants][n[num_keys]]) then
                io.write(type(n[descendants][n[num_keys]]))
            else
                io.write(tostring(n[descendants][n[num_keys]]))
            end
        else
            if istable(n[descendants][order - 1]) then
                io.write(type(n[descendants][order - 1]))
            else
                io.write(tostring(n[descendants][order - 1]))
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
        c = c[descendants][pos]
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
function find_range(root, key_start, key_end, returned_keys, returned_descendants)
    local i = 0
    local num_found
    local n = find_leaf(root, key_start)
    if n == nil then return 0 end

    while i < n[num_keys] and n[keys][i] <= key_start do i = i + 1 end
    if i == n[num_keys] then return 0 end

    while n ~= nil do
        while i < n[num_keys] and n[keys][i] <= key_end do
            returned_keys[num_found] = n[keys][i]
            returned_descendants[num_found] = n[descendants][i]
            num_found = num_found + 1
            i = i + 1
        end
        n = n[descendants][order-1]
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
        if c[keys][i] == key then
            pos = i
            break
        end
        pos = i + 1
    end
    if pos == c[num_keys] then
        return nil
    else
        return c[descendants][pos]
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
    local returned_descendants
    local num_found = find_range(root, key_start, key_end, returned_keys, returned_descendants)

    if num_found == 0 then
        print('None found')
    else
        for i=0, num_found - 1 do
            io.write("Key ")
            io.write(tostring(returned_keys[i]))
            io.write(" value ")
            io.write(type(returned_descendants[i]))
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
    while left_index <= p[num_keys] and p[descendants][left_index] ~= left do
        left_index = left_index + 1
    end
    return left_index
end

-- Inserts a new pointer to a record and its corresponding key into a leaf. Returns the altered leaf.
function insert_into_leaf(leaf, key, descendant)
    local insertion_point = 0

    while insertion_point < leaf[num_keys] and leaf[keys][insertion_point] < key do
        insertion_point = insertion_point + 1
    end

    local i = leaf[num_keys]
    while i > insertion_point do
        leaf[keys][i] = leaf[keys][i - 1]
        leaf[descendants][i] = leaf[descendants][i-1]
        i = i - 1
    end
    leaf[keys][insertion_point] = key
    leaf[descendants][insertion_point] = descendant
    leaf[num_keys] = leaf[num_keys] + 1
    return leaf
end

-- Inserts a new key and pointer to a node into a node into which these can fit without violating
-- the B+ tree properties.
function insert_into_node(root, n, left_index, key, right)
    local i = n[num_keys]

    while i > left_index do
        n[descendants][i+1] = n[descendants][i]
        n[keys][i] = n[keys][i - 1]
        i = i - 1
    end
    n[descendants][left_index + 1] = right
    n[keys][left_index] = key
    n[num_keys] = n[num_keys] + 1
    return root
end

-- Creates a new root for two subtrees and inserts the appropriate key into the new root.
function insert_into_new_root(left, key, right)
    local root = make_node()
    root[keys][0] = key
    root[descendants][0] = left
    root[descendants][1] = right
    root[num_keys] = 1
    root[parent] = nil
    left[parent] = root
    right[parent] = root
    return root
end

-- Inserts a new node (leaf or internal node) into the B+ tree. Returns the root of the tree after insertion.
function insert_into_parent(root, left, key, right)
    local left_index
    local par

    par = left[parent]

    -- Case: new root
    if par == nil then
        return insert_into_new_root(left, key, right)
    end

    -- Case: leaf or node
    -- Find the parent's pointer to the left node.
    left_index = get_left_index(par, left)

    -- Simple case: the new key fits into the node.
    if par[num_keys] < order - 1 then
        return insert_into_node(root, par, left_index, key, right)
    end

    -- Harder case:  split a node in order to preserve the B+ tree properties.
    return insert_into_node_after_splitting(root, par, left_index, key, right)
end

-- Inserts a new key and pointer to a node into a node, causing the node's size to exceed the order,
-- and causing the node to split into two.
function insert_into_node_after_splitting(root, old_node, left_index, key, right)
    local i
    local j
    local pos
    local split
    local k_prime
    local new_node
    local child
    local temp_keys = {}
    local temp_descendants = {}

--  First create a temporary set of keys and pointers
--  to hold everything in order, including
--  the new key and pointer, inserted in their
--  correct places.
--  Then create a new node and copy half of the
--  keys and pointers to the old node and
--  the other half to the new.
    i = 0
    j = 0
    while i < old_node[num_keys] + 1 do
        if j == left_index + 1 then j = j + 1 end
        temp_descendants[j] = old_node[descendants][i]

        i = i + 1
        j = j + 1
    end

    i = 0
    j = 0
    while i < old_node[num_keys] do
        if j == left_index then j = j + 1 end
        temp_keys[j] = old_node[keys][i]

        i = i + 1
        j = j + 1
    end

    temp_descendants[left_index + 1] = right
    temp_keys[left_index] = key

--  Create the new node and copy
--  half the keys and pointers to the
--  old and half to the new.
    split = cut(order)
    new_node = make_node()
    old_node[num_keys] = 0

    pos = 0
    for i=0,split-2 do
        old_node[descendants][i] = temp_descendants[i]
        old_node[keys][i] = temp_keys[i]
        old_node[num_keys] = old_node[num_keys] + 1
        pos = i + 1
    end
    old_node[descendants][pos] = temp_descendants[pos]
    k_prime = temp_keys[split-1]

    j = 0
    for i=pos+1,order-1 do
        new_node[descendants][j] = temp_descendants[i]
        new_node[keys][j] = temp_keys[i]
        new_node[num_keys] = new_node[num_keys] + 1
        j = j + 1
        pos = i + 1
    end
    new_node[descendants][j] = temp_descendants[pos]
    new_node[parent] = old_node[parent]

    for i=0,new_node[num_keys] do
        child = new_node[descendants][i]
        child[parent] = new_node
    end

    return insert_into_parent(root, old_node, k_prime, new_node)
end

-- Inserts a new key and pointer to a new record into a leaf so as to exceed the tree's order, causing the leaf
-- to be split in half.
function insert_into_leaf_after_splitting(root, leaf, key, descendant)
    local new_leaf
    local temp_keys = {}
    local temp_descendants = {}
    local insertion_index
    local split
    local new_key
    local i
    local j

    new_leaf = make_leaf()

    insertion_index = 0
    while insertion_index < order - 1 and leaf[keys][insertion_index] < key do
        insertion_index = insertion_index + 1
    end

    j=0
    i=0
    for i=0, leaf[num_keys] - 1 do
        if j == insertion_index then j = j + 1 end
        temp_keys[j] = leaf[keys][i]
        temp_descendants[j] = leaf[descendants][i]
        j = j + 1
    end

    temp_keys[insertion_index] = key
    temp_descendants[insertion_index] = descendant

    leaf[num_keys] = 0

    split = cut(order - 1)

    for i=0, split-1 do
        leaf[descendants][i] = temp_descendants[i]
        leaf[keys][i] = temp_keys[i]
        leaf[num_keys] = leaf[num_keys] + 1
    end

    i = split
    j = 0
    while i < order do
        new_leaf[descendants][j] = temp_descendants[i]
        new_leaf[keys][j] = temp_keys[i]
        new_leaf[num_keys] = new_leaf[num_keys] + 1
        i = i + 1
        j = j + 1
    end

    new_leaf[descendants][order - 1] = leaf[descendants][order - 1]
    leaf[descendants][order-1] = new_leaf

    for i=leaf[num_keys],order - 2 do
        leaf[descendants][i] = nil
    end
    for i=new_leaf[num_keys],order - 2 do
        new_leaf[descendants][i] = nil
    end

    new_leaf[parent] = leaf[parent]
    new_key = new_leaf[keys][0]

    return insert_into_parent(root, leaf, new_key, new_leaf)
end

function start_new_tree(key, value)
    local root = make_leaf()
    root[keys][0] = key
    root[descendants][0] = value
    root[descendants][order - 1] = nil
    root[num_keys] = 1
    root[parent] = nil
    return root
end

function insert(root, key, value)
    local leaf

    -- The current implementation ignores duplicates.
    if find(root, key) ~= nil then
        return root
    end

    -- Case: the tree does not exist yet. Start a new tree.
    if root == nil then
        return start_new_tree(key, value)
    end

    -- Case: the tree already exists.
    leaf = find_leaf(root, key)

    if leaf[num_keys] < order - 1 then
        leaf = insert_into_leaf(leaf, key, value)
        return root
    end

    return insert_into_leaf_after_splitting(root, leaf, key, value)
end

-- DELETION
-- Utility function for deletion.  Retrieves the index of a node's nearest neighbor (sibling)
-- to the left if one exists.  If not (the node is the leftmost child), returns -1 to signify
-- this special case.
function get_neighbor_index(n)
    assert(n[parent] ~= nil)
    for i=0,n[parent][num_keys] do
        if n[parent][descendants][i] == n then
            return i-1
        end
    end
    assert(false)
end

function remove_entry_from_node(n, key, descendant)
    local pos = 0
    local num_descendants

    -- Remove the key and shift other keys accordingly.
    while n[keys][pos] ~= key do
        pos = pos + 1
    end
    for i=pos+1,n[num_keys]-1 do
        n[keys][i-1] = n[keys][i]
    end

    -- Remove the pointer and shift other pointers accordingly.
    -- First determine number of pointers.
    num_descendants = n[num_keys]
    if not n[is_leaf] then
        num_descendants = num_descendants + 1
    end

    pos=0
    while n[descendants][pos] ~= descendant and pos <= n[num_keys] do
        pos = pos + 1
    end
    for i=pos+1, num_descendants-1 do
        n[descendants][i-1] = n[descendants][i]
    end

    -- One key fewer
    n[num_keys] = n[num_keys] - 1

    -- Set the other pointers to NULL for tidiness.
    -- A leaf uses the last pointer to point to the next leaf.
    if n[is_leaf] then
        for i=n[num_keys], order-2 do
            n[descendants][i] = nil
        end
    else
        for i=n[num_keys] + 1, order-1 do
            n[descendants][i] = nil
        end
    end

    return n
end

function adjust_root(root)
    local new_root

    -- Case: nonempty root.
    -- Key and pointer have already been deleted,
    -- so nothing to be done.

    if root[num_keys] > 0 then
        return root
    end

    -- Case: empty root.

    -- If it has a child, promote
    -- the first (only) child
    -- as the new root.
    if not root[is_leaf] then
        new_root = root[descendants][0]
        new_root[parent] = nil
    else
        -- If it is a leaf (has no children),
        -- then the whole tree is empty.
        new_root = NULL
    end

    return new_root;
end

-- Coalesces a node that has become
-- too small after deletion
-- with a neighboring node that
-- can accept the additional entries
-- without exceeding the maximum.
function coalesce_nodes(root, n, neighbor, neighbor_index, k_prime)
    local i, j, pos, neighbor_insertion_index, n_end, tmp

    -- Swap neighbor with node if node is on the
    -- extreme left and neighbor is to its right.
    if neighbor_index == -1 then
        tmp = n
        n = neighbor
        neighbor = tmp
    end

    -- Starting point in the neighbor for copying
    -- keys and pointers from n.
    -- Recall that n and neighbor have swapped places
    -- in the special case of n being a leftmost child.
    neighbor_insertion_index = neighbor[num_keys]

    -- Case:  nonleaf node.
    -- Append k_prime and the following pointer.
    -- Append all pointers and keys from the neighbor.
    if not n[is_leaf] then
        -- DEBUG - DONT KNOW STATUS
        -- Append k_prime.
        neighbor[keys][neighbor_insertion_index] = k_prime
        neighbor[num_keys] = neighbor[num_keys] + 1


        n_end = n[num_keys]
        j=0
        i=neighbor_insertion_index+1
        while j < n_end do
            neighbor[keys][i] = n[keys][j]
            neighbor[descendants][i] = n[descendants][j]
            neighbor[num_keys] = neighbor[num_keys]+1
            n[num_keys] = n[num_keys]-1;
            j = j + 1
            i = i + 1
        end

        -- The number of pointers is always
        -- one more than the number of keys.
        neighbor[descendants][i] = n[descendants][j]

        -- All children must now point up to the same parent.
        for i=0, neighbor[num_keys] do
            tmp = neighbor[descendants][i]
            tmp[parent] = neighbor
        end


    -- In a leaf, append the keys and pointers of
    -- n to the neighbor.
    -- Set the neighbor's last pointer to point to
    -- what had been n's right neighbor.
    else
        j=0
        i=neighbor_insertion_index
        while j < n[num_keys] do
            neighbor[keys][i] = n[keys][j]
            neighbor[descendants][i] = n[descendants][j]
            neighbor[num_keys] = neighbor[num_keys] + 1
            j = j + 1
            i = i + 1
        end
        neighbor[descendants][order - 1] = n[descendants][order - 1]
    end

    root = delete_entry(root, n[parent], k_prime, n)

    return root;
end

function redistribute_nodes(root, n, neighbor, neighbor_index, k_prime_index, k_prime)
    local i, tmp

    -- Case: n has a neighbor to the left.
    -- Pull the neighbor's last key-pointer pair over
    -- from the neighbor's right end to n's left end.
    if neighbor_index ~= -1 then
        if not n[is_leaf] then
            n[descendants][n[num_keys] + 1] = n[descendants][n[num_keys]]
        end
        i = n[num_keys]
        while i > 0 do
            n[keys][i] = n[keys][i - 1]
            n[descendants][i] = n[descendants][i - 1]
            i = i - 1
        end

        if not n[is_leaf] then
            n[descendants][0] = neighbor[descendants][neighbor[num_keys]]
            tmp = n[descendants][0]
            tmp[parent] = n
            neighbor[descendants][neighbor[num_keys]] = nil
            n[keys][0] = k_prime;
            n[parent][keys][k_prime_index] = neighbor[keys][neighbor[num_keys - 1]]
        else
            n[descendants][0] = neighbor[descendants][neighbor[num_keys] - 1]
            neighbor[descendants][neighbor[num_keys] - 1] = nil
            n[keys][0] = neighbor[keys][neighbor[num_keys] - 1]
            n[parent][keys][k_prime_index] = n[keys][0]
        end
    -- Case: n is the leftmost child.
    -- Take a key-pointer pair from the neighbor to the right.
    -- Move the neighbor's leftmost key-pointer pair
    -- to n's rightmost position.
    else
        if n[is_leaf] then
            n[keys][n[num_keys]] = neighbor[keys][0]
            n[descendants][n[num_keys]] = neighbor[descendants][0]
            n[parent][keys][k_prime_index] = neighbor[keys][1]
        else
            n[keys][n[num_keys]] = k_prime
            n[descendants][n[num_keys] + 1] = neighbor[descendants][0]
            tmp = n[descendants][n[num_keys] + 1]
            tmp[parent] = n
            n[parent][keys][k_prime_index] = neighbor[keys][0]
        end
        local pos = 0
        for i=0, neighbor[num_keys] - 2 do
            neighbor[keys][i] = neighbor[keys][i + 1]
            neighbor[descendants][i] = neighbor[descendants][i + 1]
            pos = i
        end
        if not n[is_leaf] then
            neighbor[descendants][pos+1] = neighbor[descendants][pos + 2]
        end
    end

    -- n now has one more key and one more pointer;
    -- the neighbor has one fewer of each.

    n[num_keys] = n[num_keys] + 1
    neighbor[num_keys] = neighbor[num_keys] - 1

    return root;
end

-- Deletes an entry from the B+ tree. Removes the record and its key and pointer
-- from the leaf, and then makes all appropriate changes to preserve the B+ tree properties.
function delete_entry(root, n, key, descendant)
    local min_keys, neighbor, neightbor_index, k_prime_index, k_prime, capacity

    -- Remove key and descendant from node
    n = remove_entry_from_node(n, key, descendant)

    -- Case: deletion from the root
    if n == root then
        return adjust_root(root)
    end

    -- Case:  deletion from a node below the root.
    -- (Rest of function body.)

    -- Determine minimum allowable size of node,
    -- to be preserved after deletion.
    min_keys = cut(order) - 1
    if n[is_leaf] then
        min_keys = cut(order - 1)
    end

    -- Case:  node stays at or above minimum.
    -- (The simple case.)
    if n[num_keys] >= min_keys then
        return root
    end

    -- Case:  node falls below minimum.
    -- Either coalescence or redistribution
    -- is needed.

    -- Find the appropriate neighbor node with which
    -- to coalesce.
    -- Also find the key (k_prime) in the parent
    -- between the pointer to node n and the pointer
    -- to the neighbor.
    neightbor_index = get_neighbor_index(n)
    k_prime_index = neightbor_index
    if neightbor_index == -1 then
        k_prime_index = 0
    end
    k_prime = n[parent][keys][k_prime_index]

    if neightbor_index == -1 then
        neighbor = n[parent][descendants][1]
    else
        neighbor = n[parent][descendants][neightbor_index]
    end

    capacity = order - 1
    if n[is_leaf] then
        capacity = order
    end

    -- Coalescence
    if neighbor[num_keys] + n[num_keys] < capacity then
--        print("coalesce")
        return coalesce_nodes(root, n, neighbor, neightbor_index, k_prime)

    -- Redistribution
    else
--        print("redistribute")
        return redistribute_nodes(root, n, neighbor, neightbor_index, k_prime_index, k_prime)
    end
end


-- Master deletion function
function delete(root, key)
    local key_leaf, key_record
    key_record = find(root, key)
    key_leaf = find_leaf(root, key)

    if key_record ~= nil and key_leaf ~= nil then
        root = delete_entry(root, key_leaf, key, key_record)
    end
    return root
end

-- DEBUG part
rt = make_node()
l = make_node()
r = make_node()
l1 = make_leaf()
l2 = make_leaf()
r1 = make_leaf()
r2 = make_leaf()

-- root
rt[keys][0] = 7
rt[descendants][0] = l
rt[descendants][1] = r
rt[num_keys] = 1

-- left
l[keys][0] = 4
l[descendants][0] = l1
l[descendants][1] = l2
l[num_keys] = 1
l[parent] = rt

-- right
r[keys][0] = 10
r[descendants][0] = r1
r[descendants][1] = r2
r[num_keys] = 1
r[parent] = rt

-- leaves
for i=0,2 do
    l1[keys][i] = i+1
    l1[descendants][i] = tostring(-1 * (i+1))
end
l1[num_keys] = 3
l1[parent] = l

for i=0,2 do
    l2[keys][i] = i+4
    l2[descendants][i] = tostring(-1 * (i+4))
end
l2[num_keys] = 3
l2[parent] = l

for i=0,2 do
    r1[keys][i] = i+7
    r1[descendants][i] = tostring(-1 * (i+7))
end
r1[num_keys] = 3
r1[parent] = r

for i=0,2 do
    r2[keys][i] = i+10
    r2[descendants][i] = tostring(-1 * (i+10))
end
r2[num_keys] = 3
r2[parent] = r

l1[descendants][3] = l2
l2[descendants][3] = r1
r1[descendants][3] = r2
r2[descendants][3] = nil

print("Height: ", height(rt))
print("Length of path from l1 to rt: ", path_to_root(rt, l1))
print("Length of path from r2 to rt: ", path_to_root(rt, r2))
print("Length of path from l to rt: ", path_to_root(rt, l))

print("Printing whole tree:")
print_tree(rt)

print("Printing only leaves:")
print_leaves(rt)

-- DELETION
--dr = rt
--delete(dr, 1)
--print_tree(dr)
--
--delete(dr, 2)
--print_tree(dr)

print("\nINSERTION\n")

ir = nil
local num = 9

for i=0,num do
    print("insert",i+1)
    ir = insert(ir, i + 1, -1 * (i + 1))
    print(ir)
    print_tree(ir)
    print("\n####################")
end

print("\nDELETION\n")

for i=0,num do
    print("delete", i + 1)
    ir = delete(ir, i + 1)
    print_tree(ir)
    print("\n####################")
end

--for i=0,num do
--    print("delete", num - i + 1)
--    ir = delete(ir, num - i + 1)
--    print_tree(ir)
--    print("\n####################")
--end

--a = {}
--init_node(a)
--a[is_leaf] = true
--print(a, 'a')
--b = {}
--init_node(b)
--b[is_leaf] = true
--print(b, 'b')
--c = {}
--init_node(c)
--c[is_leaf] = true
--print(c, 'c')
--d = {}
--init_node(d)
--d[is_leaf] = true
--print(d, 'd')
--
--enqueue(a)
--enqueue(b)
--enqueue(c)
--enqueue(d)
--
--print(a[next])
--
--print_leaves(a)
--
--a[keys][0] = 2.5
--a[descendants][0] = b
--a[descendants][1] = c
--a[is_leaf] = false
--a[num_keys] = 1
--b[parent] = a
--b[keys][0] = 1
--b[descendants][0] = 'b1'
--b[keys][1] = 2
--b[descendants][1] = 'l2'
--b[num_keys] = 2
--b[descendants][2] = c
--c[parent] = a
--c[keys][0] = 3
--c[descendants][0] = 'c1'
--c[keys][1] = 4
--c[descendants][1] = 'c2'
--c[num_keys] = 2
--
--print(height(a))
--print(path_to_root(a, b))
--
--print_tree(a)
--print("")

--find_and_print(a, 1)

--print(cut(4))
--print(cut(5))



