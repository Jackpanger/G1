## Solution for Problem 2

1. ```ocaml
   let f x y z = if x y then y else z + 1
   ```

   Step 1: associate a type variable with every subexpression of `e`, and introduce two type variables for `x` one for each occurrence of `x`, ` y`, ` z` in the expression:

   ```
   x: 'a
   y: 'b
   z: 'c
   x y: 'd
   z+1: 'e
   1: 'f
   if x y then y else z + 1: 'g
   ```

   Then assign type variables to all program variables:

   ```
   x: 'x
   y: 'y
   z: 'z
   ```

   Then we generate the typing constraints from all the subexpressions.

   From `x`, `y`, and `z` we infer

   ```
   'a ='x
   'b ='y
   'c ='z
   ```

   From `z+1` , we infer:

   ```
   'c = int
   'e = int
   'f = int
   ```

   From `x y` we infer

   ```
   'a = 'b -> 'd 
   ```

   From `if x y then y else z + 1` we infer

   ```
   'd = bool
   'g = 'b
   'g = 'e
   ```

    In summary, we have the constraints:

   ```
   'a ='x
   'b ='y
   'c ='z
   'c = int
   'e = int
   'f = int
   'a = 'b -> 'd 
   'd = bool
   'g = 'b
   'g = 'e
   ```

   Now we solve the constraints. Processing all of the constraints is fine and produces the candidate solution

   σ = {`a ↦ 'int -> 'bool`,`'b ↦ int`,`'c ↦int`, `'e ↦ int`, `'x ↦ 'int -> 'bool `, `'g ↦ int`, `'y ↦ int `, `'z ↦ int`, `'d ↦ bool`, `f ↦ 'int`}

   The definition we know

   ```
   f: 'a->'b->'c->'g
   ```

   After applying σ to this type we get:

   ```
   f: ('int -> 'bool)->int->int->int
   ```

2. ```ocaml
   let g x y z = if x > 0 then x + y else y z
   ```

   Step 1: associate a type variable with every subexpression of `e`, and introduce two type variables for `x` one for each occurrence of `x`, ` y`, ` z` in the expression:

   ```
   x: 'a
   y: 'b
   z: 'c
   x>0: 'd
   x+y: 'e
   y z: 'f
   if x > 0 then x + y else y z: 'g
   ```

   Then assign type variables to all program variables:

   ```
   x: 'x
   y: 'y
   z: 'z
   ```

   Then we generate the typing constraints from all the subexpressions.

   From `x`, `y`, and `z` we infer

   ```
   'a ='x
   'b ='y
   'c ='z
   ```

   From `x+y` , we infer:

   ```
   'a = int
   'b = int
   'e = int
   ```

   From `y z` we infer

   ```
   'b = 'c -> 'f
   ```

   From `if x > 0 then x + y else y z` we infer

   ```
   'd = bool
   'g = 'f
   'g = 'e
   ```

    In summary, we have the constraints:

   ```
   'a ='x
   'b ='y
   'c ='z
   'a = int
   'b = int
   'e = int
   'b = 'c -> 'f
   'd = bool
   'g = 'f
   'g = 'e
   ```

   Now we solve the constraints. Processing the first 6 constraints is fine and produces the candidate solution

   σ = {`a ↦ 'int`,`'b ↦ int`,`'c ↦'z`, `'e ↦ int`, `'x ↦ 'int `, `'y ↦ int `}

   However, when we process the next constraint `'b = 'c -> 'f` and apply σ to both sides, we get

   `int ='c -> 'f `, which is unsolvable. This is a type mismatch. Hence there is a type error in the function definition.

3. ```ocaml
   let h x y z = z x (y x)
   ```

   Step 1: associate a type variable with every subexpression of `e`, and introduce two type variables for `x` one for each occurrence of `x`, ` y`, ` z` in the expression:

   ```
   x: 'a
   y: 'b
   z: 'c
   y x: 'd
   z x: 'e
   z x (y x): 'f
   ```

   Then assign type variables to all program variables:

   ```
   x: 'x
   y: 'y
   z: 'z
   ```

   Then we generate the typing constraints from all the subexpressions.

   From `x`, `y`, and `z` we infer

   ```
   'a ='x
   'b ='y
   'c ='z
   ```

   From `y x` we infer

   ```
   'b = 'a -> 'd 
   ```

   From `z x` we infer

   ```
   'c = 'a -> 'e
   ```

   From `z x (y x)` we infer

   ```
   'e = 'd -> 'f 
   ```

    In summary, we have the constraints:

   ```
   'a ='x
   'b ='y
   'c ='z
   'b = 'a -> 'd 
   'c = 'a -> 'e
   'e = 'd -> 'f 
   ```

   Now we solve the constraints. Processing all of the constraints is fine and produces the candidate solution

   σ = {`a ↦ 'x`, `b ↦ 'y`, `c ↦ 'z`,`'y ↦ 'x -> 'd `,`'z ↦ 'x -> 'e`, `'e ↦ 'd -> 'f `}

   The definition we know

   ```
   h: 'a->'b->'c->'f
   ```

   After applying σ to this type we get:

   ```
   h: 'x->('x -> 'd)->('x->'d -> 'f)->'f
   ```

   After renaming we get:

   ```
   h: 'a->('a -> 'b)->('a->'b -> 'c)->'c
   ```

   
