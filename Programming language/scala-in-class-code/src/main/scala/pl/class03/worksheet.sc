import scala.language.postfixOps

class CoVar[+T](x: T):
  def method1: T = x
  def method2[U>:T](y:U): List[U] = List(x,y)

class ContraVar[-T, U>:T](x: T):
  def method1: U = x
  def method2(y: T): List[U] = List(x,y)