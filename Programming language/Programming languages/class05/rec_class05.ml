type ptype = TFire | TWater | TEarth;; 
type mon = {name: string; hp : int; ptype: ptype};;
let charmander = {name = "Charmander"; hp = 39; ptype = TFire}
;; 


let pokemon_battle (m1: mon) (m2: mon) : string = 
  let helper m1 m2 = 
  (match m1.ptype, m2.ptype with 
      | TFire, _ -> m1.name 
      | _, _ -> m2.name) in 
  if m1.ptype == m2.ptype 
  then (if m1.hp >= m2.hp then m1.name else m2.name)
  else (helper m1 m2)
;; 

let pokemon_battle_pair (mpair: mon * mon) : string = 
  pokemon_battle (fst mpair) (snd mpair)
;;


(* Define pokemon_battle_with_charmander version *)
let pokemon_battle_with_charmander : mon -> string = 
  pokemon_battle charmander 
;;

Printf.printf "%d\n" charmander.hp;;

(* ------------------------------------------------------------ *)

let rec fib : int -> int = 
	fun n -> 
		match n with
		| 0 -> 0
		| 1 -> 1
		| n -> fib (n-1) + fib (n-2)

let fib2 (n:int) :int = 
	let rec loop (i:int) (a:int) (b:int) :int  =
		if i = n then a
		else loop (i+1) (b) (a+b) in 
	loop 0 0 1;;

Printf.printf "Fibonacci: %d\n" (fib 4);;

let rec reverse = function
  | [] -> []
  | hd :: tl -> reverse tl @ [hd]

let reverse xs = 
  let rec reverse_helper rev_xs = function
  | hd :: tl -> reverse_helper (hd :: rev_xs) tl
  | [] -> rev_xs
  in reverse_helper [] xs

let print_list: int list -> unit  = 
	fun l ->
		let rec helper: int list -> string  = 
			fun l1 -> 
				match l1 with
				| [] -> ""
				| [hd] -> (Printf.sprintf "%d" hd)
				| hd :: tl -> (Printf.sprintf "%d;" hd)^(helper tl)
		in
		Printf.printf "[%s]\n" (helper l)
;; 

(* print_list (reverse [1;1;1;2;1;2]);; *)

