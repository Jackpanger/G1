package hw10

object hw10 extends App {

  // Part 1: Dynamic Dispatch
  
  class A {
    private def m1(s: String): A = {
      println("A.m1(String)")
      new A()
    }
    
    def m2(): Unit = {
      println("A.m2()")
    }
    
    def m3(): A = {
      println("A.m3()")
      this
    }

  }
  
  class B extends A {
    private def m1(s: String): B = {
      println("B.m1(String)")
      new B()
    }
    
    override def m2(): Unit = {
      println("B.m2()")
    }
  }
  
  object A {
    def main(): Unit = {
      val a1: A = new B()

      val a2: A = a1.m1("Hello")

      a2.m2()

      val a3: A = a1.m3()

      a3.m2()
    }
  }
  
  A.main()
  
  // Part 2: Data Layout and VTables
  
  class Base {
    
    private val name: String = "Base"
    val x: Base = this
    
    override def toString(): String = { 
      name
    }
  
    def m1(o: AnyRef): AnyRef = { 
      println("Base.m1(AnyRef)")
      o
    }
    
    def m2(): Base = { 
      println("Base.m2(Base)")
      x
    }
    
    private def m3(s: String): Unit = {
      println("Base.m3(String)") 
    }
  }
  
  class Derived extends Base {
    
    private val name: String = "Derived"
    val z: Int = 0


    override def m1(o: AnyRef): Derived = { 
      println("Derived.m(AnyRef)") 
      this
    }
    
    def m4(o: Derived): String = { 
      println("Derived.m4(Derived)") 
      name
    }
  }
  
  println((new Derived()).m2().toString())
}
