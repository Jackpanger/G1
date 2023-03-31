#include <iostream>

#include "scala.h"
#include "main.h"

using namespace scala;
using namespace java::lang;
using namespace std;

// Goal: We want to simulate the following Scala code
/*
class A(a: Int):
  val a1: Int = a
  private val a2: Int = 3

  def m1(x: Int) = a1 + a2 * x
  private def m2(x: Int) = a2 * x
  def m3(that: A) = m2(that.a1) + that.m1(this.a1)

class B(a: Int, b: Int) extends A(a):
  val b1: Int = b

  override def m1(x: Int) = a1 + b1 * x


object Main extends App:
  val a: A = new A(1)

  val x: Int = a.m1(3)

  println(x)

  val b: B = new B(1, 2)

  val y: Int = a.m2(b)

  println(y)
*/

// A(Int)
__A::__A(int32_t a)
   : __vptr(&__vtable),
     a1(a),
     a2(3)
{
}

// A.m1(Int): Int
int32_t __A::m1(A __this, int32_t x) {
  return __this->a1 + __this->a2 * x;
}

// A.m2(Int): Int
int32_t __A::m2(A __this, int32_t x) {
  return __this->a2 * x;
}

// A.m3(Int): Int
int32_t __A::m3(A __this, A that) {
  return m2(__this, that->a1) + that->__vptr->m1(that, __this->a1);
}

// Internal accessor for A's class.
java::lang::Class __A::__class() {
  static java::lang::Class k =
    new java::lang::__Class(stringLiteral("A"), __AnyRef::__class());
  return k;
}

// The vtable for A.  Note that this definition
// invokes the default no-arg constructor for __A_VT.
__A_VT __A::__vtable;


// Implementations of B methods

__B::__B(int32_t a, int32_t b)
   : __vptr(&__vtable),
     a1(a),
     a2(3),
     b1(b)
{
}


// B.m1(Int): Int
int32_t __B::m1(B __this, int32_t x) {
  return __this->a1 + __this->b1 * x;
}

// Internal accessor for B's class.
java::lang::Class __B::__class() {
  static java::lang::Class k =
    new java::lang::__Class(stringLiteral("B"), __B::__class());
  return k;
}

// The vtable for B.  Note that this definition
// invokes the default no-arg constructor for __B_VT.
__B_VT __B::__vtable;

// Main 

int main(void) {

  // val a: A = new A(1)
  A a = new __A(1);

  // val x = a.m1(3)
  int32_t x = a->__vptr->m1(a, 3);
  
  // println(x)
  cout << x << endl;

  // val b: B = new B(1, 2)
  B b = new __B(1, 2);

  // val y: Int = a.m3(b)
  int32_t y = a->__vptr->m3(a, (A) b);

  // println(y)
  cout << y << endl;

  return 0;
}
