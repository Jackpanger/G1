## Homework 2 (20 Points)

The deadline for Homework 2 is Tuesday, September 20, 10pm. The late
submission deadline is Monday, September 26, 10pm.

### Getting the homework files

Before you perform the next steps, you first need to create your own
private copy of this git repository. To do so, click on the link
provided in the announcement of this homework assignment on
Brightspace. After clicking on the link, you will receive an email from
GitHub, when your copy of the repository is ready. It will be
available at
```https://github.com/nyu-pl-fa22/hw02-<YOUR-GITHUB-USERNAME>```.
Note that this may take a few minutes.

Here are some Git-related resources:
* If you are unfamiliar with Git, watch the [first two git basics video](http://git-scm.com/videos).
* If you are unfamiliar with Github, watch [this YouTube video](https://www.youtube.com/watch?v=0fKg7e37bQE).
* A [simple git cheatsheet](http://rogerdudler.github.io/git-guide/).
* A [complete reference](http://www.git-scm.com/book/en/v2).
* I suggest using the command line to interact with Git/Github, but in a pinch [this GUI](https://desktop.github.com/) might be useful.

* Open a browser at `https://github.com/nyu-pl-fa22/hw02-<YOUR-GITHUB-USERNAME>` with your Github username inserted at the appropriate place in the URL.
* Choose a place on your computer for your homework assignments to reside and open a terminal to that location.
* Execute the following git command: <br/>
  `git clone https://github.com/nyu-pl-fa22/hw02-<YOUR-GITHUB-USERNAME>.git`<br/>
  `cd hw02`


#### Submitting your solution

Put your solution into a plain text file called `solution.md` or a PDF
file called `solution.pdf`. Readable scans or photos of hand-written
solutions can be submitted as PDF.

Once you have completed the assignment, you can submit your solution
by pushing the modified code template to GitHub. This can be done by
opening a terminal in the root directory of your local copy of the
repository and executing the following commands in the terminal:

```bash
git add .
git commit -m "solution"
git push
```

You can replace "solution" by a more meaningful commit message.

Refresh your browser window pointing at
```
https://github.com/nyu-pl-fa22/hw02-<YOUR-GITHUB-USERNAME>/
```
and double-check that your solution has been uploaded correctly.

You can resubmit an updated solution anytime by reexecuting the above
git commands. Though, please remember the late submission penalty
rules.

### Problem 1 (4 Points)

Consider the following pseudo code.

```scala
 1: def outer() = {
 2:   var x: Int = 2

 3:   def middle() = {
 4:     var y: Int = x

 5:     def inner() = {
 6:       println(y); println(x)
 7:       var x: Int = 1
 8:     }
       
 9:     var x: Int = 1

10:     inner()
11:     println(x); println(y)
12:   }

13:   var y: Int = 3

14:   middle()
15:   println(x); println(y)
16: }
```

#### Part 1

1. Suppose this was code for a language with the declaration-order
   rules of Scala (names must be declared before use and the static
   scope of a name is the entire block in which the name is
   declared). At each print statement (lines 6, 11, and 15), indicate
   which declarations of `x` and `y` are in the static scope (i.e.,
   provide the line numbers of the relevant declarations that the
   occurrence of `x` and `y` refer to).

2. Will the compiler accept the program or reject it with a static
   semantic error?  If it is rejected, explain why. If it is accepted,
   what does the program print when `outer` is executed, assuming
   static scoping.

Note that every `var` declaration introduces a new local variable.

#### Part 2

Repeat Part 1 for a hypothetical language that follows the
declaration-order rules of Java. That is, names in this language must
be declared before use, and the scope of a name extends from its
declaration through the end of the block. We assume that this
hypothetical language supports nested subroutines. A nested subroutine
is a function that is defined inside another function like `inner` in
`middle`.

### Problem 2 (6 Points)

Consider the following pseudo code:

```scala
var x: Int = 0 // global variable

def set_x(n: Int) = {
  x = n
}

def print_x() = {
  println(x)
}

def first(y: Int) = {
  set_x(y)
  print_x()
}

def second () = {
  var x: Int
  first(3)
}

def main () = {
  set_x(1)
  first(2)
  print_x()
  second()
  print_x()
}
```

1. What does this program print when `main` is executed if the language uses static scoping? 

2. What does it print with dynamic scoping? Why?


### Problem 3 (4 Points)

Consider the following fragment of code in C:

```c
{
  int a, b;
  ...
  {
    int c, d;
    ...
    {
      int e;
      ...
    }
    ...
  }
  ...
  {
    int f, g, h;
    ...
  }
  ...
}
```

Assume that each integer variable occupies 4 bytes and that the shown
variable declarations are the only variable declarations contained in
this code. What is the maximal amount of stack space (in bytes) that
needs to be allocated at any point during the execution of this program?


### Problem 4 (6 Points)

Consider the following C++ program (see also `problem4.cpp`):

```c++
#include <cstdio>

struct Node {
  int data;
  Node* next;
};

void foo(Node* n) {
  delete n->next;
}

Node* create(int x, Node* n) {
  Node* n1 = new Node();
  n1->data = x;
  n1->next = n;
  
  return n1;
}

int main(void) {
  Node* n1 = create(1, NULL);
  Node* n2 = create(2, n1);
  
  foo(n2);
  Node* n3 = create(3, NULL);
  
  n1->data = 42;
  
  printf("%d\n", n3->data);
  delete n2;
  delete n3;
 
  return 0;
}
```

1. This program contains a problem related to heap memory allocation
   that may make the program crash. However, the program may just as
   likely not crash and instead consistently print the value 42. (In
   general, the behavior of this program is undefined according to the
   C++ language specification.) Explain what is going on.
   
2. What happens if the two lines

   ```c++
   foo(n2);
   Node* n3 = create(3, NULL);
   ```
   
   are swapped?

C++ primer:

* The `struct` declaration introduces a new type `Node` that describes
  compound values consisting of an `int` value and a `Node*` value,
  which can be referred to by the *fields* `data` and `next`, respectively.

* The type `Node*` stands for a pointer to a `Node` value.

* The expression `n->next` where `n` is of type `Node*` will
  dereference the pointer `n` to obtain the `Node` value that `n`
  points to and then retrieve the value stored in the `next` field
  of that `Node`.

* The expression `new Node()` will allocate a new `Node` value on the
  heap and return a pointer to that value.
  
* The expression `delete x` will delete the value pointed to by `x`
  (assuming the pointer stored in `x` was previously returned by a
  call to `new`).
  
