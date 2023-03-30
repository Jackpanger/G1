(** Problem 1 *)

(* unzip: ('a * 'b) list -> 'a list * 'b list *)
let unzip xys = List.fold_right (fun a b -> (fst a:: fst b, snd a::snd b)) xys ([],[])

(* fold_right: ('a -> 'b -> 'b) -> 'a list -> 'b -> 'b *)
let fold_right op xs z = List.fold_left (fun x y -> op y x) z (List.fold_left (fun x y -> y::x) [] xs)

(* in_relation: ('a -> bool) -> 'a list -> bool *)
let in_relation p xs =
    let rec rel_helper p = function
    | []|_::[] -> true
    | hd1::(hd2::tl1 as tl) ->
        if (p hd1 hd2) then rel_helper p tl else false
    in (rel_helper[@tailrec]) p xs

    
(** Problem 2 *)

(** An ADT for nested lists *)
type 'a nlist =
  | NList of ('a nlist) list
  | Atom of 'a

(* Pretty printing functions for nested lists *)
let rec pr_nlist_list pe ppf = function
  | [] -> ()
  | [xs] -> pr_nlist pe ppf xs
  | xs :: xss ->
      Format.fprintf ppf "%a;@ %a" (pr_nlist pe) xs (pr_nlist_list pe) xss 
  
and pr_nlist pe ppf = function
  | Atom x -> Format.fprintf ppf "Atom@ @[%a@]" pe x
  | NList xss -> Format.fprintf ppf "NList@[<2>@ [@[%a@]]@]" (pr_nlist_list pe) xss

(* Pretty printer for values xxs of type int nlist *)
let string_of_int_nlist xss =
  pr_nlist (fun ppf -> Format.fprintf ppf "%d") Format.str_formatter xss;
  Format.flush_str_formatter ()
        
(* to_list: 'a nlist -> 'a list *)
let to_list xss =
    let rec to_list_helper acc = function
    | NList [] -> acc
    | Atom x -> [x]
    | NList (Atom hd::tl) -> to_list_helper (hd::acc) (NList tl)
    | NList (hd::tl) -> to_list_helper ((to_list_helper [] hd)@acc) (NList tl)
    in List.rev ((to_list_helper[@tailrec]) [] xss)

(** An ADT for binary search trees *)
type tree =
  | Leaf
  | Node of int * tree * tree

(* Pretty printing functions for trees *)
let rec pr_tree ppf = function
  | Leaf -> Format.fprintf ppf "Leaf"
  | Node (x, left, right) ->
      Format.fprintf ppf "@[<2>Node@ (%d,@ %a,@ %a)@]" x pr_tree left pr_tree right

let string_of_tree t =
  pr_tree Format.str_formatter t;
  Format.flush_str_formatter ()

        
(* fold: ('a -> int -> 'a) -> 'a -> tree -> 'a *) 
let rec fold op z t = match t with
    | Leaf -> z
    | Node(k, left, right) ->
     (fold[@tailrec]) op (op (fold op z left) k) right
        
(* list_of_tree: tree -> int list *)
let list_of_tree t = List.rev (fold (fun x y -> y::x) [] t)

(* is_sorted: tree -> bool *)
let is_sorted t =
    if (fold (fun x y -> if x<y then y else max_int) min_int t) == max_int then false
    else true

  
