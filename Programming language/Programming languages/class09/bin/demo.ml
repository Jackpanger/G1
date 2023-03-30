module type QueueType =
  sig
    type 'a queue
    val empty : 'a queue
    val is_empty : 'a queue -> bool
    val enq : 'a -> 'a queue -> 'a queue
    val deq : 'a queue -> ('a * 'a queue) option
  end

module Queue : QueueType =
  struct
    type 'a queue = 'a list * 'a list

    let empty = [], []

    let is_empty q = 
      q = empty 
      
    let enq x q = 
      match q with
      | enqs, deqs -> x :: enqs, deqs

    let rec deq = function
      | [], [] -> None
      | enqs, x :: deqs -> Some (x, (enqs, deqs))
      | enqs, [] -> deq ([], List.rev enqs)

  end

let q = Queue.empty

let q1 = Queue.enq 1 q

let q2 = Queue.enq 2 q1

let v1, q3 = Queue.deq q2 |> Option.get

let v2, q4 = Queue.deq q3 |> Option.get

let q = Queue.(empty |> enq 1 |> enq 2)

let _ = Printf.printf "%d %d\n" v1 v2


let flip f x y = f y x
    
let q5 =
  List.fold_left
    (flip Queue.enq)
    Queue.empty
    [1; 2; 3; 4]

let v, _ = Queue.(empty |> enq 1 |> enq 2 |> deq) |> Option.get

let _ = Printf.printf "%d\n" v
    
