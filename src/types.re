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


module Decode = {
  let method = (json) => Get;
  let headers = (json) => [];
  let testCase = (json) =>
    Json.Decode.{
      description: json |> field("description", string),
      method: json |> field("method", method),
      url: json |> field("description", string),
      headers: json |> field("headers", headers),
      auth: None,
      expect: None,
      assertions: None,
      preRequest: None,
      postRequest: None
    };
    
  let dictToArray: Js.Dict.t(string) => (list((string,string)), string) => list((string,string))  = (dict) => (prev, key: string) => [(key, Js.Dict.unsafeGet(dict,key) :> string), ...prev];
  let environmentVariables: (Js.Json.t => list((string,string))) = (json) => 
    json 
    |> Json.Decode.dict(Json.Decode.string) 
    |> (d => Js.Dict.keys(d) |> Js.Array.reduce(dictToArray(d), []));  
  
  let testCases = json => json |> Json.Decode.list(testCase);
  let testSuite = (json) =>
    Json.Decode.{
      testCases: json |> field("testCases", list(testCase)),
      environmentVariables: json |> field("environmentVariables", environmentVariables)
    };
};

type yamlSafeLoad = (string, unit) => Js.Json.t;

[@bs.module "js-yaml"] external safeLoad : yamlSafeLoad = "safeLoad";
let file = Node.Fs.readFileAsUtf8Sync("testfile.yaml");

let result = safeLoad(file, ()) |> Decode.testCases;
Js.log(List.hd(result).description);