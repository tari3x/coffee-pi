open Ctypes
open Foreign

(*
  https://github.com/ocamllabs/ocaml-ctypes/issues/167
  It says you could also add -E to linker command.
*)
external _force_link_ : unit -> unit = "initHX711"

let init = foreign "initHX711" (int @-> double @-> returning void)
let get_reading = foreign "getReading" (void @-> returning double)
