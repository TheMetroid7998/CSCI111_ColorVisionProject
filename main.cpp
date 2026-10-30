// Author: Jaz Salazar
// See dev branch for additional documentation.
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

bool rgb_validator(string color_input, int& red, int& green, int& blue);
bool input_preflight(string& color_input, int& format_base);
string hex_to_dec(string color_input);
double contrast_calculator(int rgb_colors[2][3]);
void rgb_to_srgb(const int rgb_colors[2][3], double (&srgb_colors)[2][4]);

int main() {
    int rgb_colors[2][3] = {0};
    // Two dimensional array that can store 2 rows of 3 columns containing RGB color values
    // Allows use of a for loop to reuse the input logic with flexible pointers

    cout << "\nRGB colors must be three integers separated by whitespaces and between 0-255.\n"
                "HEX colors must be in six digit format (optionally with '#' or '0x' prefixes).\n"
                "Examples: '157 34 53', '0 0 0', '0x9D2235', '#75787b'\n";

    for (int i = 0; i < 2; i++) {
        bool valid_color_format = false;

        do {
            cout << "\nEnter color values in RGB or HEX format for Color " << i+1 << " of 2:\n";

            string color_input;
            int format_base = 0;

            getline(cin, color_input);

            if (!input_preflight(color_input,format_base)) {
                cout << "Invalid input. Try again.\n";
                cout << "Examples of valid input: '157 34 53', '0 0 0', '0x9D2235', '#75787B'\n";
                continue;
            }

            if (format_base == 16) {
                // cout << "[DEBUG] Format confirmed as HEX. Proceeding to convert HEX --> DEC.\n";
                string hex_converted = hex_to_dec(color_input);
                // cout << "[DEBUG] Converted HEX --> DEC. Proceeding to verify converted RGB format.\n";
                valid_color_format = rgb_validator(hex_converted,
                rgb_colors[i][0],
                rgb_colors[i][1],
                rgb_colors[i][2]
                );
            } else if (format_base == 10) {
                // cout << "[DEBUG] Format confirmed as DEC. Proceeding to verify RGB format.\n";
                valid_color_format = rgb_validator(color_input,
                    rgb_colors[i][0],
                    rgb_colors[i][1],
                    rgb_colors[i][2]
                    );
            } else {
                cout << "ERROR: Input format could not be determined!\n";
                // cout << "[DEBUG] The value of 'input_base' is invalid.\n";
                return -1;
            }

            if (!valid_color_format) {
                cout << "Invalid color format. Please try again.\n";
                cout << "Examples of valid input: '157 34 53', '0 0 0', '0x9D2235', '#75787B'\n";
            }
        } while (!valid_color_format);
    }

    cout << "You have selected the following colors: "
        << rgb_colors[0][0] << " " << rgb_colors[0][1] << " " << rgb_colors[0][2] << " and "
        << rgb_colors[1][0] << " " << rgb_colors[1][1] << " " << rgb_colors[1][2] << "\n";

    double contrast_score = contrast_calculator(rgb_colors);
    cout << "Contrast Ratio: " << contrast_score << ":1\n";
    // contrast score goes from 1 - 21

    if (contrast_score >= 7.0) {
        cout << "\nThese colors have a high amount of contrast.\n";
    } else if (contrast_score >= 3.0 && contrast_score < 7.0) {
        cout << "\nThese colors have a good amount of contrast.\n";
    } else { //contrast score < 3.0
        cout << "\nThese colors don't have a lot of contrast. You should pick some better colors.\n";
    }
    
    return 0;
}

bool input_preflight(string& color_input, int& format_base) {

    // remove leading spaces
    while (!color_input.empty() && isspace(color_input.front())) {
        color_input.erase(0, 1);
    }
    // remove trailing spaces
    while (!color_input.empty() && isspace(color_input.back())) {
        color_input.pop_back();
    }

    // check if string is empty
    // do this AFTER removing leading/trailing spaces in case of stupid user
    if (color_input.empty()) {
        // cout << "[DEBUG] ERROR: User input stream is empty!\nReason: User inputted no real characters.\n";
        return false;
    }

    // check if string is long enough to hold a minimum valid RGB triad or HEX string
    // HEX has to have at at least 6 chars, DEC could technically fit in 5 chars
    if (color_input.size() < 5) {
        // cout << "[DEBUG] ERROR: User input stream is too short!\nReason: User did not input a valid HEX or RGB color.";
        return false;
    }

    // remove hexadecimal prefixes if they exist
    if (!color_input.empty() && color_input.front() == '#' ) {
        color_input.erase(0, 1);
    } else if (color_input.size() > 2 && color_input[0] == '0' && (color_input[1] == 'x' || color_input[1] == 'X') ) {
        color_input.erase(0,2);
    }

    // cout << "[DEBUG] User input pre-formatting succeeded!\n";
    // cout << "[DEBUG] Current input format: " << color_input << "\n";

    // DISCLAIMER: THIS WAS MODIFIED BY AI FROM MY ORIGINAL CODE.
    // Check for potential valid HEX input based on length after trimming
    bool valid_hex = (color_input.size() == 6);
    if (valid_hex) {
        for (char c : color_input) {
            if (!isxdigit(c) ) {
                valid_hex = false;
                // cout << "[DEBUG] HEX checks failed!\nReason: At least 1 character outside 'A'-'F' and '0'-'9' was present in user input; this could be a DEC stream...\n";
                break;
            }
        }
        // cout << "[DEBUG] HEX checks finished!\n";
    }

    if (valid_hex) {
        //cout << "[DEBUG] User input verified as HEX!\n";
        // Since we have validated it as HEX, convert all alphabetical characters to uppercase
        // This makes it easier to handle the conversion from HEX to DEC later
        for (char& c : color_input) {
            c = (char)toupper(c);
        }
        format_base = 16; // already checked above. applying the base confirmation here to skip DEC if needed
        // cout << "[DEBUG] HEX conversion to uppercase succeeded!\n";
    }
    else {
        // cout << "[DEBUG] User input failed HEX checks, now checking DEC format...\n";
        for (char c : color_input) {
            if (!isdigit(c) && !isspace(c)) {
                // cout << "[DEBUG] DEC checks failed!\nReason: at least 1 character(s) were not valid.\n";
                return false;
            }
        }
        // cout << "[DEBUG] DEC checks passed!\n";
        // cout << "[DEBUG] User input verified as DEC!\n";
        format_base = 10;
        // if it's not HEX and all chars are valid (not counting syntax), then it must be DEC
    }
    return true;
}

// SOURCE DISCLAIMER: This code was adapted from https://www.geeksforgeeks.org/cpp/convert-hexadecimal-to-decimal-in-cpp/
string hex_to_dec(const string color_input) {
    string converted_output;
    //cout << "[DEBUG] Now beginning HEX conversion...\n";

    string rgb_hex[3];
    for (int c = 0; c < 6; c+=2) {
        rgb_hex[c/2] += color_input[c];
        rgb_hex[c/2] += color_input[c+1];
    }

    for (int i = 0; i <= 2; i++){
        string hex_val;
        hex_val = rgb_hex[i]; // writes the hex pair at the iteration index to hex_val

        int dec_val = 0;
        for (int j = 1, base = 1; j >= 0; j--, base += 15) {
            int digit = 0;

            // I don't really understand this part.
            // How does C++ understand the numeric equivalent of the alphabetical characters?
            if (hex_val[j] >= '0' && hex_val[j] <= '9') {
                digit = hex_val[j] - '0';
            } else if (hex_val[j] >= 'A' && hex_val[j] <= 'F') {
                digit = hex_val[j] - 'A' + 10;
                // HEX input was already converted to uppercase alphanumeric in 'preflight' btw
            }
            dec_val += digit * base;
        }

        string dec_str = to_string(dec_val);
        converted_output.append(dec_str);
        if (i != 2) { // for every loop except the final one, add a space to separate the converted triads
            converted_output.append(" ");
        }
        // cout << "[DEBUG] Converted " << i << " of 3 HEX pairs...\n";
    }
    // cout << "[DEBUG] Hexadecimal conversion succeeded!\n";
    // cout << "[DEBUG] Current input format: " << converted_output << "\n";
    return converted_output;
}

bool rgb_validator(const string color_input, int& red, int& green, int& blue) {
    // cout << "[DEBUG] Now beginning RGB color validation...\n";
    stringstream rgb(color_input);
    int r, g, b;

    // DISCLAIMER: THIS WAS SUGGESTED BY AI.
    // Require 3 valid integers, fails if at least 3 integers cannot be parsed from input stringstream
    if (!(rgb >> r >> g >> b)) {
        // cout << "[DEBUG] RGB integer variable assignment failed!\nReason: Failed to extract integers from input stream.\n";
        return false;
    }

    // Reject values outside of 0-255
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        // cout << "[DEBUG] RGB range checks failed!\nReason: At least 1 integer value(s) were outside of the 0-255 range.\n";
        return false;
    }

    // Reject extraneous input beyond 3 integers
    string extra;
    if (rgb >> extra) {
        // cout << "[DEBUG] RGB input validation failed!\nReason: More than 3 values were parsed from user input.\n";
        return false;
    }

    // Store values AFTER all preconditions pass.
    red = r;
    green = g;
    blue = b;
    // cout << "[DEBUG] RGB validation completed sucessfully!\n";
    return true;
}

double contrast_calculator(int rgb_colors[2][3]) {
    // cout << "[DEBUG] Now beginning color contrast calculation...\n";

    double srgb_colors[2][4] = {0};
    rgb_to_srgb(rgb_colors, srgb_colors);

    for (int i = 0; i < 2; i++) {
        srgb_colors[i][3] = (srgb_colors[i][0] * 0.2126) + (srgb_colors[i][1] * 0.7152) + (srgb_colors[i][2] * 0.0722);
    }
    double color_contrast = (max(srgb_colors[0][3], srgb_colors[1][3]) + 0.05) / (min(srgb_colors[0][3], srgb_colors[1][3]) + 0.05);

    return color_contrast;
}

void rgb_to_srgb(const int rgb_colors[2][3], double (&srgb_colors)[2][4]) {
    // i = the individual color entered by the user --> Color 1 or Color 2
    // j = the channel of the individual color, i --> Red (0), Green (1), or Blue (2)
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            double k = rgb_colors[i][j] / 255.0;
            if (k <= 0.03928) {
                srgb_colors[i][j] = k / 12.92;
            } else {
                srgb_colors[i][j] = pow(((k + 0.055) / 1.055), 2.4);
            }
        }
    }
}
