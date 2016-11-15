# skull service httpcli
Skull Service of Http Client

## Introduction
The http client skull-service is for helping us to send http request as well as receive http response easier. We only need to fill out the `idl/http-query_req.proto` from skull-module, then extract the `idl/http-query_resp.proto` when we receive the http response.

## How to Install
Clone this into `$skull_project_top/src/services`, then go to its folder, run:
```console
git submodule update --init --recursive
skull service --import $service_name
```

That's it :)
