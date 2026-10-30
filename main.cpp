// Author: Jaz Salazar
#include <iostream>
#include <sstream>

using namespace std;

bool rgb_validator(string color_input, int& red, int& green, int& blue);

bool input_preflight(string& color_input, int& format_base);

string hex_to_dec(string color_input, string converted_output);

int score_calculator(int rgb_colors[2][3]);

int main() {
    int rgb_colors[2][3];
    // Two dimensional array that can store 2 rows of 3 columns containing RGB color values
    // HEX colors are (hopefully) automatically converted to RGB (please please please work omfg)
    // Allows use of a for loop to reuse the input logic with flexible pointers

    // TODO: Consider adding a vector or array to store original color names for user reference?

    // Using a for loop to reuse the same code for two colors.

    // move to dedicated function 'input_collector' ?
    for (int i = 0; i < 2; i++) {

        bool valid_color_format = false;

        // cout << "Introduction \n";

        do {
            cout << "RGB colors must be three integers separated by whitespaces and between 0-255.\n"
                    "HEX colors must be in six digit format (optionally with '#' or '0x' prefixes).\n"
                    "Enter colors in RGB or HEX format for Color " << i+1 << " of 2:\n";

            string color_input;
            int format_base = 0;

            getline(cin, color_input);

            if (!input_preflight(color_input,format_base)) {
                cout << "Invalid input. Try again.\n";
                cout << "Examples of valid input: '157 34 53', '0 0 0', '0x9D2235', '#75787B'\n";

                /* this is done to counter the automatic 'i++' with each loop iteration
                 * considering putting this as a while loop, but there's issues with storing an unknown number of colors
                 */
                i--;
                continue; // restarts the loop
            }

            if (format_base == 16) {
                string hex_converted = hex_to_dec(color_input, "");
                valid_color_format = rgb_validator(
                    hex_converted,
                rgb_colors[i][0],
                rgb_colors[i][1],
                rgb_colors[i][2]
                );
            } else if (format_base == 10) {
                valid_color_format = rgb_validator(
                    color_input,
                    rgb_colors[i][0],
                    rgb_colors[i][1],
                    rgb_colors[i][2]
                    );
            } else { // format_base should not be anything but 10 or 16 at this point
                cout << "ERROR: Input format could not be determined!\n"
                        "The value of 'input_base' is invalid.\n";
                return 2; // Not sure which Error code to return?
            }

            if (!valid_color_format) {
                cout << "Invalid color format. Please try again.\n";
                cout << "Examples of valid input: '157 34 53', '0 0 0', '0x9D2235', '#75787B'\n";

                /* this is done to counter the automatic 'i++' with each loop iteration
                 * considering putting this as a while loop, but there's issues with storing an unknown number of colors
                 */
                i--;
            }

        } while (!valid_color_format);
    }

    // Scaffolding, remove later.
    cout << rgb_colors[0][0] << " " << rgb_colors[0][1] << " " << rgb_colors[0][2] << endl;
    cout << rgb_colors[1][0] << " " << rgb_colors[1][1] << " " << rgb_colors[1][2] << endl;
    cout << "Well, we got this far. That's gotta be a good sign, right?";

    int color_score = score_calculator(rgb_colors);

    if (color_score >= 180) {
        cout << "These colors should be visible when placed together.\n";
    } else if (color_score >= 120 && color_score < 180) {
        cout << "These colors should be fairly visible when placed together, but you may want to tweak them.\n";
    } else { // color_score < 120
        cout << "These colors don't seem to be very distinct from each other. You should pick some better colors.\n";
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
        return false;
    }

    // check if string is long enough to hold a minimum valid RGB triad or HEX string
    // HEX has to have at at least 6 chars, DEC could technically fit in 5 chars
    // anything less just doesn't make sense.
    if (color_input.size() < 5) {
        return false;
    }

    // remove potential hexadecimal prefixes
    if (!color_input.empty() && color_input.front() == '#' ) {
        color_input.erase(0, 1);
    } else if (color_input.size() > 2 && color_input[0] == '0' && (color_input[1] == 'x' || color_input[1] == 'X') ) {
        // I don't like having to check the size here because of the above check, but just in case...
        color_input.erase(0,2);
    }

    /* THIS IS THE ORIGINAL VERSION THAT I WROTE AND DEBUGGED
     * BELOW THIS IS THE VERSION THAT CHATGPT SUGGESTED TO HANDLE MORE SPECIFIC EDGE CASES
     * I DECIDED TO ACCEPT IT ONCE I UNDERSTOOD WHY IT MADE THOSE CHANGES
     *

    if (color_input.size() == 6) {
        for (char c : color_input) {
            if (!isxdigit(c) ) {
                return false;
            }
        }
        format_base = 16;
    }
    else {
        for (char c : color_input) {
            if (!isdigit(c) && !isspace(c)) {
                return false;
            }
        }
        format_base = 10;
    }
    */

    // DISCLAIMER: THIS WAS MODIFIED BY AI. MY EXPLANATION IS ABOVE.
    /* Check for potential valid HEX input based on length after trimming
     * by this point, there are no leading/trailing whitespaces and hex prefixes have been removed
     * the ONLY valid hex input would be EXACTLY 6 chars
     */
    bool valid_hex = (color_input.size() == 6);
    if (valid_hex) {
        for (char c : color_input) {
            if (!isxdigit(c) ) {
                valid_hex = false; // CAN'T be HEX, could be weird DEC tho
                break;
            }
        }
    }

    if (valid_hex) {
        // Since we have validated it as HEX, convert all alphabetical characters to uppercase
        // This makes it easier to handle the conversion from HEX to DEC later
        for (char c : color_input) {
            color_input[c] = toupper(color_input[c], locale());
            /* this seems to be a C thing, not a C++ thing
             * the argument needs to be specified as an unsigned char, otherwise it is undefined and the implementation is implicit
             *
             * color_input[c] = static_cast<char>(toupper(static_cast<unsigned char>(color_input[c])));
             * this made it shut up but what does it MEAN
             *
             * color_input[c] = toupper(color_input[c], locale());
             * this also works and seems cleaner, just asks to define the C locale
             */
            // WARNING, Clang-Tidy: Narrowing conversion from 'int' to signed type 'value_type' (aka 'char') is implementation-defined
        }
        format_base = 16; // already checked above. applying the base confirmation here to skip DEC if needed
    }
    else {
        for (char c : color_input) {
            if (!isdigit(c) && !isspace(c)) {
                return false;
            } // CAN'T be EITHER if not valid (already checked for HEX)
        }
        format_base = 10;
        // if it's not HEX and all chars are valid (not counting syntax), then it must be DEC
    }

    return true;
}

// SOURCE DISCLAIMER: This code was adapted from https://www.geeksforgeeks.org/cpp/convert-hexadecimal-to-decimal-in-cpp/
string hex_to_dec(const string color_input, string& converted_output) {

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
            // changes the base for second char position, this SHOULD be reset with every call for rgb_hex[a]
            // if not, we have to implement it differently

            // I don't really understand this part.
            // How does C++ understand the numeric equivalent of the alphabetical characters?

            // HEX input was already converted to uppercase alphanumeric in 'preflight' btw
            if (hex_val[j] >= '0' && hex_val[j] <= '9') {
                digit = hex_val[j] - '0';
            } else if (hex_val[j] >= 'A' && hex_val[j] <= 'F') {
                digit = hex_val[j] - 'A' + 10;
            }
            dec_val += digit * base;
        }

        string dec_str = to_string(dec_val);

        converted_output.append(dec_str);
        // for every loop except the final one, add a space to separate the converted triads
        if (i != 2) {
            converted_output.append(" ");
        }
    }
    return converted_output;
}

bool rgb_validator(const string color_input, int& red, int& green, int& blue) {
    // DO NOT PASS `color_input` BY REFERENCE. THAT DEFEATS THE POINT OF MAKING IT A `const`!

    stringstream rgb(color_input);
    int r, g, b;

    /* DISCLAIMER: THIS WAS GENERATED BY AI. MY EXPLANATION / UNDERSTANDING IS BELOW.
     * stringstream rgb is extracted to r, g, and b separately, interpreting the string characters as ints
     * stops at spaces, essentially parses the integer until it finds smth that's not an int
     * anything extra is handled below
     */
    // Require 3 valid integers
    // NOTE: This does not handle integer representations such as '+20'.
    // This should have been fixed by the changes to the preflight function.
    if (!(rgb >> r >> g >> b)) {
        return false;
    }

    // Reject values outside of 0-255
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        return false;
    }

    // Reject extraneous input beyond 3 integers
    string extra;
    if (rgb >> extra) {
        return false;
    }

    // Store values AFTER all preconditions pass.
    red = r;
    green = g;
    blue = b;

    return true;
}

int score_calculator(const int rgb_colors[2][3]) {

    int r_score = (max(rgb_colors[0][0], rgb_colors[1][0]) - min(rgb_colors[0][0], rgb_colors[1][0]));
    int g_score = (max(rgb_colors[0][1], rgb_colors[1][1]) - min(rgb_colors[0][1], rgb_colors[1][1]));
    int b_score = (max(rgb_colors[0][2], rgb_colors[1][2]) - min(rgb_colors[0][2], rgb_colors[1][2]));
    // subtracts the smallest value from the largest value for each channel (independently)
    // might have problems, but allows to potentially identify weaknesses based on channel scores

    int rgb_score = (r_score + g_score + b_score) / 3;

    return rgb_score;
}
