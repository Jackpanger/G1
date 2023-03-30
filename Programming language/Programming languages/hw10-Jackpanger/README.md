# Homework 10 (20 Points)


The deadline for Homework 10 is Monday, Dec 5 at 10pm. The late
submission deadline is Friday, Dec 9 at 10pm.

The purpose of this homework assignment is to develop a better
understanding of class inheritance, subtyping, and dynamic dispatch.

## Getting the code template

Before you perform the next steps, you first need to create your own
private copy of this git repository. To do so, click on the link
provided in the announcement of this homework assignment on
Brightspace. After clicking on the link, you will receive an email from
GitHub, when your copy of the repository is ready. It will be
available at
`https://github.com/nyu-pl-fa22/hw10-<YOUR-GITHUB-USERNAME>`.
Note that this may take a few minutes.

* Open a browser at
  `https://github.com/nyu-pl-fa22/hw10-<YOUR-GITHUB-USERNAME>` with
  your Github username inserted at the appropriate place in the URL.
* Choose a place on your computer for your homework assignments to reside and open a terminal to that location.
* Execute the following git command: <br/>
  ```git clone https://github.com/nyu-pl-fa22/hw10-<YOUR-GITHUB-USERNAME>.git```<br/>
  ```cd hw10```

The code template for solving the exercises is provided in the file

```
src/main/scala/pl/hw10/hw10.scala
```

relative to the root directory of the repository. Follow the
instructions in the notes for setting up your Scala toolchain to
import the project into InteliJ (or use your other favorite IDE or
editor to work on the assignment).

## Submitting your solution

Put your solutions into the file `solution.md`. Once you have
completed the assignment, you can submit your solution by pushing the
modified code template to GitHub. This can be done by opening a
terminal in the project's root directory and executing the following
commands:

```bash
git add .
git commit -m "solution"
git push
```

You can replace "solution" by a more meaningful commit message.

Refresh your browser window pointing at
```
https://github.com/nyu-pl-fa22/hw10-<YOUR-GITHUB-USERNAME>/
```
and double-check that your solution has been uploaded correctly.

You can resubmit an updated solution anytime by reexecuting the above
git commands. Though, please remember the rules for submitting
solutions after the homework deadline has passed.


## Problem 1: Dynamic Dispatch (10 Points)

Consider the classes `pl.hw10.A` and `pl.hw10.B` in the code template.

1. What are the static and dynamic types of `a1` on line 37? Explain.

1. What are the static and dynamic types of `a2` on line 39? Explain.

1. What are the static and dynamic types of `a3` on line 43? Explain.

1. What method is the call `a1.m1("Hello")` dispatched to on line 39? Explain.

1. What method is the call `a2.m2()` dispatched to on line 41? Explain.

1. What method is the call `a1.m3()` dispatched to on line 43?
   Explain.
   
1. What method is the call `a3.m2()` dispatched to on line 45? Explain.

## Problem 2: Data Layout and VTables (10 Points)

Consider the code of the classes `pl.hw10.Base` and `pl.hw10.Derived`.

1. Provide the list of the correctly ordered entries in the data
   layouts of each of these two classes. The entries should be listed
   in the form:
   
   ```<classname>.<fieldname>: <type>```
   
   where `<classname>` refers to the class where the corresponding
   field was most recently declared and `<type>` refers to its
   type. You may omit the data layout entry for the vpointer. **(2.5
   Points)**
      
1. Provide the list of the correctly ordered entries in the vtable of
   each of these two classes. The entries should be listed in the
   form:
   
   ```<classname>.<methodname>(<type>, ..., <type>): <type>```

   where `<classname>` refers to the name of the class where the
   corresponding method was most recently overridden.
   
   For the sake of this exercise, you may assume that `Base` only
   inherits method `toString` from its super class `AnyRef`. **(3.5
   Points)**
     
1. What is the dynamic type of the value returned by`(new
   Derived()).m2()`? Explain. **(2 Points)**

1. What value does the expression `(new Derived()).m2().toString()`
   evaluate to? Explain. What changes if we override the
   implementation of method `toString` again in class `Derived` like
   this:
   
   ```scala
   class Derived extends Base {
     ...
     
     override def toString(): String = { 
       name
     }
   
   }
   ```
   
   Again, explain your findings. Make sure that your explanations are
   consistent with your answer to part 2.3.
   **(2 Points)**
