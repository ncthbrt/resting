type httpVerb =
  | Get
  | Post
  | Patch
  | Put
  | Delete
  | Head
  | Options;

type auth = {
  user: option(string),
  password: option(string)
};

type environmentVariables = list((string, string));

type httpResponseCode =
  | Ok
  | Created;

type expect = {
  body: option(string),
  responseCode: option(httpResponseCode)
};

type assertions = string;

type header = (string, string);

type testCase = {
  description: string,
  method: httpVerb,
  url: string,
  auth: option(auth),
  headers: list(header),
  expect: option(expect),
  assertions: option(assertions),
  preRequest: option(string),
  postRequest: option(string)
};

type testSuite = {
  testCases: list(testCase),
  environmentVariables
};

exception UnknownHttpMethod(string);

module Decode = {

  let dictToArray: Js.Dict.t(string) => list((string, string)) =
    (dict) => dict |> Js.Dict.entries |> Array.to_list;         

  let method = (json) => json |> Json.Decode.string |> String.uppercase |> x => switch(x) {
    | "GET" => Get
    | "POST" => Post        
    | "PATCH" => Patch
    | "PUT" => Put
    | "DELETE" => Delete
    | "HEAD" => Head
    | "OPTIONS" => Options
    | method => raise(UnknownHttpMethod(method))
  };

  let header: Js.Json.t => header = (json) =>
      json |> Json.Decode.dict(Json.Decode.string) |> Js.Dict.entries |> Array.to_list |> List.hd;
  

  let headers: Js.Json.t => list(header) =
    (json) => json |> Json.Decode.list(header);

  let testCase = (json) =>
    Json.Decode.{
      description: json |> field("description", string),
      method: json |> field("method", method),
      url: json |> field("url", string),
      headers: json |> field("headers", headers),
      auth: None,
      expect: None,
      assertions: json |> optional(field("assertions", string)),
      preRequest: json |> optional(field("pre-request", string)),
      postRequest: json |> optional(field("post-request", string))
    };

  let environmentVariables: Js.Json.t => list((string, string)) =
    (json) => json |> Json.Decode.dict(Json.Decode.string) |> dictToArray;
  let testCases = (json) => json |> Json.Decode.list(testCase);

};

type yamlSafeLoad = (string, unit) => Js.Json.t;

[@bs.module "js-yaml"] external safeLoad : yamlSafeLoad = "safeLoad";

let file = Node.Fs.readFileAsUtf8Sync("testfile.yaml");

let result = safeLoad(file, ()) |> Decode.testCases;

Js.log(List.hd(result).description);