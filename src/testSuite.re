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

module HttpResponseCode : {
  type httpResponseCode = pri HttpResponseCode(int);
  exception InvalidResponseCode(int);
  let tryCreate: int => option(httpResponseCode);
  let create: int => httpResponseCode;
} = {
  exception InvalidResponseCode(int);

  type httpResponseCode = HttpResponseCode(int);

  let tryCreate = code => 
      code >=100 && code<=500
      ? Some(HttpResponseCode(code))
      : None;
  
  let create = code => 
     (code >= 100 && code<=500) 
      ? HttpResponseCode(code)
      : raise(InvalidResponseCode(code));
};

type httpStatusCode = HttpResponseCode.httpResponseCode;


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
