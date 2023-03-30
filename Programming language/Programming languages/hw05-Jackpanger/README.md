# Homework 5 (20 Points)

The deadline for Homework 5 is Wednesday, Oct 19, 10pm. The late
submission deadline is Tuesday, Oct 25, 10pm.

## Getting the code template

Before you perform the next steps, you first need to create your own
private copy of this git repository. To do so, click on the link
provided in the announcement of this homework assignment on
Brightspace. After clicking on the link, you will receive an email from
GitHub, when your copy of the repository is ready. It will be
available at
`https://github.com/nyu-pl-fa22/hw05-<YOUR-GITHUB-USERNAME>`.  
Note that this may take a few minutes.

* Open a browser at `https://github.com/nyu-pl-fa22/hw05-<YOUR-GITHUB-USERNAME>` with your Github username inserted at the appropriate place in the URL.
* Choose a place on your computer for your homework assignments to reside and open a terminal to that location.
* Execute the following git command: <br/>
  ```
  git clone https://github.com/nyu-pl-fa22/hw05-<YOUR-GITHUB-USERNAME>.git
  ```
  <br/>

### Scala template (Problem 1)

The code template for solving Problem 1 is provided in the file

```
src/main/scala/pl/hw05/hw05.scala
```

relative to the root directory of the repository. Follow the
instructions in the notes for setting up your Scala toolchain to
import the project into InteliJ (or use your other favorite IDE or
editor to work on the assignment).

There are also some unit tests in

```
src/test/scala/pl/hw05/hw05Spec.scala
```

to help you test your code for Problem 1.

### OCaml template (Problem 2)

The code template for solving Problem 2 is provided in the file

```
lib/hw05/hw05.ml
```

Simply replace all occurrences of `failwith "Not yet implemented"` by
your implementation of the corresponding function.

There are also some unit tests in

```
test/hw05_spec.ml
```

to help you test your code for Problem 2.

The file

```
main/main.ml
```

is the entry point of the executable program for your code. You can also add testing code there.

The directory structure is configured for the OCaml build tool `dune`.

To compile and run the executable program, execute the following command in the root directory of the repository:

```bash
dune exec bin/main.exe
```

You will have to install `dune` and `ounit2` for this to work. Follow the [OCaml setup instructions](https://github.com/nyu-pl-fa22/ocaml-in-class-code/#installation-build-tools-and-ides) to do this. 

To run the unit tests, simply execute
```bash
dune runtest
```

Note that the interpreter will initially fail with an error message
`"Not yet implement"` for each unit test.

To provide editing support for OCaml in your IDE, we use 
the [Merlin toolkit](https://github.com/ocaml/merlin). Assuming
you have set up an editor or IDE with Merlin, you should be able to
just open the source code files and start editing. Merlin should
automatically highlight syntax and type errors in your code and
provide other useful functionality. 

**Important**: Execute `dune exec bin/main.exe` once immediately after cloning the
repository. This is needed so that Merlin is able to resolve the
dependencies between the different source code files.

## Submitting your solution

Once you have completed the assignment, you can submit your solution
by pushing the modified code template to GitHub. This can be done by
opening a terminal in the project's root directory and executing the
following commands:

```bash
git add .
git commit -m "solution"
git push
```

You can replace "solution" by a more meaningful commit message.

Refresh your browser window pointing at
```
https://github.com/nyu-pl-fa22/hw05-<YOUR-GITHUB-USERNAME>/
```
and double-check that your solution has been uploaded correctly.

You can resubmit an updated solution anytime by reexecuting the above
git commands. Though, please remember the rules for submitting
solutions after the homework deadline has passed.


## Problem 1: Tail Recursion in Scala (8 Points)

1. Write a Scala function

   ```scala
   def binom(n: Int, k: Int): Int
   ```

   that computes the binomial coefficient of two natural numbers `n`
   and `k`. Recall that for natural numbers <img
   src="http://latex.codecogs.com/svg.latex?k\le&space;n"
   style="margin-bottom:-3px" border="0"/>, the binomial coefficient,
   written

   <img src="http://latex.codecogs.com/svg.latex?\binom{n}{k}" border="0"/> 

   can be defined recursively as follows:

   <img src="http://latex.codecogs.com/svg.latex?\binom{n}{0}=\binom{n}{n}=1\quad\text{and}\quad\binom{n}{k}=\binom{n-1}{k-1}+\binom{n-1}{k}\quad\text{for}\;{}0<k<n" border="0"/>

   Your implementation should use recursion instead of loops but still
   run in constant stack space (assuming the compiler performs
   tail-call elimination). You will need to introduce a tail-recursive
   helper function to achieve this. See also the function `facTail`
   used to define the factorial function `fac` [here](https://github.com/nyu-pl-fa22/class05/blob/master/src/main/scala/pl/class05/Fac.scala).
 
   Hints

   1. A useful trick is to consider the following multiplicative identity
      of binomial coefficients for your implementation instead of the
      additive identity given above:

      <img src="http://latex.codecogs.com/svg.latex?\binom{n}{k}=\frac{n\binom{n-1}{k-1}}{k}\quad\text{for}\;0<k<n" border="0"/>

      Though, be careful not to introduce rounding errors.
 
   1. You are allowed to introduce a helper function to implement the
      tail-recursion.
 
   1. If you are unsure about whether your helper function is
      tail-recursive, you can prepend the `def` keyword with the
      `@tailrec` annotation. This will cause the Scala compiler to
      issue a warning if it cannot perform tail-call elimination on
      your function.
      

1. The *tribonacci* numbers are a generalization of fibonacci
   numbers. The *n*-th tribonacci number, *T(n)* is defined as
   follows:
   
   * *T(n) = 0* if *n = 0* or *n = 1*
   * *T(n) = 1* if *n = 2*
   * *T(n) = T(n - 1) + T(n - 2) + T(n - 3)* if *n > 2*

   Write a Scala function 
   
   ```scala
   def trib(n: Int): Int 
   ```
   
   that computes the *n*-th tribonnaci number. Your function should
   run in constant space and in time that is linear in *n*. Use recursion instead
   of loops in your implementation, but feel free to introduce a
   tail-recursive helper function.

Put your solution into the file `src/main/scala/pl/hw05/hw05.scala`.

## Problem 2: OCaml Warm-Up (12 Points)

**Important**: In your solutions for this problem, you are not allowed
to use any of the functions provided by the `List` module in the OCaml
standard library. Moreover, in part 3, you are not allowed to use the
built-in list concatenation operator `@` of OCaml.

1. Write an OCaml function 

   ```ocaml
   unzip: ('a, 'b) list -> 'a list * 'b list
   ```
   
   that takes a list of pairs and splits it component-wise into two
   lists. Some examples:
   
   ```ocaml
   # unzip [(1, "a"); (2, "b"); (3, "c")] ;;
   - : int list * string list = ([1; 2; 3], [a; b; c])
   
   # unzip [] ;;
   -: 'a list * 'b list = ([], [])
   ```

   Your implementation does not need to be tail-recursive. **(4 Points)**

1. Write an OCaml function

   ```ocaml
   drop: int -> 'a list -> 'a list
   ```

   where `drop n xs` drops every `n`-th element from the list
   `xs`. In particular, if `n` is not strictly positive, then `drop`
   should return `xs`. Examples:
   
   ```ocaml
   # drop 3 ["a"; "b"; "c"; "d"; "e"; "f"; "g"] ;;
   - : string list = ["a"; "b"; "d"; "e"; "g"]
   
   # drop 0 [1; 2] ;;
   - : int list = [1; 2]
   
   # drop 1 [1; 2] ;;
   - : int list = []
   
   # drop -1 [1; 2] ;;
   - : int list = [1; 2]
   ```


1. Consider the implementation of the `concat` function from class,
   which takes two lists `xs` and `ys` of the same type and
   concatenates them together:
   
   ```ocaml
   let rec concat xs ys = match xs with
     | [] -> ys
     | x :: xs1 -> x :: concat xs1 ys
   ```
   
   This implementation is not tail-recursive. Write a tail-recursive
   version of `concat`. 
   
   **Hint**: use the predefined function `reverse` in your
   implementation. **(4 Points)**
   
