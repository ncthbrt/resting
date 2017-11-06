open Types;

type yamlSafeLoad = (string, unit) => Js.Json.t;

[@bs.module "js-yaml"] external safeLoad : yamlSafeLoad = "safeLoad";

let value = safeLoad("test: 123", ());

let a: array(string) =
  switch (Js.Json.decodeObject(value)) {
  | Some(obj) => Js.Dict.keys(obj)
  | None => [||]
  };

a |> Js.log;

Js.log(Js.Json.stringify(value));
