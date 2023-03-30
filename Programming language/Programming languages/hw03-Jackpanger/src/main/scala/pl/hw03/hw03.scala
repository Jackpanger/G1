package pl.hw03

import scala.collection.mutable

object hw03 extends App :

  // Problem 1: Numbers to Words
  def numToWords(num: Int): String =
    require(num >= 0) // make sure that num is non-negative
    if (num == 0) return "zero"
    val result = new mutable.StringBuilder()
    var test = num
    while (test > 0)
      test % 10 match
        case 0 => result.insert(0, "zero-")
        case 1 => result.insert(0, "one-")
        case 2 => result.insert(0, "two-")
        case 3 => result.insert(0, "three-")
        case 4 => result.insert(0, "four-")
        case 5 => result.insert(0, "five-")
        case 6 => result.insert(0, "six-")
        case 7 => result.insert(0, "seven-")
        case 8 => result.insert(0, "eight-")
        case 9 => result.insert(0, "nine-")
      test = test / 10
    result.delete(result.length() - 1, result.length())
    result.toString()


  // Problem 2: Newton's Method
  def squareRoot(c: Double, epsilon: Double): Double =
    require(c >= 0) // makes sure that we don't call squareRoot with negative values
    require(epsilon >= 0) // the error bound should also be positive

    var xn = 1.0

    // Replace the ??? with your implementation of Newton's Method

    while (math.abs(xn * xn - c) >= epsilon)
      xn = xn - (xn * xn - c) / (2 * xn)

    xn // returns value of xn

  // Problem 3: Binary Search
  def isStrictlySorted(a: Array[Int]): Boolean =
    require(a != null) // make sure that 'a' is non-null
    for (i <- 0 until a.length - 1) {
      if (a(i) >= a(i + 1)) return false;
    }
    true


  def binarySearch(x: Int, a: Array[Int]): Int =
    require(a != null && isStrictlySorted(a)) // make sure that 'a' is non-null and sorted
    if(a.length==0) return 0
    var left = 0
    var right = a.length - 1
    while (left <= right) {
      var mid = (left + right) / 2
      if (a(mid) == x) {
        return mid
      } else if (a(mid) < x) {
        left = mid + 1
      } else {
        right = mid-1
      }
    }
    left
//    tmpSmall




// Add testing code here, or better yet, add them as additional unit tests to test/scala/pl/hw03/hw03Spec.scala

// Examples:
// println(squareRoot(9, 0.0001))
// println(isStrictlySorted(Array(1, 2, 3)))
// println(binarySearch(2, Array(1, 3, 4)))


