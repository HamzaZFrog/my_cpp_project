// main.cpp
#include <iostream>
#include <fmt/core.h>
#include <openssl/ssl.h>
#include <boost/algorithm/string.hpp>
#include <zlib.h>
#include <curl/curl.h>
#include <spdlog/spdlog.h>
#include <Poco/Net/HTTPClientSession.h>
#include <gtest/gtest.h>

// Function using zlib to compress a string
std::string compressString(const std::string& str) {
    uLong sourceLen = str.size();
    uLong destLen = compressBound(sourceLen);
    std::string compressed(destLen, '\0');
    compress(reinterpret_cast<Bytef*>(&compressed[0]), &destLen, reinterpret_cast<const Bytef*>(str.data()), sourceLen);
    compressed.resize(destLen);
    return compressed;
}

int main() {
    spdlog::info("Hello, Conan + Artifactory with multiple dependencies!");
    std::string example = "This is a test string for compression.";

    // Use Boost to manipulate a string
    boost::to_upper(example);
    spdlog::info("Boost transformed string: {}", example);

    // Compress string using zlib
    std::string compressed = compressString(example);
    spdlog::info("Compressed string length: {}", compressed.size());

    // Use libcurl to make a simple HTTP request (for demonstration)
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.example.com");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            spdlog::error("curl_easy_perform() failed: {}", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }

    // Basic OpenSSL usage
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        spdlog::error("Unable to create SSL context");
    } else {
        spdlog::info("SSL context created successfully");
        SSL_CTX_free(ctx);
    }

    // Use Poco to create an HTTP session
    Poco::Net::HTTPClientSession session("www.example.com");
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/");
    session.sendRequest(request);

    return 0;
}

