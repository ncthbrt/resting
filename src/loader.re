open Types;

type yamlSafeLoad = (string, unit) => Js.Json.t;

[@bs.module "js-yaml"] external safeLoad : yamlSafeLoad = "safeLoad";

let data = {| {
  "start": { "x": 1.1, "y": -0.4 },
  "end":   { "x": 5.3, "y": 3.8 }
} |};

let line = data |> Js.Json.parseExn |> Decode.line;

Printf.printf("%f\n", line.start.x);
