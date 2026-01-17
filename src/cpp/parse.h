#pragma once

#include <string>

namespace cc
{
    struct json_data_t
    {
        float return_val;
        std::string_view to_currency;
        std::string date;

        bool found_ = false;
        bool date_found_ = false;
    };

    json_data_t get_result(const std::string_view json_data,
                           const std::string_view result_currency);
}
