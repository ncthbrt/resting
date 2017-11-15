open TestSuite;

let dictToList: Js.Dict.t(string) => list((string, string)) =
  (dict) => dict |> Js.Dict.entries |> Array.to_list;

let method = (json) =>
  json
  |> Json.Decode.string
  |> String.uppercase
  |> (
    (x) =>
      switch x {
      | "GET" => Get
      | "POST" => Post
      | "PATCH" => Patch
      | "PUT" => Put
      | "DELETE" => Delete
      | "HEAD" => Head
      | "OPTIONS" => Options
      | method => raise(UnknownHttpMethod(method))
      }
  );
let auth = (json) =>
  Json.Decode.{
    user: json |> optional(field("user", string)),
    password: json |> optional(field("password", string))
  };

let statusCode = (json) => 
  json |> Json.Decode.int |> TestSuite.HttpResponseCode.create;

let expect = (json) => Json.Decode.{
    body: json |> optional(field("body", x => x)),
    statusCode: json |> optional(field("statusCode", statusCode)),
    headers: json |> field("headers",dict(string)) |> dictToList
  };

let headers: Js.Json.t => list(header) =
  (json) => json |> Json.Decode.dict(Json.Decode.string) |> dictToList;

let testCase = (json) =>
  Json.Decode.{
    description: json |> field("description", string),
    method: json |> field("method", method),
    url: json |> field("url", string),
    headers: json |> field("headers", headers),
    auth: json |> optional(field("auth", auth)),
    expect: json |> optional(field("expect", expect)),
    assertions: json |> optional(field("assertions", list(string))),
    preRequest: json |> optional(field("pre-request", string)),
    postRequest: json |> optional(field("post-request", string))
  };

let environmentVariables = (json) => json |> Json.Decode.dict(Json.Decode.string) |> dictToList;

let testCases = Json.Decode.list(testCase);

type yamlSafeLoad = (string, unit) => Js.Json.t;  

[@bs.module "js-yaml"] external safeLoad : yamlSafeLoad = "safeLoad";

let file = Node.Fs.readFileAsUtf8Sync("testfile.yaml");

let result = safeLoad(file, ()) |> testCases;

Js.log(switch(List.hd(result).expect){
  | Some(v) => v.statusCode
  | None => Some(TestSuite.HttpResponseCode.create(200))
});
