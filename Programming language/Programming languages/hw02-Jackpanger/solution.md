## Homework 2 

### Problem 1 (4 Points)

#### Part 1

1. ```scala
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

   *Suppose this was code for a language with the declaration-order rules of Scala (names must be declared before use and the static scope of a name is the entire block in which the name is declared). At each print statement (lines 6, 11, and 15), indicate which declarations of `x` and `y` are in the static scope (i.e., provide the line numbers of the relevant declarations that the occurrence of `x` and `y` refer to).*

   **Ans:  **

   > 1. **line 6:**  
   >
   >    x: line 7
   >
   >    y: line 4
   >
   > 2. **line 11:**
   >
   >    x: line 9
   >
   >    y: line 4
   >
   > 3. **line 15:**
   >
   >    x: line 2
   >
   >    y: line 13

2. **Ans:**

   >The compile will reject it with a static semantic error. 
   >
   >When middle() is executed, as the declaration-order rules of Scala, line 9 declares a variable x.
   >
   >As mentioned in the Lecture, languages like Scala whose variables are declared below cannot be used in the statements above, hence, the compiler will reject the program with a static semantic error.

#### Part 2

1. **Ans:**

   >1. **line 6:**  
   >
   >   x: line 2
   >
   >   y: line 4
   >
   >2. **line 11:**
   >
   >   x: line 9
   >
   >   y: line 4
   >
   >3. **line 15:**
   >
   >   x: line 2
   >
   >   y: line 13

2. **Ans:**

   >2
   >1
   >1
   >2
   >2
   >3

### Problem 2 (6 Points)

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

1. **Ans:**

   >2 2 3 2

2. **Ans:**

   >2 2 3 3
   >
   >The difference is the subroutine second(). In static scoping, the global variable will not be changed, while in dynamic scoping, the change of local initialization "var x: Int"  will effect the global variable.

### Problem 3 (4 Points)

```scala
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

*Assume that each integer variable occupies 4 bytes and that the shown variable declarations are the only variable declarations contained in this code. What is the maximal amount of stack space (in bytes) that needs to be allocated at any point during the execution of this program?*

**Ans:**

>20 bytes
>
>As c,d,e will deallocate the memory, so at most 5 variables will show up at the same time. 

### Problem 4 (6 Points)

```c
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

1. *This program contains a problem related to heap memory allocation that may make the program crash. However, the program may just as likely not crash and instead consistently print the value 42. (In general, the behavior of this program is undefined according to the C++ language specification.) Explain what is going on.*

   **Ans:**

   >When deleting the Node n1 by foo(), the pointer n1 doesn't deallocate, and so does n1->data. 
   >
   >n1->data stores its address in the heap. After deallocating the memory and creating n3, n3->data will use the memory that n1->data used before.
   >
   >Since n1->data and n3->data point to the same address, so when executing n1->data=42, n3->data will change to 42 as well.
   >
   >So it would be better if recycle all of the unused pointers.

2. *What happens if the two lines are swapped?*

   **Ans:**

   >In this case, all Nodes and their variables point to different addresses. Although one of them is deallocated, no other new Nodes use the recycled memory now.
   >
   >Hence, it will print the value 3 correctly.

