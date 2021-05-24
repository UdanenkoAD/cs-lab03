#include <iostream>
#include <vector>
#include "histogram.h"
#include "svg.h"
#include <curl/curl.h>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>


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
read_input(istream& in, bool prompt, bool flag, int n) {
    Input data;

    if (prompt)
        cerr << "Enter number count: ";
    size_t number_count;
    if (flag)
        in >> number_count;
    else
        number_count=n;

    if (prompt)
        cerr << "Enter numbers: ";
    if (flag)
        data.numbers = input_numbers(in, number_count);
    else
    {
            int A = 0;
            int B = 100;
            vector<double> result(n);
            for (int i=0; i<n; i++){
                result[i]= A + rand() % ((B + 1) - A);
            }
            data.numbers=result;
    }

    if (prompt)
        cerr << "Enter column count: ";
    size_t bin_count;
    in >> data.bin_count;

    return data;
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


size_t
write_data(void* items, size_t item_size, size_t item_count, void* ctx) {
    auto data_size=item_size*item_count;
    stringstream* buffer = reinterpret_cast<stringstream*>(ctx);
    buffer->write(reinterpret_cast<char*>(items), data_size);
    return data_size;
}


Input
download(const string& address,bool flag, int n) {
    stringstream buffer;
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();
       if(curl) {
            CURLcode res;
            curl_easy_setopt(curl, CURLOPT_URL,  address.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            res = curl_easy_perform(curl);
            if (res)
            {
                cerr << curl_easy_strerror(res) << endl;
                exit(1);
            }
        }
        curl_easy_cleanup(curl);
    return read_input(buffer, false, flag,n);
}


int
main(int argc, char* argv[]){
   srand(time(NULL));
   bool flag=true;
   int n=0;
   Input input;
    if (argc > 1) {
        if (strcmp(argv[1], "-generate") == 0 || strcmp(argv[2], "-generate" ) == 0){
            if (strcmp(argv[1], "-generate") == 0){
                n=atoi(argv[2]);
                if (n==0){
                    cerr << "Enter the number of generated numbers after ' -generate' ";
                    return 0;
                }
                flag=false;
                input = download(argv[3],flag,n);}
            if (strcmp(argv[2], "-generate") == 0){
                n=atoi(argv[3]);
                if (n==0){
                    cerr << "Enter the number of generated numbers after ' -generate' ";
                    return 0;
                }
                flag=false;
                input = download(argv[1],flag,n);}
        }

        else
            input = download(argv[1],flag,n);
    }
     else {
        input = read_input(cin, true, true,n);
    }

    // Обработка данных
     const auto bins = make_histogram(input);

    // Вывод данных
    show_histogram_svg(bins);
    return 0;
}
