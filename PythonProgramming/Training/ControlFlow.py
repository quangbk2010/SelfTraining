a = 10
b = 2
s1 = "orange"
s2 = "apple"

if a < b:
    print ("a < b")
elif a == b:
    pass
elif s1 == "orange"\
        or s2 == "peal" and (a > b or a == 1):
    print ("Complex case")
else:
    raise RuntimeError ("Unknown case")
