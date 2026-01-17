import requests
import json

def fetch_current_currency_exchange_rates(base_currency):
    url = f'https://cdn.jsdelivr.net/npm/@fawazahmed0/currency-api@latest/v1/currencies/{base_currency}.json'
    response = requests.get(url)

    if response.status_code != 200:
        return ''

    return response.text

def main():
    json_data = ''

    base_curr = ''
    target_curr = ''

    while True:
        base_curr = input('Masukkan kurs acuan: ')
        base_curr = base_curr.lower()

        json_data = fetch_current_currency_exchange_rates(base_curr)
        if not json_data:
            print('Kurs acuan tidak valid!\n\n')
        else:
            break

    while True:
        target_curr = input('Masukkan kurs tujuan: ')
        target_curr = target_curr.lower()

        if base_curr == target_curr:
            print("Kurs tujuan dan kurs acuan sama, pilih kurs tujuan yang berbeda.")
        else:
            break

    json_data_base_dict = json.loads(json_data)
    json_data_dict = json_data_base_dict[base_curr]

    if target_curr not in json_data_dict:
        return False

    mult_factor = json_data_dict[target_curr]
    date = json_data_base_dict['date']
    curr_cnt = 0.0

    while True:
        curr_cnt = input('Masukkan jumlah yang ingin dikonversi: ')
        if curr_cnt.isnumeric():
            break
        else:
            print('Jumlah yang dikonversi bukan angka.')

    print(f'Hasil: {mult_factor * int(curr_cnt)} (sesuai tgl. {date})')

    while True:
        res = input('Apakah ingin mencoba lagi? (Y/N)\n')
        res = res.upper()

        if ((res != 'Y') and (res != 'N')):
            print('Input tidak valid.')
        else:
            return (res == 'Y')

if __name__ == "__main__":
    print('=====[Kalkulator Kurs Valuta Asing]=====')
    while main():
        pass
