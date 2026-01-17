#include "fetch.h"

#include <format>
#include <curl/curl.h>

namespace cc
{
    static std::size_t json_write_callback(void* contents, std::size_t size,
                                           std::size_t bytes, void* user_data)
    {
        std::string* buffer = reinterpret_cast<std::string*>(user_data);
        std::size_t bytes_written = size * bytes;

        buffer->append(static_cast<const char*>(contents), bytes_written);

        return bytes_written;
    }

    static std::string fetch_currency_exchange_rates(const std::string_view base_currency,
                                                     const std::string_view date)
    {
        // Thanks to github.com/fawazahmed0/exchange-api for the forex information.
        std::string url = std::format("https://cdn.jsdelivr.net/npm/@fawazahmed0/currency-api@{}/v1/currencies/{}.json",
                                      date, base_currency);

        CURL* curl = curl_easy_init();
        if (curl == nullptr)
            throw std::runtime_error("Failed to initialize CURL.");

        std::string json_contents;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, json_write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &json_contents);

        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Accept: application/json");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        CURLcode result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        curl_global_cleanup();

        if (result != CURLE_OK)
            return "";

        return json_contents;
    }

    std::string fetch_latest_currency_exchange_rates(const std::string_view base_currency)
    {
        return fetch_currency_exchange_rates(base_currency, "latest");
    }
}
