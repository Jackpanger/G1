Problem 1:

1. Static type: A

   Dynamic type: B

   Static type is the type that the compiler infers for that expression at compile type. So a1: A is given a type A, so this is its static type, while at runtime, B is given, which is the dynamic type.

2. Static type: A

   Dynamic type: A

   The static type is the same. At runtime, a1.m1 initialize a new A instance, so the dynamic type is A.

3. Static type: A

   Dynamic type: B

   The static type given is A. While the dynamic type of a1 is B. In line 43, a1 will use method m3 in class B.

4. It's the m1 in class A. Although the dynamic type is B, a private m1 in class A fits this situation. And it's non-virtual.

5. m2 in class A. Since the dynamic type of a2 is A, so m2 is called in class A. 

6. m3 in class A. Because only class A has method m3, so any usage from the subclass is inherited from class A.

7. m2 in class B. The dynamic type of a1 is B, and m2 is overridden in class B.


Problem 2:

1. ```scala
   Base.name: String
   Base.x: Base
   --------------------
   Derived.name:String
   Derived.z:Int
   ```

2. ```
   Base.toString():String
   Base.m1(AnyRef):AnyRef
   Base.m2():Base
   --------------------
   Derived.toString():String
   Derived.m1(AnyRef):Derived
   Derived.m2:Base
   Derived.m4(Derived):String
   ```

3. Derived. As executing `m2()`, we return x, whose static type is Base. Since this is assigned to x, this here is a Derived instance, so the dynamic type is Derived.

4. Value: `Base.m2(Base)` and `Base`. Executing m2(), we get `println("Base.m2(Base)")` and execute `x.toString`. As `toString()`is inherited from Base, hence, we get the name in class Base, i.e., "Base".

   After changes, we get the value: `Base.m2(Base)` and `Derived`. In this case, we can print the name in class Derived.
