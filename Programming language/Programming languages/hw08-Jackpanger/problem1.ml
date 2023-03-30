(** Your solution for Problem 1 of Homework 8 *)

type ('a,'b) either = 
  | Left of 'a 
  | Right of 'b

(** Write OCaml functions that satisfy the following polymorphic type signatures *)

(** f: 'a * 'b -> 'a *)

let f (x,y) = x

(** g: ('a -> 'b) -> ('b -> 'c) -> 'a -> 'c *)

let g x y z = y (x z) 

(** h: ('a * 'b -> 'c) -> 'a -> 'b -> 'c *)

let h x y z = x y z

(** i: ('a -> 'b -> 'c) -> 'a * 'b -> 'c *)

let i x (y,z) = x y z

(** j: ('a, 'b) either * ('a -> 'c) * ('b -> 'c) -> 'c *)

let j x y z = match x,y,z with
  | Left a, b, c ->  b a
  | Right a, b, c -> c a

(** k: ('a, 'b * 'c) either -> ('a, 'b) either * ('a, 'c) either *)

let k x = match x with
| Left a  -> Left a, Left a
| Right (b,c) -> Right b, Right c

