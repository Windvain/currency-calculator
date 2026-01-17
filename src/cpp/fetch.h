#pragma once

#include <string>
#include <cstdint>

namespace cc
{
    std::string fetch_latest_currency_exchange_rates(const std::string_view base_currency);
}
