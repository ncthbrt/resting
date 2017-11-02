type httpVerbs = 
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

type environmentVariables = Map(string, string);

type expect = {
    body: string
};

type assertions = string;
type header = (string, string);

type testCase = {
    description: string,
    method: httpVerbs,
    url: string,
    auth: option(auth),
    headers: list(header),
    expect: option(expect),
    assertions: option(assertions),
    preRequest: option(string),
    postRequest: option(string)
};

type testSchema = {
    testCases: list(testCase),
    environmentVariables: environmentVariables
};
