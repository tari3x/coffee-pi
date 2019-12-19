open Core
open Async

module H = Hx711

let empty_threshold = 100.
let full_threshold = 400.
let full_too_long_interval = sec 1800.
let empty_confirmation_interval = sec 180.
let buzz_interval = sec 60.
let poll_interval = sec 1.

let first_full = ref None
let last_non_empty = ref None
let last_buzz = ref None

(* CR: buzz *)
let buzz () =
  assert false

let buzz () =
  let should_buzz =
    match !last_buzz with
    | None -> true
    | Some buzz -> Time.(now () > add buzz buzz_interval)
  in
  if should_buzz
  then begin
    last_buzz := Some (Time.now ());
    buzz ()
  end

(**
   SUCCESS =
   (last_non_empty < now - empty_confirmation_timeout)
   || (first_full > now - empty_timeout)

   If confirmed empty then we clear first_full.
*)
let rec measure_loop () =
  let full () =
    match !first_full with
    | None -> first_full := Some (Time.now ())
    | Some _ -> ()
  in
  let non_empty () =
    last_non_empty := Some (Time.now ())
  in
  let measure () =
    let reading = H.get_reading () in
    if reading > full_threshold  then full ();
    if reading > empty_threshold then non_empty ()
  in
  let check () =
    let confirmed_empty () =
      match !last_non_empty with
      | None -> true
      | Some last_non_empty ->
        Time.(now () > add last_non_empty empty_confirmation_interval)
    in
    let full_too_long () =
      match !first_full with
      | None -> false
      | Some first_full ->
        Time.(now () > add first_full full_too_long_interval)
    in
    if confirmed_empty ()
    then first_full := None
    else if full_too_long ()
    then buzz ()
  in
  let%bind () = Clock.after poll_interval in
  measure ();
  check ();
  measure_loop ()

let main () =
  print_s [%message "starting"];
  H.init
    ~zero_reading:0
    ~probe_weight:100.
    ~probe_reading:100;
  measure_loop ()

let command =
  let param =
    let open Command.Let_syntax in
    let%map () = return () in
    fun () ->
      main ()
  in
  Command.async
    ~readme:(fun () -> "")
    ~summary:""
    param

;;
Command.run command

