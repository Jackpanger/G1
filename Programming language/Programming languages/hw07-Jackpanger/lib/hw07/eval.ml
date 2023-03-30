(** {0} The evaluation function of the MiniML interpreter. *)
  
open Ast
open Util

(** Generated a type error at position [pos] for expected type [exp_typ] and actual type [act_typ]. *)
let type_error pos exp_typ act_typ =
  fail pos (Printf.sprintf "Type mismatch\n  Expected type: %s\n  Actual type: %s" exp_typ act_typ)
  
(** Extract bool from a BoolVal. Generate type error message if value is not a BoolVal *)
let bool_of_value pos = function
  | BoolVal b -> b
  | IntVal i -> type_error pos "bool" "int"
  | Closure _ -> type_error pos "bool" "function"

(** Extract int from an IntVal. Generate type error message if value is not an IntVal *)
let int_of_value pos = function
  | IntVal i -> i
  | BoolVal _ -> type_error pos "int" "bool"
  | Closure _ -> type_error pos "int" "function"

(** Extract closure components from a Closure value. Generate type error message if value is not a Closure *)
let closure_of_value pos = function
  | Closure (x, t, env) -> x, t, env
  | IntVal _ -> type_error pos "function" "int"
  | BoolVal _ -> type_error pos "function" "bool"

(** Convert a value back to a term *)
let term_of_value pos = function
  | IntVal i -> IntConst (i, pos)
  | BoolVal b -> BoolConst (b, pos)
  | Closure (x, t, _) -> Lambda (x, t, pos)
        
(** Evaluate term t to a value using call-by-value semantics *)
let eval beta (t: term) : value =
  let rec eval t = match t with
  | FunConst (Fix, pos) ->
      let f = Var ("f", pos) in
      let x = Var ("x", pos) in
      let fn =
        Lambda ("x",
                App (App (f, App (t, f, pos), pos),
                     x, pos),
                pos)
      in
      Closure ("f", fn, [])
  | FunConst (Not, pos) ->
      let x = Var ("x", pos) in
      let fn = App (t, x, pos) in
      Closure ("x", fn, [])
  | IntConst (i, _) -> IntVal i
  | BoolConst (b, _) -> BoolVal b
  | App (FunConst (Not, _), t, pos) ->
      (* TODO: implement evaluation of Boolean negation *)
      let res = eval t in
      BoolVal (not (bool_of_value pos res))
  | App (t1, t2, pos) ->
      (* Evaluate function application. The actual work is delegated to the function beta. *)
      beta t1 t2 pos
  | BinOp (bop, t1, t2, _) ->
      (* TODO: implement evaluation of binary operators.
       * Use pattern matching on bop to differentiate between the different operators.
       * Make sure to implement short-circuiting semantics for And and Or. *)
        let pos1 = position_of_term t1 in 
        let pos2 = position_of_term t2 in 
        begin
          match bop with
          | Mult ->
            IntVal ((int_of_value pos1 (eval t1)) * (int_of_value pos2 (eval t2)))
          | Div ->
            IntVal ((int_of_value pos1 (eval t1)) / (int_of_value pos2 (eval t2)))
          | Mod ->
            IntVal ((int_of_value pos1 (eval t1)) mod (int_of_value pos2 (eval t2)))
          | Plus ->
            IntVal ((int_of_value pos1 (eval t1)) + (int_of_value pos2 (eval t2)))
          | Minus ->
            IntVal ((int_of_value pos1 (eval t1)) - (int_of_value pos2 (eval t2)))
          | And ->
            if (bool_of_value pos1 (eval t1)) = false then BoolVal false 
            else BoolVal (bool_of_value pos2 (eval t2))
          | Or -> 
            if (bool_of_value pos1 (eval t1)) = true then BoolVal true 
            else BoolVal (bool_of_value pos2 (eval t2))
          | Eq ->
            BoolVal ((int_of_value pos1 (eval t1)) = (int_of_value pos2 (eval t2)))
          | Lt ->
            BoolVal ((int_of_value pos1 (eval t1)) < (int_of_value pos2 (eval t2)))
          | Gt ->
            BoolVal ((int_of_value pos1 (eval t1)) > (int_of_value pos2 (eval t2)))
          | Le ->
            BoolVal ((int_of_value pos1 (eval t1)) <= (int_of_value pos2 (eval t2)))
          | Ge ->
            BoolVal ((int_of_value pos1 (eval t1)) >= (int_of_value pos2 (eval t2)))
          | Ne ->
            BoolVal ((int_of_value pos1 (eval t1)) <> (int_of_value pos2 (eval t2)))
        end 
  | Ite (t1, t2, t3, _) ->
      (* TODO: implement evaluation of conditional expressions. *)
      let pos1 = position_of_term t1 in 
      let b = bool_of_value pos1 (eval t1) in
      if b then (eval t2) else (eval t3)
  | Lambda (x, t, _) ->
      Closure (x, t, [])
  | Var (x, pos) ->
      (* This case should never be reachable assuming that the input term is closed 
       * and substitution is implemented correctly. *)
      fail pos "unexpected free variable"
  in
  eval t

(** beta-reduction step using applicative-order (call-by-value) semantics *)
let rec beta_call_by_value (t1: term) (t2: term) (pos: pos) : value =
  (* TODO: implement beta reduction for the function application term `t1 t2`.
   * Make sure that your evaluation order yields call-by-value semantics. 
   * Hint: To evaluate t1 and/or t2, you can call 'eval' recursively using 'beta_call_by_value' as argument. *)
    let newT1 =  eval beta_call_by_value t1 in 
    let newT2 =  eval beta_call_by_value t2 in
    match newT1, newT2 with
    | Closure(x1, x2,_), IntVal x ->  eval beta_call_by_value (subst x2 x1 (IntConst (x, pos)))
    | Closure(x1, x2,_), BoolVal x ->  eval beta_call_by_value (subst x2 x1 (BoolConst (x, pos)))
    | Closure(x1, x2,_), Closure (x4,x5,_) ->  eval beta_call_by_value (subst x2 x1 (Lambda (x4, x5,pos)))
    | _ -> fail pos "unexpected free variable"

(* 
    failwith "ss" *)

(** Evaluate term t to a value using applicative-order (call-by-value) semantics *)
let eval_by_value (t: term) : value = eval beta_call_by_value t


(** beta-reduction step using normal-order (call-by-name) semantics *)
let rec beta_call_by_name  (t1: term) (t2: term) (pos: pos) : value =
  (* TODO: implement beta reduction for the function application term `t1 t2`.
   * Make sure that your evaluation order yields call-by-name semantics.
   * Hint: To evaluate t1 and/or t2, you can call 'eval' recursively using 'beta_call_by_name' as argument. *)
  match t1 with
  | Lambda (x1, x2,x3) -> eval beta_call_by_name (subst x2 x1 t2)
  | App (x1,x2,x3) -> 
    begin
    match (eval beta_call_by_name x1) with
    | Closure (x4,x5,x6) -> beta_call_by_name (subst x5 x4 x2) t2 pos
    | _ -> fail pos "unexpected free variable"
    end
  | _ -> fail pos "unexpected free variable"
    

(** Evaluate term t to a value using normal-order (call-by-name) semantics *)
let eval_by_name (t: term) : value = eval beta_call_by_name t

(** {1} Bonus part *)
    
(** Evaluate term t using value environments instead of substitutions *)
let eval_with_envs (t: term) : value =
  let rec eval (env: env) (t: term) = 
    match t with
    | FunConst (Fix, pos) ->
      let f = Var ("f", pos) in
      let x = Var ("x", pos) in
      let fn =
        Lambda ("x",
                App (App (f, App (t, f, pos), pos),
                     x, pos),
                pos)
      in
      Closure ("f", fn, env)
    | FunConst (Not, pos) ->
      let x = Var ("x", pos) in
      let fn = App (t, x, pos) in
      Closure ("x", fn, env)
      | IntConst (i, _) -> IntVal i
  | BoolConst (b, _) -> BoolVal b
  | App (FunConst (Not, _), t, pos) ->
      (* TODO: implement evaluation of Boolean negation *)
      let res = eval env t in
      BoolVal (not (bool_of_value pos res))
  | App (t1, t2, pos) ->
      (* Evaluate function application. The actual work is delegated to the function beta. *)
      begin
        match t1, t2 with
        | Var(x1,_), IntConst (x,_)  -> IntVal x
        | Lambda(x1,x2,x3), _  ->
          let res = eval env t2 in 
          begin
          match res with
          | IntVal x -> eval ((x1,(IntVal x))::env) t1
          | BoolVal x -> eval ((x1,(BoolVal x))::env) t1
          | Closure(x,_,_) as t -> eval ((x, t)::env) t1
          end
        |  _  ->
          let res1 = eval env t1 in
          let res2 = eval env t2 in
          begin
          match res1,res2 with
          | Closure(x1,x2,x3),(IntVal x) -> eval ((x1,res2)::env) x2
          | _ -> fail pos "term t must have a binding1"
          end
        (* | _ -> fail pos "term t must have a binding1"   *)
      end
  | BinOp (bop, t1, t2, _) ->
      (* TODO: implement evaluation of binary operators.
       * Use pattern matching on bop to differentiate between the different operators.
       * Make sure to implement short-circuiting semantics for And and Or. *)
        let pos1 = position_of_term t1 in 
        let pos2 = position_of_term t2 in 
        begin
          match bop with
          | Mult ->
            IntVal ((int_of_value pos1 (eval env t1)) * (int_of_value pos2 (eval env t2)))
          | Div ->
            IntVal ((int_of_value pos1 (eval env t1)) / (int_of_value pos2 (eval env t2)))
          | Mod ->
            IntVal ((int_of_value pos1 (eval env t1)) mod (int_of_value pos2 (eval env t2)))
          | Plus ->
            IntVal ((int_of_value pos1 (eval env t1)) + (int_of_value pos2 (eval env t2)))
          | Minus ->
            IntVal ((int_of_value pos1 (eval env t1)) - (int_of_value pos2 (eval env t2)))
          | And ->
            if (bool_of_value pos1 (eval env t1)) = false then BoolVal false 
            else BoolVal (bool_of_value pos2 (eval env t2))
          | Or -> 
            if (bool_of_value pos1 (eval env t1)) = true then BoolVal true 
            else BoolVal (bool_of_value pos2 (eval env t2))
          | Eq ->
            BoolVal ((int_of_value pos1 (eval env t1)) = (int_of_value pos2 (eval env t2)))
          | Lt ->
            BoolVal ((int_of_value pos1 (eval env t1)) < (int_of_value pos2 (eval env t2)))
          | Gt ->
            BoolVal ((int_of_value pos1 (eval env t1)) > (int_of_value pos2 (eval env t2)))
          | Le ->
            BoolVal ((int_of_value pos1 (eval env t1)) <= (int_of_value pos2 (eval env t2)))
          | Ge ->
            BoolVal ((int_of_value pos1 (eval env t1)) >= (int_of_value pos2 (eval env t2)))
          | Ne ->
            BoolVal ((int_of_value pos1 (eval env t1)) <> (int_of_value pos2 (eval env t2)))
        end 
  | Ite (t1, t2, t3, _) ->
      (* TODO: implement evaluation of conditional expressions. *)
      if ((eval env t1) = BoolVal true) then 
          eval env t2
      else eval env t3
  | Lambda (x, t, _) ->
        let tmpFind =  List.find_map (fun (key,value)-> if key = x then Some value else None) in 
        if  (tmpFind env = None) then 
          Closure(x, t, env)
        else 
          eval env t
  | Var (x, pos) ->
      let tmpFind =  List.find_map (fun (key,value)-> if key = x then Some value else None) in 
      let res = tmpFind env in 
      if (res != None) then
        match (Option.get res) with
        | IntVal x -> IntVal x
        | BoolVal x -> BoolVal x
        | Closure(x, y, z) -> eval env (Lambda (x,y,pos))
      else
        fail pos 
        (* (string_of_term t) *)
        "term t must have a binding"
    (* failwith "Not yet implemented"  *)
    (* TODO: replace this with your implementation *)
  in
  eval [] t
