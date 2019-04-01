print ("Hello World!")
i = 1
f1 = 2.0
f2 = float(2)
f3 = 2.1
s1 = "a\tb"
s2 = 'a\tb'

print ("i = %s" % i)
print ("f1 = %d" % f1)
print ("f1 = %.2f" % f1)
print ("f2 = %f, f3 = %f" % (f2, f3))
print ("s1 = %s, s2 = %s" % (s1, s2))

mylist = []
mylist.append (1)
mylist.append (2)
mylist.append (3)
i = 0
for x in mylist:
    print ("mylist[%d] = %d" % (i, x))
    i += 1
