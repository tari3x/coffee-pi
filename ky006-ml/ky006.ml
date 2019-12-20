open Ctypes
open Foreign

(*
  https://github.com/ocamllabs/ocaml-ctypes/issues/167
  It says you could also add -E to linker command.
*)
external _force_link_ : unit -> unit = "initKY006"

let init = foreign "initKY006" (void @-> returning void)
let buzz = foreign "buzz" (void @-> returning void)
