#!/usr/bin/env python

class Type(type):
    def __repr__(cls):
        return cls.__name__

class A(object, metaclass=Type):
    def meth1(self):
        print("A::meth1")
    def meth2(self):
        print("A::meth2")
    def meth3(self):
        print("A::meth3")

class B(A):
    def meth2(self):
        print("B::meth2")

class C(A):
    def meth1(self):
        print("C::meth1")
        super().meth1()

class D(A):
    def meth2(self):
        print("D::meth2")

class E(B): pass
class F(C): pass
class G(C): pass
class H(C): pass
class I(C): pass
class J(D): pass
class K(E): pass
class L(F,I): pass
class M(I,J): pass

class N(A,E,L,M): pass

print(N.mro())

k = M()
k.meth2()
f = N()
f.meth1()
