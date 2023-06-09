# Homework 8 (20 Points)

The deadline for Homework 8 is Friday, Nov 18, 10pm. The late
submission deadline is Tuesday, Nov 22, 10pm.

## Getting the code template

Before you perform the next steps, you first need to create your own
private copy of this git repository. To do so, click on the link
provided in the announcement of this homework assignment on
Brightspace. After clicking on the link, you will receive an email from
GitHub, when your copy of the repository is ready. It will be
available at
`https://github.com/nyu-pl-fa22/hw08-<YOUR-GITHUB-USERNAME>`.
Note that this may take a few minutes.

* Open a browser at `https://github.com/nyu-pl-fa22/hw08-<YOUR-GITHUB-USERNAME>` with your Github username inserted at the appropriate place in the URL.
* Choose a place on your computer for your homework assignments to reside and open a terminal to that location.
* Execute the following git command: <br/>
  ```bash
  git clone https://github.com/nyu-pl-fa22/hw08-<YOUR-GITHUB-USERNAME>.git
  cd hw08
  ```


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
https://github.com/nyu-pl-fa22/hw08-<YOUR-GITHUB-USERNAME>/
```
and double-check that your solution has been uploaded correctly.

You can resubmit an updated solution anytime by reexecuting the above
git commands. Though, please remember the rules for submitting
solutions after the homework deadline has passed.

## Problem 1: Type Inhabitation (8 Points)

Write OCaml functions that satisfy the following polymorphic type signatures:

1. a function `f: 'a * 'b -> 'a`

2. a function `g: ('a -> 'b) -> ('b -> 'c) -> 'a -> 'c`

3. a function `h: ('a * 'b -> 'c) -> 'a -> 'b -> 'c`

4. a function `i: ('a -> 'b -> 'c) -> 'a * 'b -> 'c`


For the next two subproblems, assume we are additionally given the following algebraic data type:

```ocaml
type ('a,'b) either = 
  | Left of 'a 
  | Right of 'b
```

5. a function `j: ('a, 'b) either * ('a -> 'c) * ('b -> 'c) -> 'c`

6. a function `k: ('a, 'b * 'c) either -> ('a, 'b) either * ('a, 'c) either`

It does not matter what your implementations of these functions do as
long as they satisfy the above type signatures. In all cases, there is
an obvious implementation that follows from the type signature. Note
that you are not allowed to use any explicit type annotations in your
solution.

**Hint:** Here is how to approach this problem. Consider the type of the function
`g`:

`g: ('a -> 'b) -> ('b -> 'c) -> 'a -> 'c`

This type tells you that `g` is a curried function that takes three
parameters, say `x` `y` and `z`, and then returns some
result. So the basic shape of the definition of `g` must look like
this

```ocaml
let g x y z = e
```

for some appropriate expression `e` that you need to find. To figure
out what `e` is, inspect the type of `g` and what it tells you about
its parameters and return value. E.g. it tells you that `x` must be a
function of type `'a -> 'b`, `y` must be a function of type `'b -> 'c`
and `z` must be a value of type `'a`. Moreover, the return value has
type `'c`, which hence is the type of the expression `e` that you need
to find. So think about how to construct a value of type `'c` using
`x`, `y`, and `z`.

Put your solution into the file `problem1.ml`.

## Problem 2: Type Inference (12 Points)

For each of the following OCaml functions, use the type inference
algorithm presented in class to infer the type of the given function
if it exists, respectively, indicate why it does not exist. 

1. ```ocaml
   let f x y z = if x y then y else z + 1
   ```
   
2. ```ocaml
   let g x y z = if x > 0 then x + y else y z
   ```
   
3. ```ocaml
   let h x y z = z x (y x)
   ```

Your solution should include:

a. The type equality constraints derived from the body of the
   function.
   
b. The most general unifier obtained from these constraints (if it
   exists).
   
c. If a unifier of the constraints exists, give the inferred type of
   the function.
   
d. If no unifier exists, explain why there is a type error in the
   function definition.

Put your solutions into the file `problem2.md`.

**Hint:** You can use the OCaml type checker to double-check your results.
