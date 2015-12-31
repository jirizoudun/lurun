VM usage:
lurun compiled_progam arg1 arg2 ...

compiled_: File compiled in Luac5.3 compiler.
argX: Arguments depends on compiled program.


Simple B+ tree key-value storage:

Run:
./lurun key_value_storage.luac [-i inputfile] [-o outputfile]

----------------------------------------------
input file or stdin syntax:
to insert:	i key<number> value
to delete:	d key<number>
to find:	f key<number>
to print tree:	p
to quit:	q
----------------------------------------------
Example syntax:
i 1 jedna
i 2 dva
i 3 tri
i 4 ctyri
i 5 pet
i 6 sest
p
d 5
p
d 2
p
f 3
q