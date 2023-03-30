package pl.hw05

import scala.annotation.tailrec

object hw05 extends App:

  // Problem 1: Tail Recursion

  // Naive inefficient implementation of binom used for testing
  def binomNaive(n: Int, k: Int): Int =
    require (0 <= k && k <= n)
    if n == k || k == 0 then 1 
    else binomNaive(n - 1, k - 1) + binomNaive(n - 1, k)
  
  // Your implementation of Part 1
  def binom(n: Int, k: Int): Int =
    require (0 <= k && k <= n)
    @tailrec
    def binomHelper(nres: Long, kres:Long, n: Int, k: Int): Int=
      if n == k || k == 0 then  (nres/kres).toInt
      else binomHelper(n*nres,k*kres,n-1,k-1)
    binomHelper(1,1, n,k)

  // Naive inefficient implementation of trib used for testing
  def tribNaive(n: Int): Int =
    require (0 <= n)
    n match
      case 0 | 1 => 0
      case 2 => 1
      case _ => tribNaive(n - 1) + tribNaive(n - 2) + tribNaive(n - 3)
    end match

  // Your implementation of Part 2
  def trib(n: Int): Int =
    require (0 <= n)
    @tailrec
    def tribHelp(k:Int, n:Int, n1:Int,n2:Int,n3:Int):Int=
      if n==0 || n==1 then 0
      else if n==2 then 1
      else if k==n then n3
      else tribHelp(k+1,n,n2,n3,n1+n2+n3)
    tribHelp(2,n,0,0,1)

