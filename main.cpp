#include <iostream>
#include <sstream>
#include "test_zone.h"

using namespace std;

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

    // TODO: implement support for HEX color format
    do { // TODO: Input validation for colors, limit input to 3 integers per color.
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


// DISCLAIMER: THIS WAS MODIFIED BY AI. MY EXPLANATION / UNDERSTANDING IS BELOW.
bool rgb_validator(string color_input, int& red, int& green, int& blue);
// using int& 'var' allows the original copies of the rgb inputs to be modified, instead of creating copies

bool input_preflight(string user_input, int& format_base);

int main() {
    int rgb_colors[2][3];
    // Two dimensional array that can store 2 rows of 3 RGB color values

    // Using a for loop to reuse the same code for two colors.
    for (int i = 0; i < 2; i++) {

        bool valid_input = false;
        string color_input;
        int format_base = 0;


        do {
            // cout << "Enter three integers (separated by whitespaces and between 0-255) for Color " << i+1 << ":\n";

            cout << "RGB colors must be three integers separated by whitespaces and between 0-255.\nHEX colors cannot have preceding '#' or '0x' characters.\n";
            cout << "Enter colors in RGB or HEX format for Color " << i+1 << ":\n";

            getline(cin, color_input);

            input_preflight(color_input,format_base);

            // remember that this is only for rgb rn
            valid_input = rgb_validator(
                color_input,
                rgb_colors[i][0],
                rgb_colors[i][1],
                rgb_colors[i][2]
                );
            // The 2D array allows me to use a for loop to reuse the input logic with flexible pointers

            if (!valid_input) {
                cout << "Invalid input. Try again.\n";
            }

        } while (!valid_input);
    }

    cout << rgb_colors[0][0] << " " << rgb_colors[0][1] << " " << rgb_colors[0][2] << endl;
    cout << rgb_colors[1][0] << " " << rgb_colors[1][1] << " " << rgb_colors[1][2] << endl;
    cout << "Well, we got this far. That's gotta be a good sign, right?";


    // more code later

    return 0;
}

bool input_preflight(string user_input, int& format_base) {

    if (user_input.empty()) {
        return false;
    }
    while (!user_input.empty() && isspace(user_input.front())) {
        user_input.erase(0, 1);
    } // remove leading spaces
    while (!user_input.empty() && isspace(user_input.back())) {
        user_input.pop_back();
    } // remove trailing spaces

    // can't be HEX if length is 6 and has invalid input
    // can't be DEC if it has invalid input
    if (user_input.size() == 6) {
        for (char c : user_input) {
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
        for (char c : user_input) {
            if (isdigit(c) || isspace(c)) { // CAN'T be EITHER if not valid (already checked for HEX)
            } else {
                return false;
            }
        }
        //cout << "\nFormatting base set to 10 (decimal)...\n";
        format_base = 10;
        // if it's not HEX and all chars are valid (not counting syntax), then it must be DEC
    }
    return true;
}



bool rgb_validator(const string color_input, int& red, int& green, int& blue) {
    // DO NOT PASS `color_input` BY REFERENCE. THAT DEFEATS THE POINT OF MAKING IT A `const`!

    stringstream rgb(color_input);
    int r, g, b;

    /* DISCLAIMER: THIS WAS GENERATED BY AI. MY EXPLANATION / UNDERSTANDING IS BELOW.
     * rgb is extracted to r, g, and b separately, interpreting the string characters as ints
     * stops at spaces, essentially parses the integer until it finds smth that's not an int
     * anything extra is handled below
     */
    // Require 3 valid integers
    // NOTE: This does not handle integer representations such as '+20'. Fix later?
    if (!(rgb >> r >> g >> b)) {
        return false;
    }

    // Reject values outside of 0-255
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        return false;
    }

    /* DISCLAIMER: THIS WAS GENERATED BY AI. MY EXPLANATION / UNDERSTANDING IS BELOW.
     * Rejects extraneous input but permit trailing spaces
     * ws consumes whitespace characters, but stops at non whitespace characters
     * eof is "end of file"
     * if all whitespace characters were consumed and the stream is at eof, then there was no extra stuff
     * if whitespaces were consumed but we're not at eof, then there's extra input
    rgb >> ws;
    if (!rgb.eof() ) {return false;}
    */

    // Reject extraneous input
    // A simpler implementation that fits with existing methodology
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


