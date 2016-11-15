#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>
#include <google/protobuf/message.h>

#include <skullcpp/api.h>
#include "skull_protos.h"
#include "config.h"

#include "EPHandler.h"
#include "HttpRequest.h"
#include "HttpResponseImp.h"

// ====================== Service Init/Release =================================
/**
 * Service Initialization. It will be called before module initialization.
 */
static
void skull_service_init(skullcpp::Service& service, const skull_config_t* config)
{
    SKULLCPP_LOG_INFO("svc.http", "Skull service initializing");

    // Load skull_config to skullcpp::Config
    skullcpp::Config::instance().load(config);
}

/**
 * Service Release
 */
static
void skull_service_release(skullcpp::Service& service)
{
    SKULLCPP_LOG_INFO("svc.http", "Skull service releasd");
}

/**************************** Service APIs Calls *******************************
 *
 * Service API implementation. For the api which has read access, you can call
 *  `service.get()` to fetch the service data. For the api which has write
 *  access, you can also call the `service.set()` to store your service data.
 *
 * @note  DO NOT carry over the service data to another place, the only safe
 *         place for it is leaving it inside the service api scope
 *        API message name is end with '_req' for the request, with '_resp'
 *         for the response.
 */
static
void _http_query(const skullcpp::Service& service,
                 const google::protobuf::Message& request,
                 google::protobuf::Message& response) {
    SKULLCPP_LOG_DEBUG("Callout http query");

    const auto& query  = (skull::service::http::query_req&)request;

    // Extract query items and construct http request
    HttpRequest httpRequest;
    bool success = httpRequest.parse(query);
    if (!success) {
        auto& resp = (skull::service::http::query_resp&)response;
        resp.set_code(1);
        return;
    }


    // Send http request out
    const std::string httpRequestContent = httpRequest.getHttpContent();
    bool isHttps       = query.has_ishttps() ? query.ishttps() : false;
    int  port          = query.has_port()    ? query.port()    : isHttps ? 443 : 80;
    int  timeout       = query.has_timeout() ? query.timeout() : 0;
    std::string hostIp = query.has_host()    ? query.host()    : "";

    EPHandler handler;
    skullcpp::EPClient::Status st = handler.send(service, hostIp, port, timeout, httpRequestContent);
    SKULLCPP_LOG_DEBUG("ep status: " << st);
}

/**************************** Register Service ********************************/
// Register Read APIs Here
static skullcpp::ServiceReadApi api_read_tbl[] = {
    /**
     * Format: {API_Name, API_Entry_Function}, e.g. {"get", skull_service_get}
     */
    {"query", _http_query},
    {NULL, NULL}
};

// Register Write APIs Here
static skullcpp::ServiceWriteApi api_write_tbl[] = {
    /**
     * Format: {API_Name, API_Entry_Function}, e.g. {"set", skull_service_set}
     */
    {NULL, NULL}
};

// Register Service Entries
static skullcpp::ServiceEntry service_entry = {
    skull_service_init,
    skull_service_release,
    api_read_tbl,
    api_write_tbl
};

SKULLCPP_SERVICE_REGISTER(&service_entry)
