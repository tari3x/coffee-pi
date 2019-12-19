open Core

module H = Hx711_stubs

let init ~zero_reading ~probe_weight ~probe_reading =
  let offset = -zero_reading in
  let div = Float.of_int (probe_reading - zero_reading) /. probe_weight in
  H.init offset div

let get_reading = H.get_reading
