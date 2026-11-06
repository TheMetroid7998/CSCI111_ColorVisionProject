// Author: Jaz Salazar
// See dev branch for additional documentation.
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <cmath>
#include <ctime>

using namespace std;

bool file_preflight(bool& file_io, const filesystem::path& file_path);
bool input_preflight(string& color_input, int& format_base);
string hex_to_dec(string color_input);
bool rgb_validator(string color_input, int& red, int& green, int& blue);
double contrast_calculator(int rgb_colors[2][3]);
void rgb_to_srgb(const int rgb_colors[2][3], double (&srgb_colors)[2][4]);
void output_report (bool file_io, const int rgb_colors[2][3], double contrast_score);

const filesystem::path DEFAULT_FILE_PATH ("../colors.txt");
constexpr int BASE_DEC = 10;
constexpr int BASE_HEX = 16;
constexpr double WCAG_AAA = 7.0;
constexpr double WCAG_AA = 4.5;

/*
static struct {
    double lvl_AAA = 7.0;
    double lvl_AA = 4.5;
    double r_lum = 0.2126;
    double g_lum = 0.7152;
    double b_lum = 0.0722;
    double piecewise_threshold = 0.03928;
    double accurate_threshold = 0.04045;
} WCAG; */

int main() {
    // Two-dimensional array that can store 2 rows of 3 columns containing RGB color values
    // Allows use of a for loop to reuse the input logic with flexible pointers
    int rgb_colors[2][3] = {0}; // All values initialized to 0 to prevent potential references to corrupted data
    bool file_io;
    ifstream user_color_file; // Creates file object but does not open anything until user specifies desire for file io

    cout << "\nThis program will evaluate two colors based on WCAG 2.x.\n";

    if (file_preflight(file_io, DEFAULT_FILE_PATH)) {
        user_color_file.open(DEFAULT_FILE_PATH);
    } else {
        cout << "RGB colors must be three integers separated by whitespaces and between 0-255.\n"
            "HEX colors must be in six digit format (optionally with '#' or '0x' prefixes).\n"
            "Examples: '157 34 53', '0 0 0', '0x9D2235', '#75787b'\n\n";
    }

    for (int i = 0; i < 2; i++) {
        bool valid_color_format = false;

        do {
            string color_input;
            int format_base = 0; // initialized here so that it's reset with each loop

            if (file_io) {
                getline(user_color_file, color_input);
                cout << "Read color '" << color_input << "' from file " << DEFAULT_FILE_PATH << ".\n" << endl;

            } else {
                cout << "Enter color values in RGB or HEX format for Color " << i+1 << " of 2:\n";
                getline(cin, color_input);
                // if (!cin) {return 1}
            }

            if (!input_preflight(color_input,format_base)) {
                goto bad_input; // be careful with goto, I think it works ok here
            }

            if (format_base == BASE_HEX) {
                // cout << "[DEBUG] Format confirmed as HEX. Proceeding to convert HEX --> DEC.\n";
                string hex_converted = hex_to_dec(color_input);
                // cout << "[DEBUG] Converted HEX --> DEC. Proceeding to verify converted RGB format.\n";
                valid_color_format = rgb_validator(hex_converted,
                rgb_colors[i][0],
                rgb_colors[i][1],
                rgb_colors[i][2]
                );
            } else if (format_base == BASE_DEC) {
                // cout << "[DEBUG] Format confirmed as DEC. Proceeding to verify RGB format.\n";
                valid_color_format = rgb_validator(color_input,
                    rgb_colors[i][0],
                    rgb_colors[i][1],
                    rgb_colors[i][2]
                    );
            } else {
                cout << "ERROR: Input format could not be determined!\n";
                cout << "[DEBUG] The value of 'input_base' is invalid.\n";
                exit(2);
            }

            // label pointer used to skip validation if input fails basic checks
            bad_input:
            if (!valid_color_format) {
                cout << "\nInvalid input. Please try again.\n";
                cout << "Examples of valid input: '157 34 53', '0 0 0', '0x9D2235', '#75787B'\n";
            }
        } while (!valid_color_format);
    }

    cout << "You have selected the following colors: "
        << rgb_colors[0][0] << " " << rgb_colors[0][1] << " " << rgb_colors[0][2] << " and "
        << rgb_colors[1][0] << " " << rgb_colors[1][1] << " " << rgb_colors[1][2] << "\n";

    double contrast_score = contrast_calculator(rgb_colors);
    cout << "Contrast Ratio: " << contrast_score << ":1\n";
    // contrast score goes from 1:1 to 21:1

    if (contrast_score >= WCAG_AAA) { // contrast greater than 7:1
        cout << "\nThese colors have a good amount of contrast.\n";
    } else if (contrast_score >= WCAG_AA && contrast_score < WCAG_AAA) { // contrast between 4.5:1 & 7:1
        cout << "\nThese colors have a decent amount of contrast.\n";
    } else { //contrast score < 3.0
        cout << "\nThese colors have very low contrast. You should pick some better colors.\n";
    }

    if (file_io) {
        user_color_file.close();
        output_report(file_io, rgb_colors, contrast_score);
    }
    return 0;
}

bool file_preflight(bool& file_io, const filesystem::path& file_path) {
    // file_path = DEFAULT_FILE_PATH (for now)
    char io_choice = 'X'; // initialized to an invalid choice to keep loop running

    do {
        cout << "Will you read your colors from a file [Y/N]?\n";
        cin >> io_choice;
        if (io_choice == 'Y' || io_choice == 'y') {
            file_io = true;
            cout << "Colors must be entered in a text document named 'colors.txt' in the root directory.\n"
                "Currently, only the first two colors found will be read from the file due to budget cuts." << endl;
        } else if (io_choice == 'N' || io_choice == 'n') {
            file_io = false;
            return false;
        } else {
            cout << "Please make a valid choice ['Y' or 'N'].\n";
        }
    } while (io_choice == 'X');

    /*
    // save custom paths for 1.7.x
    if (file_io) {
        io_choice = 'X';
        filesystem::path user_file_path;

        cout << "Is your file located at " << file_path << " [Y/N]?\n";
        if (io_choice == 'N' || io_choice == 'n') {
            while (file_path.empty() || file_path == "\"../colors.txt\"") {
                cin >> user_file_path;
            }
            user_file_path = file_path;
        } else if (io_choice == 'Y' || io_choice == 'y') {
            return true;
        } else {
            cout << "Please make a valid choice ['Y' or 'N'].\n";
        }
    }
    */

    if (file_io) {

        if (!filesystem::exists(file_path)) { // Check if file exists
            cout << "ERROR: A readable file was not found at  " << file_path << "!\n";
            return false;
        }

        if (filesystem::file_size(file_path) == 0) { // Check if file is not empty
            cout << "ERROR: The file found at " << file_path << " contains no data!\n ";
            return false;
        }

        return true;
    } else {
        return false;
    }
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
    // HEX has to have at least 6 chars, DEC could technically fit in 5 chars
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
        format_base = BASE_HEX; // already checked above. applying the base confirmation here to skip DEC if needed
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
        format_base = BASE_DEC;
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


            // HEX input was already converted to uppercase alphanumeric in 'preflight' btw
            if (hex_val[j] >= '0' && hex_val[j] <= '9') {
                // Comparison works bc C++ compares ASCII codes, which are sequential
                digit = hex_val[j] - '0';
                // ASCII character codes are offset by 48, which corresponds to the character '0'
                // Subtracting the ASCII character code offset gives the numerical integer value
            } else if (hex_val[j] >= 'A' && hex_val[j] <= 'F') {
                digit = hex_val[j] - 'A' + 10;
                // Subtracts offset of 65 for 'A' and corrects offset to 55
                // Easier to figure out than `digit = hex_val[j] - '7'`
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

    /* newer version that I haven't tested extensively yet.
    rgb >> r >> g >> b >> ws;
    if (!rgb.eof()) {
        return false;
    } */

    // Reject values outside 0-255
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        // cout << "[DEBUG] RGB range checks failed!\nReason: At least 1 integer value(s) were outside the 0-255 range.\n";
        return false;
    }

    // Reject extraneous input beyond 3 integers
    string extra;
    if (rgb >> extra) {
        // cout << "[DEBUG] RGB input validation failed!\nReason: More than 3 values were parsed from user input.\n";
        return false;
    }

    red = r;
    green = g;
    blue = b;
    // cout << "[DEBUG] RGB validation completed successfully!\n";
    return true;
}

// Source: https://www.w3.org/WAI/GL/wiki/Relative_luminance, https://adamjberkowitz.com/blog/post/how-to-calculate-color-contrast
double contrast_calculator(int rgb_colors[2][3]) {

    double srgb_colors[2][4] = {0};
    rgb_to_srgb(rgb_colors, srgb_colors);

    // Calculates relative luminance for each color
    for (int i = 0; i < 2; i++) {
        srgb_colors[i][3] = (srgb_colors[i][0] * 0.2126) + (srgb_colors[i][1] * 0.7152) + (srgb_colors[i][2] * 0.0722);
        //srgb_colors[i][3] = (srgb_colors[i][0] * WCAG.r_lum) + (srgb_colors[i][1] * WCAG.g_lum) + (srgb_colors[i][2] * WCAG.b_lum);
    }

    double color_contrast = (max(srgb_colors[0][3], srgb_colors[1][3]) + 0.05) / (min(srgb_colors[0][3], srgb_colors[1][3]) + 0.05);
    return color_contrast;
}

// Source: https://www.w3.org/WAI/GL/wiki/Relative_luminance, https://adamjberkowitz.com/blog/post/how-to-calculate-color-contrast
void rgb_to_srgb(const int rgb_colors[2][3], double (&srgb_colors)[2][4]) {
    // i = the individual color entered by the user --> Color 1 or Color 2
    // j = the channel of the individual color, i --> Red (0), Green (1), or Blue (2)
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            double k = rgb_colors[i][j] / 255.0;
            if (k <= 0.03928) { // piecewise equation threshold is technically 0.04045?
                srgb_colors[i][j] = k / 12.92;
            } else {
                srgb_colors[i][j] = pow(((k + 0.055) / 1.055), 2.4);
            }
        }
    }
}

/* source: https://stackoverflow.com/a/56678483
void luminance_to_lightness(double (&srgb_colors)[2][4]) {
    for (int_i = 0; i < 2; i++) { // '_' added to stop CLion from being stupid
        double l = srgb_colors[i][3];
        if (l <= (216.0/24389.0)) {
            srgb_colors[i][3] = l * (24389.0/27.0);
        } else {
            srgb_colors[i][3] = pow(l, (1/3)) * 116 - 16;
        }
    }
} */

void output_report (const bool file_io, const int rgb_colors[2][3], double contrast_score) {
    filesystem::path report_path("../report.txt");
    ofstream report(report_path);

    // Create correct header
    if (!filesystem::exists(report_path)) { // Check if file exists
        report << string(50,'=') << "\n   CSCI-111 Color Vision Project Output Report   \n" << string(50,'=') << "\n\n";
    } else {
        if (filesystem::file_size(report_path) != 0) { // Check if file is not empty
            report << "\n\n";
        } else {
            report << string(50,'=') << "\n   CSCI-111 Color Vision Project Output Report   \n" << string(50,'=') << "\n\n";
        }
    }

    time_t timestamp;
    time(&timestamp);
    report << "Report generated at: " << ctime(&timestamp) << "\n";

    if (file_io) {
        report << "Input method: FILE\n";
    } else {
        report << "Input method: CLI\n";
    }

    report << "Colors evaluated (RGB): "
        << rgb_colors[0][0] << " " << rgb_colors[0][1] << " " << rgb_colors[0][2] << " and "
        << rgb_colors[1][0] << " " << rgb_colors[1][1] << " " << rgb_colors[1][2] << "\n";

    report << "Contrast evaluation: " << contrast_score << ":1\n";
    //report << "(Contrast ratios range from 1:1 to 21:1, higher ratios indicate higher contrast and generally better visibility.)\n\n";

    cout << "A report has been generated at " << report_path << endl;
    report.close();
}