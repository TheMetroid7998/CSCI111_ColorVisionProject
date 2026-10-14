#include <iostream>
#include <sstream>

using namespace std;

// ORIGINAL CODE (v1.0)
/*
int main() {
    int r1 = -1;
    int g1 = -1;
    int b1 = -1;
    int r2 = -1;
    int g2 = -1;
    int b2 = -1;
    // initial values set to -1 to auto fail color validity checks if user input fails

    cout << "This program will compare values for two colors to estimate if they have sufficient contrast to be differentiated.\n";

    bool user_satisfy = false;

    do {
         // Currently accepts both colors if the user enters in 6 numbers at once.
        cout <<"Enter RGB values (0-255) with a single space in between for two unique colors: \n";
        cout << "Color 1: ";
        cin >> r1 >> g1 >> b1;
        //cout << endl;
        cout << "Color 2: ";
        cin >> r2 >> g2 >> b2;
        //cout << endl;

        if (r1 >= 0 && r1 <= 255 && g1 >= 0 && g1 <= 255 && b1 >= 0 && b1 <= 255) {
            if (r2 >= 0 && r2 <= 255 && g2 >= 0 && g2 <= 255 && b2 >= 0 && b2 <= 255) {
                // Is there a better way to check all values between 0-255 inclusive here?
                cout << "Both colors validated successfully.\n";
                user_satisfy = true;
            } else {
                cout << "Your first color was fine, but there's an issue with your second color.\n";
                user_satisfy = false;
            }
        } else {
            cout << "Your first color has an issue and couldn't be validated.\n";
            user_satisfy = false;
        }
    } while (user_satisfy == false);

    int r_score = (max(r1, r2) - min(r1, r2));
    int g_score = (max(g1, g2) - min(g1, g2));
    int b_score = (max(b1, b2) - min(b1, b2));
    int rgb_score = (r_score + g_score + b_score)/3;
    // These seem to have to be defined here, the program doesn't seem to use the updated values entered by the user if RGB scores are calculated before input is collected.

    cout << "You have selected the colors (" << r1 << " " << g1 << " " << b1 << ") and (" << r2 << " " << g2 << " " << b2 << ")." << endl;
    //cout << "[Debug] r_score: " << r_score << " , g_score: " << g_score << " , b_score: " << b_score << " ; rgb_score: " << rgb_score << " ." << endl;
    // Will remove eventually or format for a more user-friendly display.


    if (rgb_score >= 180) {
        //good
        cout << "These colors should be visible when placed together.\n";
    } else if (rgb_score >= 120 && rgb_score < 180) {
        //decent
        cout << "These colors should be fairly visible when placed together, but you may want to consider altering your choices for best visibility.\n";
    } else {
        //not good
        cout << "These colors don't seem to be very distinct from each other. You should pick some better colors.\n";
    }

    //cout << "[Debug] Program completed all planned operations successfully and will now terminate.";
    return 0;
}
*/
// REFERENCE ONLY, MARKED FOR DELETION BEFORE RELEASING v2.0


// DISCLAIMER: THIS WAS MODIFIED BY AI. MY EXPLANATION / UNDERSTANDING IS BELOW.
bool rgb_validator(string color_input, int& red, int& green, int& blue);
// using int& 'var' allows the original copies of the rgb inputs to be modified, instead of creating copies

bool input_preflight(string color_input, int& format_base);

string hex_to_dec(const string& color_input, string converted_output);
// using const string& color_input allows for __blank__


int main() {
    int rgb_colors[2][3];
    // Two dimensional array that can store 2 rows of 3 columns containing RGB color values
    // HEX colors are (hopefully) automatically converted to RGB (please please please work omfg)
    // Allows use of a for loop to reuse the input logic with flexible pointers

    // TODO: Consider adding a vector or array to store original color names for user reference?

    // Using a for loop to reuse the same code for two colors.
    for (int i = 0; i < 2; i++) {

        bool valid_color_format = false;
        string color_input;
        int format_base = 0;

        do {
            cout << "RGB colors must be three integers separated by whitespaces and between 0-255.\n"
                    "HEX colors can optionally have '#' or '0x' prefixes.\n"
                    "Enter colors in RGB or HEX format for Color " << i+1 << ":\n";

            getline(cin, color_input);

            if (!input_preflight(color_input,format_base)) {
                /* this is done to counter the automatic 'i++' with each loop iteration
                 * considering putting this as a while loop, but there's issues with storing an unknown number of colors
                 */
                cout << "Invalid input. Try again.\n";
                cout << "Examples of valid input: '157 34 53', '0 0 0', '0x9D2235', '#75787B'";
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
                // remember that this is only for rgb rn
                valid_color_format = rgb_validator(
                    color_input,
                    rgb_colors[i][0],
                    rgb_colors[i][1],
                    rgb_colors[i][2]
                    );
            } else { // format_base should not be anything but 10 or 16 at this point
                cout << "ERROR: Input format could not be determined!\n"
                        "The value of 'input_base' is invalid.\n";
                return 2;
            }

            if (!valid_color_format) {
                cout << "Invalid color format. Please try again.\n";
            }

        } while (!valid_color_format);
    }

    // Scaffolding, remove later.
    cout << rgb_colors[0][0] << " " << rgb_colors[0][1] << " " << rgb_colors[0][2] << endl;
    cout << rgb_colors[1][0] << " " << rgb_colors[1][1] << " " << rgb_colors[1][2] << endl;
    cout << "Well, we got this far. That's gotta be a good sign, right?";


    // The algorithmic color evaluation happens here!
    // But later.

    return 0;
}

bool input_preflight(string& color_input, int& format_base) {
    // TODO: Function 'input_preflight' can be made static or made anonymous to enforce internal linkage

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
    // can't be HEX if length is 6 and has invalid input
    // can't be DEC if it has invalid input
    if (color_input.size() == 6) {
        for (char c : color_input) {
            if (!isxdigit(c) ) { // CAN'T be HEX, could be weird DEC tho
                return false;
            }
        }
        format_base = 16;
        // if it's exactly 6 chars and all are HEX (with no spaces), then it can't be DEC and must be HEX
    }
    // can't be HEX if longer than 6 chars, can't be DEC if invalid input
    // COULD be DEC if 6 chars and has spaces though!
    else {
        for (char c : color_input) {
            if (!isdigit(c) && !isspace(c)) {
                return false;
            } // CAN'T be EITHER if not valid (already checked for HEX)
        }
        format_base = 10;
        // if it's not HEX and all chars are valid (not counting syntax), then it must be DEC
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
            // TODO: Local variable 'c' can be made const
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
            // TODO: Local variable 'c' can be made const
            color_input[c] = toupper(color_input[c]);
            // TODO: Clang-Tidy: Narrowing conversion from 'int' to signed type 'value_type' (aka 'char') is implementation-defined
        }
        format_base = 16; // already checked above. applying the base confirmation here to skip DEC if needed
    }
    else {
        for (char c : color_input) {
            // TODO: Local variable 'c' can be made const
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
string hex_to_dec(const string& color_input, string& converted_output) {
    // TODO: Function 'hex_to_dec' can be made static or made anonymous to enforce internal linkage

    // TODO: Note to self: Why is 'color_input' a const? Learn more about how const and call by reference works.

    string rgb_hex[3];
    for (int c = 0; c < 6; c+=2) {
        rgb_hex[c/2] += color_input[c];
        rgb_hex[c/2] += color_input[c+1];
    }

    // TODO: Clang-Tidy: Loop variable is copied but only used as const reference; consider making it a const reference
    for (string a : rgb_hex) {
        string hex_val;
        hex_val = rgb_hex[stoi(a)]; // writes the hex pair at the iteration index to hex_val

        int dec_val = 0;
        for (int i = 1, base = 1; i >= 0; i--, base += 15) {
            int digit = 0;
            // changes the base for second char position, this SHOULD be reset with every call for rgb_hex[a]
            // if not, we have to implement it differently

            // I don't really understand this part.
            // How does C++ understand the numeric equivalent of the alphabetical characters?

            // HEX input was already converted to uppercase alphanumeric in 'preflight' btw
            if (hex_val[i] >= '0' && hex_val[i] <= '9') {
                digit = hex_val[i] - '0';
            } else if (hex_val[i] >= 'A' && hex_val[i] <= 'F') {
                digit = hex_val[i] - 'A' + 10;
            }
            dec_val += digit * base;
        }

        string dec_str = to_string(dec_val);

        converted_output.append(dec_str);
        if (a != "2") {
            converted_output.append(" ");
        }
    }
    return converted_output;
}

bool rgb_validator(const string color_input, int& red, int& green, int& blue) {
    // DO NOT PASS `color_input` BY REFERENCE. THAT DEFEATS THE POINT OF MAKING IT A `const`!
    // TODO: Clang-Tidy: The const qualified parameter 'color_input' of type 'const string' (aka 'const basic_string<char>') is copied for each invocation; consider making it a reference

    stringstream rgb(color_input);
    int r, g, b;

    /* DISCLAIMER: THIS WAS GENERATED BY AI. MY EXPLANATION / UNDERSTANDING IS BELOW.
     * rgb is extracted to r, g, and b separately, interpreting the string characters as ints
     * stops at spaces, essentially parses the integer until it finds smth that's not an int
     * anything extra is handled below
     */
    // Require 3 valid integers
    // NOTE: This does not handle integer representations such as '+20'. This may have been fixed by the changes to the preflight function.
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


