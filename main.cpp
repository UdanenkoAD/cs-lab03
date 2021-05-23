#include <iostream>
#include <vector>
#include "histogram.h"
#include "svg.h"
#include <curl/curl.h>


using namespace std;

vector<double>
input_numbers(istream& in, size_t count) {
    vector<double> result(count);
    for (size_t i = 0; i < count; i++) {
        in >> result[i];
    }
    return result;
}



Input
read_input(istream& in, bool prompt) {
    if (prompt){
    Input data;

    cerr << "Enter number count: ";
    size_t number_count;
    in >> number_count;

    cerr << "Enter numbers: ";
    data.numbers = input_numbers(in, number_count);

    cerr << "Enter column count: ";
    size_t bin_count;
    in >> data.bin_count;

    return data;
    }
}




vector <size_t> make_histogram(struct Input data){
        double max;
        double min;
        find_minmax(data, min, max);
         vector<size_t> bins(data.bin_count);
    for (double number : data.numbers) {
        size_t bin = (size_t)((number - min) / (max - min) * data.bin_count);
        if (bin == data.bin_count) {
            bin--;
        }
        bins[bin]++;
    }
    return bins;
}


 void show_histogram_text(vector<size_t> bins){
         const size_t SCREEN_WIDTH = 80;
    const size_t MAX_ASTERISK = SCREEN_WIDTH - 4 - 1;

    size_t max_count = 0;
    for (size_t count : bins) {
        if (count > max_count) {
            max_count = count;
        }
    }
    const bool scaling_needed = max_count > MAX_ASTERISK;

    for (size_t bin : bins) {
        if (bin < 100) {
            cout << ' ';
        }
        if (bin < 10) {
            cout << ' ';
        }
        cout << bin << "|";

        size_t height = bin;
        if (scaling_needed) {
            const double scaling_factor = (double)MAX_ASTERISK / max_count;
            height = (size_t)(bin * scaling_factor);
        }

        for (size_t i = 0; i < height; i++) {
            cout << '*';
        }
        cout << '\n';
    }
       return;
}




int
main(int argc, char* argv[]){
    curl_global_init(CURL_GLOBAL_ALL);
    if (argc>1)
    {
       CURL* curl = curl_easy_init();
       if(curl) {
            CURLcode res;
            curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                cerr << curl_easy_strerror(res) << endl;
                exit(1);
            }
            curl_easy_cleanup(curl);
        }
        return 0;
    }
    // Ввод данных
     const auto input = read_input(cin,true);

    // Обработка данных
     const auto bins = make_histogram(input);

    // Вывод данных
    show_histogram_svg(bins);
    return 0;
}
