#include "hextodec.h"
#include <iostream>
using namespace std;

// This code was adapted from: https://www.geeksforgeeks.org/cpp/convert-hexadecimal-to-decimal-in-cpp/

string hextodec(string hex_val) {

    int dec_val = 0;

    for (int i = 1, base = 1;
         i >= 0;
         i--, base += 15) {
        int digit;

        if (hex_val[i] >= '0' && hex_val[i] <= '9') {
            digit = hex_val[i] - '0';
        } else if (hex_val[i] >= 'A' && hex_val[i] <= 'F') {
            digit = hex_val[i] - 'A' + 10;
        } else {
            //bool valid_input = false;
            continue;
        }

        dec_val += digit * base;
    }

    string dec_str = to_string(dec_val);

    return dec_str;
}
