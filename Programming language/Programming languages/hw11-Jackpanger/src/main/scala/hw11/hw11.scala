package hw11

import scala.collection.immutable.AbstractMap
import scala.language.implicitConversions

object hw11:
  sealed abstract class TreeMap[+A] extends AbstractMap[Int, A]:
    def updated[B >: A](key: Int, value: B): TreeMap[B] =
      this match
        case Leaf() => Node(key,value,Leaf(),Leaf())
        case Node (key1, value1, left, right) =>
          if key1==key then Node(key1, value, left, right)
          else if key<key1 then Node(key1,value1,left.updated(key,value),right)
          else Node(key1,value1,left,right.updated(key,value))
      end match

    def deleteMin(): ((Int, A), TreeMap[A]) =
      require(this.nonEmpty, "deleteMin on empty map")

      (this: @unchecked) match
        /* Replace the following with your own case analysis */
        case _ =>
          (this.toList.head,this.toList.drop(1).foldLeft(TreeMap.empty[A])((a,b)=>a.updated(b._1,b._2)))
      end match
    
    def removed(key: Int): TreeMap[A] = 
      this match
        case Leaf() => Leaf()
        case Node(key1, value1, left, right) =>
          if key < key1 then Node(key1, value1, left.removed(key), right)
          else if key > key1 then Node(key1, value1, left, right.removed(key))
          else
            if right==Leaf() then left
            else Node(right.deleteMin()._1._1,right.deleteMin()._1._2,left,right.deleteMin()._2)
      end match
  
    def get(key: Int): Option[A] = this match
      case Leaf() => None
      case Node(key1, value, left, right) =>
        if key == key1 then Some(value)
        else if key < key1 then left.get(key)
        else right.get(key)
    
    override def toList: List[(Int, A)] =
      this match
      case Leaf() => Nil
      case Node(key, value, left, right) =>
        left.toList ::: (key, value) :: right.toList
  
    def iterator: Iterator[(Int, A)] = toList.iterator
  
  /* Case classes implementing binary search tree ADT */
  case class Leaf() extends TreeMap[Nothing]
  case class Node[+A](key: Int, value: A, 
                      left: TreeMap[A], right: TreeMap[A]) extends TreeMap[A]

  /* Companion object of TreeMap */
  object TreeMap:
    def empty[A]: TreeMap[A] = Leaf()
    
    def apply[A](kvs: (Int, A)*): Map[Int, A] = 
      kvs.toSeq.foldLeft(empty[A]: Map[Int, A])(_ + _)

  // code snippet for Problem 1 question 2

//  def main(args: Array[String]): Unit = {
//    var a = TreeMap.empty[Any].updated(1,1)
//    var b: TreeMap[String] = a
//    var (c,d) = b.deleteMin()
//    println(c._2.charAt(3))
//  }
