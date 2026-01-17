#include <format>
#include <cassert>
#include <iostream>
#include <stdexcept>

#include <json.h>
#include <curl/curl.h>

struct json_data_t
{
    float return_val;
    std::string_view to_currency;
    std::string date;

    bool found_ = false;
    bool date_found_ = false;
};

static std::size_t json_write_callback(void* contents, std::size_t size,
                                       std::size_t bytes, void* user_data)
{
    std::string* buffer = reinterpret_cast<std::string*>(user_data);
    std::size_t bytes_written = size * bytes;

    buffer->append(static_cast<const char*>(contents), bytes_written);

    return bytes_written;
}

static std::string fetch_currency_exchange_rates(const std::string_view base_currency)
{
    // Thanks to github.com/fawazahmed0/exchange-api for the forex information.
    std::string url = std::format("https://cdn.jsdelivr.net/npm/@fawazahmed0/currency-api@latest/v1/currencies/{}.json", base_currency);

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

int json_parser_cb(void* user_data, int type, const char* data, unsigned int length)
{
    json_data_t* json_data = static_cast<json_data_t*>(user_data);
    if (json_data->found_)
    {
        json_data->found_ = false;
        json_data->return_val = std::stof(data);

        return 1;
    }

    if (json_data->date_found_)
    {
        json_data->date_found_ = false;
        json_data->date = data;
    }

    if ((data != nullptr) && (type == JSON_KEY))
    {
        json_data->date_found_ = (data == std::string_view("date"));
        json_data->found_ = (data == json_data->to_currency);
    }

    return 0;
}

json_data_t parse_data(const std::string_view json_data,
                       const std::string_view result_currency)
{
    json_parser parser;
    json_data_t user_data = {
        .return_val = 0,
        .to_currency = result_currency,
        .date = ""
    };

    if (json_parser_init(&parser, nullptr, json_parser_cb, &user_data) != 0)
        throw std::runtime_error("Failed to initialize JSON parser.");

    int success = json_parser_string(&parser, json_data.data(), json_data.size(), nullptr);
    (void)success;      // Don't care.

    json_parser_free(&parser);
    return user_data;
}

bool functionality()
{
    std::string json_data;

    std::string base_curr;
    std::string target_curr;

    while (true)
    {
        std::cout << "Masukkan kurs acuan: ";
        std::cin >> base_curr;

        for (char& c : base_curr)
            c = std::tolower(c);

        json_data = fetch_currency_exchange_rates(base_curr);
        if (!json_data.empty())
            break;
        else
        {
            std::cout << "Kurs acuan tidak valid!\n\n";
        }
    }

    while (true)
    {
        std::cout << "Masukkan kurs tujuan: ";
        std::cin >> target_curr;

        for (char& c : target_curr)
            c = std::tolower(c);

        if (base_curr == target_curr)
            std::cout << "Kurs tujuan dan kurs acuan sama, pilih kurs tujuan yang berbeda.";
        else
            break;
    }

    json_data_t parsed_data = parse_data(json_data, target_curr.c_str());
    float mul_factor = parsed_data.return_val;
    float curr_cnt;

    std::cout << "Masukkan jumlah yang ingin dikonversi: ";
    std::cin >> curr_cnt;

    std::cout << "Hasil: " << std::fixed << (mul_factor * curr_cnt) << " (sesuai tgl. " << parsed_data.date << ")" << '\n';

    while (true)
    {
        std::string res;

        std::cout << "Apakah ingin mencoba lagi? (Y/N)\n";
        std::cin >> res;

        for (char& c : res)
            c = std::toupper(c);

        if ((res != "Y") && (res != "N"))
            std::cout << "Input tidak valid.";
        else
            return (res == "Y");
    }
}

int main()
{
    std::cout << "=====[Kalkulator Kurs Valuta Asing]=====\n";
    while (functionality());
}
