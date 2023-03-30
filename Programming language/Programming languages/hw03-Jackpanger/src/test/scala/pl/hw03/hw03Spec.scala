package pl.hw03

import org.scalactic.Tolerance._
import org.scalatest.flatspec.AnyFlatSpec
import hw03._

class hw03Spec extends AnyFlatSpec {

  // Tests for Problem 1

  "numToWords" should "convert a number to full words" in {
    assert(numToWords(175) === "one-seven-five")
    assert(numToWords(3000) === "three-zero-zero-zero")
    assert(numToWords(3000) === "three-zero-zero-zero")
    assert(numToWords(1) === "one")
    assert(numToWords(0) === "zero")
  }

  // Tests for Problem 2

  "squareRoot" should "compute the square root of 'c' up to error 'epsilon'" in {
    val epsilon = 0.0001

    val cs = List(0.0, 4.0, 9.0, 16.0, 25.0)

    for (c <- cs) {
      val croot = squareRoot(c, epsilon)
      assert(math.abs(croot * croot - c) < epsilon)
    }
  }

  // Tests for Problem 3

  "isSorted" should "check whether the array 'a' is strictly sorted in increasing order" in {
    assert(isStrictlySorted(Array()))
    assert(isStrictlySorted(Array(Int.MinValue, Int.MaxValue)))
    assert(isStrictlySorted(Array(-1, 1, 2, 3)))
    assert(!isStrictlySorted(Array(-1, 1, 1, 2, 3)))
    assert(!isStrictlySorted(Array(3, 2, 1)))
    assert(!isStrictlySorted(Array(1, 2, -1, 3)))
  }

  "binarySearch" should "return the smallest index of 'a' whose value is greater or equal to 'x'" in {
    assert(binarySearch(1, Array()) === 0)
    assert(binarySearch(2, Array(1, 2, 3)) === 1)
    assert(binarySearch(0, Array(1, 2, 3)) === 0)
    assert(binarySearch(2, Array(1, 3, 4)) === 1)
    assert(binarySearch(4, Array(1, 2, 3)) === 3)
  }
}