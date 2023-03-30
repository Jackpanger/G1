# Solution

## Answer to Problem 1

**Ans:**

>See from the codes, the value of `read_int()` is for accessing the value of the grades array, which is a temporary variable in `get_transcript_entry` function. The address of the stack starts from the high address to the low address. According to the calling sequence of the stack, local variables are pushed before the temporaries, which means local variables, i.e. `student_id` in this case, have high addresses compared to temporary variable `grades`. Therefore, if the hacker tries an input, which is beyond the boundary of `grades`, it will access the values beyond the address stored in the array, because an array `a` of type `char[]` is a pointer to the memory location of the first entry of the array. 
>
>As mentioned earlier, local variables have high addresses than temporaries. If the input is a proper value, it could access the value of `student_id` by the address. Because the address of the array goes from high to low when the index increases, the proper value should be a negative integer number so that it can access the high address. However, the output of the result is an ASCII character, it should be converted to ASCII code to see the actual ID then.

## Answer to Problem 2

1. *What does this program print with static scoping and deep binding semantics (i.e. Scala's standard semantics)? Explain.*

   **Ans: 0** **0**

   >As deep binding semantics, the reference environment of  `val p = foo(set_x, 1)` has been fixed in its closure, so `set_x(2)` does not affect the output. When executing `p()`, it actually executes `print_x` returned by the `foo(set_x,1)`. According to static scoping, it would be the x inside the `foo()` function, i.e. `var x = 0`. Same as the `print_x()` in `foo()`, still prints 0.

2. *What would this program print if Scala were to use dynamic scoping and shallow binding semantics? Explain.*

   **Ans: 1 2**

   >When executing `foo(set_x, 1)`, first `var x = 0` makes x = 0. Then `s(y)` changes the value of x to 1, so the output of `print_x()` will be 1. After the function returned, `set_x(2)` modifies the x to 2. Hence, according to shallow binding, the output of `p()` would be 2.

## Answer to Problem 3

1. both `x` and `y` are call-by-value parameters;

   **Ans: 4 1**

2. `x` is call-by-reference and `y` is call-by-value;

   **Ans: 4 4**

3. `x` is call-by-value and `y` is call-by-name;

   **Ans: 5 3**

4. `x` is call-by-reference and `y` is call-by-name;

   **Ans: 5 5**
