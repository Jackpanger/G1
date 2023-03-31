#pragma once

#include "scala.h"

using namespace scala;
using namespace java::lang;

struct __A_VT;
struct __A;
typedef __A* A;

// The data layout for class A 
struct __A {
  __A_VT *__vptr;
  
  int32_t a1;
  
  int32_t a2;

  // The constructor
  __A(int32_t);
  
  // Methods implemented by A.
  static int32_t m1(A, int32_t);

  static int32_t m2(A, int32_t);
  
  static int32_t m3(A, A);

  // The function returning the class object representing A.
  static java::lang::Class __class();
  
  // The vtable for A.
  static __A_VT __vtable;
};

// The vtable layout for A.
struct __A_VT {
  // The dynamic type.
  java::lang::Class __is_a;
  
  // The vtable entries for the methods of A objects
  int32_t (*hashCode)(A);
  bool (*equals)(A, AnyRef);
  String (*toString)(A);
  java::lang::Class (*getClass)(A);
  int32_t (*m1)(A, int32_t);
  int32_t (*m3)(A, A);
  
  __A_VT()
  : __is_a(__A::__class()),
    hashCode((int32_t(*)(A)) &__AnyRef::hashCode), // inheriting hashCode from AnyRef
    equals((bool(*)(A, AnyRef)) &__AnyRef::equals), // inheriting equals from AnyRef
    toString((String(*)(A)) &__AnyRef::toString), // inheriting toString from AnyRef
    getClass((java::lang::Class(*)(A)) &__AnyRef::getClass), // inheriting getClass from AnyRef
    m1(&__A::m1),
    m3(&__A::m3)
  {
  }
};

struct __B_VT;
struct __B;
typedef __B* B;

// The data layout for class B
struct __B {
  __B_VT *__vptr;
  
  int32_t a1; // inheriting a1 from A
  
  int32_t a2; // inheriting a2 from A

  int32_t b1;
  
  // The constructor
  __B(int32_t, int32_t);
  
  // Methods implemented by B.
  static int32_t m1(B, int32_t);
  
  // The function returning the class object representing B.
  static java::lang::Class __class();
  
  // The vtable for B.
  static __B_VT __vtable;
};

// The vtable layout for B.
struct __B_VT {
  // The dynamic type.
  java::lang::Class __is_a;
  
  // The vtable entries for the methods of B objects
  int32_t (*hashCode)(B);
  bool (*equals)(B, AnyRef);
  String (*toString)(B);
  java::lang::Class (*getClass)(B);
  int32_t (*m1)(B, int32_t);
  int32_t (*m3)(B, A);
  
  __B_VT()
  : __is_a(__A::__class()),
    hashCode((int32_t(*)(B)) &__AnyRef::hashCode), // inheriting hashCode from AnyRef via A
    equals((bool(*)(B, AnyRef)) &__AnyRef::equals), // inheriting equals from AnyRef via A
    toString((String(*)(B)) &__AnyRef::toString), // inheriting toString from AnyRef via A
    getClass((java::lang::Class(*)(B)) &__AnyRef::getClass), // inheriting getClass from AnyRef via A
    m1(&__B::m1), // overriding m1
    m3((int32_t(*)(B,A))&__A::m3) // inheriting m3 from A
  {
  }
};