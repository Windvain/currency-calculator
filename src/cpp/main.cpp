#include <iostream>

#include "fetch.h"
#include "parse.h"

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

        json_data = cc::fetch_latest_currency_exchange_rates(base_curr);
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

    cc::json_data_t parsed_data = cc::get_result(json_data, target_curr.c_str());
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
