/* type yamlSafeLoad = string => unit => Js.Json.t;
external safeLoad: yamlSafeLoad = "safeLoad"  [@@bs.module "js-yaml"];

let value = safeLoad ("test: 123") ();

let a: array string = 
  switch (Js.Json.decodeObject (value)) {
  | Some obj => Js.Dict.keys obj
  | None => [||]
  };

a |> Js.log;

Js.log (Js.Json.stringify (value)); */
