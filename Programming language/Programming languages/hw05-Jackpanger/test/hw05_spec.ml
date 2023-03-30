open OUnit2
open Hw05
open Util.Print
    
let unzip_tests =
  [[(1, "a"); (2, "b"); (3, "c")], ([1; 2; 3], ["a"; "b"; "c"]);
   [], ([], []);
   [(1, "a")], ([1], ["a"])]

let unzip_suite =
  let pr_result = pair_to_string ilist_to_string slist_to_string in
  List.map
    (fun (arg, res) ->
      let name = " unzip " ^ (plist_to_string arg) in
      name >:: fun tc -> assert_equal ~printer:pr_result res (unzip arg))
    unzip_tests

let concat_tests =
  [[1; 2; 3], [4; 5];
   [], [];
   [], [1];
   [1], [];
   [1], [1];
   [1], [2]]

    
let concat_suite =
  List.map (fun (xs, ys) ->
    let name = " concat " ^ (ilist_to_string xs) ^ " " ^ (ilist_to_string ys) in
    name >:: fun tc -> assert_equal ~printer:ilist_to_string (xs @ ys) (concat xs ys))
    concat_tests

let drop_tests =
  [["a"; "b"; "c"; "d"; "e"; "f"; "g"], 3, ["a"; "b"; "d"; "e"; "g"];
   ["a"; "b"; "c"; "d"; "e"; "f"; "g"], 2, ["a"; "c"; "e"; "g"];
   ["a"; "b"; "c"; "d"; "e"; "f"; "g"], 1, [];
   [], 1, [];
   ["a"; "b"], 0, ["a"; "b"];
   ["a"; "b"], -1, ["a"; "b"];
   ["a"], 1, []]

let drop_suite =
  List.map (fun (xs, n, res) ->
    let name = Printf.sprintf " drop %d %s" n (slist_to_string xs) in
    name >:: fun tc -> assert_equal ~printer:slist_to_string res (drop n xs))
    drop_tests
    
let suite =   
  "Tests" >::: unzip_suite @ concat_suite @ drop_suite
    
let () = run_test_tt_main suite
