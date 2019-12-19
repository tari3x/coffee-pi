open Ctypes
open Foreign

let init = foreign "initHX711" (int @-> double @-> returning void)
let get_reading = foreign "getReading" (void @-> returning double)
