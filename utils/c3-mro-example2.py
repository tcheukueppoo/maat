#!/usr/bin/env python

class Type(type):
    def __repr__(cls):
        return cls.__name__

class A(object, metaclass=Type):
    def meth(self):
        print("A::meth")
    def meth2(self):
        print("A::meth2")

class B(A):
    def meth(self):
        print("B::meth")
        super().meth()
    def meth2(self):
        print("B::meth2")
        super().meth2()

class C(A):
    def meth(self):
        print("C::meth")
        super().meth()

class D(A):
    pass

class E(B):
    def meth(self):
        print("E::meth")
        super().meth()

    def meth2(self):
        print("E::meth2")

    def meth1(self):
        print("E::meth1")
        super().meth2()

class F(E,C,D): pass

print(F.mro())

print()

"""
Remind yourself!!

If results are different then it implies that every class
deals with its own c3-list when doing chained super calls
on a single method.
"""

e = E()
e.meth()

print()

f = F()
f.meth()


print()

"""
Remind yourself!!

If "E::meth2" isn't part of the result, then that implies
chained super calls with different names do not backtrack
to the start of the c3-list, it just continues from where
it lefts off.
"""

e.meth1()
