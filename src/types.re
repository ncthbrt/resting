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

type environmentVariables =
  | Map(string, string);

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
  let environmentVariables = (json) => Json.Decode.{};
  let testSuite = (json) =>
    Json.Decode.{
      testCases: json |> array("testCase", testCase) |> list,
      environmentVariables: json |> dict("environmentVariables", string) |> Map
    };
};
