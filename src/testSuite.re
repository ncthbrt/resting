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

type httpStatusCode = 
  | HttpStatusCode(int);

exception InvalidResponseCode(int);

let createStatusCode = (code:int) => 
  code >= 200 && code <= 500
  ? HttpStatusCode(code)
  : raise(InvalidResponseCode(code));  
  
type header = (string, string);

type expect = {
  body: option(Js.Json.t),
  statusCode: option(httpStatusCode),
  headers: list(header)
};

type assertion = string;

type testCase = {
  description: string,
  method: httpVerb,
  url: string,
  auth: option(auth),
  headers: list(header),
  expect: option(expect),
  assertions: option(list(assertion)),
  preRequest: option(string),
  postRequest: option(string)
};

type testSuite = {
  testCases: list(testCase),
  environmentVariables
};

exception UnknownHttpMethod(string);
