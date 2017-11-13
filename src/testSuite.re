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

type header = (string, string);

type expect = {
  body: option(Js.Json.t),
  responseCode: option(httpResponseCode),
  headers: list(header)
};

type assertions = string;

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
