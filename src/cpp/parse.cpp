#include "parse.h"

#include <json.h>
#include <cassert>
#include <stdexcept>

namespace cc
{
    int json_parser_callback(void* user_data, int type, const char* data, unsigned int length)
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

    json_data_t get_result(const std::string_view json_data,
                           const std::string_view result_currency)
    {
        json_parser parser;
        json_data_t user_data = {
            .return_val = 0,
            .to_currency = result_currency,
            .date = ""
        };

        if (json_parser_init(&parser, nullptr, json_parser_callback, &user_data) != 0)
            throw std::runtime_error("Failed to initialize JSON parser.");

        int success = json_parser_string(&parser, json_data.data(), json_data.size(), nullptr);
        (void)success;      // Don't care.

        json_parser_free(&parser);
        return user_data;
    }
}
