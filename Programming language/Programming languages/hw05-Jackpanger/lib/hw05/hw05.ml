(** Homework 5: Problem 2 *)


(** Part 1 *)
let rec unzip (xys: ('a * 'b) list) : 'a list * 'b list =
  match xys with
  | [] -> [],[]
  | hd::tl -> let first,second = unzip tl in (fst hd::first, snd hd::second)


(** Part 2 *)
let drop (n: int) (xs: 'a list) : 'a list =
    let rec drop_helper (rm:'a list) (index:int)  :'a list =
    match rm with
    | _::_ when n<=0 -> xs
    | [] -> []
    | hd::tl when index!=n -> hd::drop_helper tl (index+1)
    | _::tl  -> drop_helper tl 1
    in drop_helper xs 1


(** Part 3 *)
let reverse (x: 'a list) : 'a list =
  let rec rev acc = function
    | [] -> acc
    | x :: xs -> rev (x :: acc) xs
  in rev [] x

let rec concat (xs: 'a list) (ys: 'a list) : 'a list =
  match reverse(xs) with
  | [] -> ys
  | hd::tl -> concat (reverse tl) (hd::ys)


