#include <iostream>
using namespace std;

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

    do { // TODO: Input validation for colors, limit input to 3 integers per color.
         // Currently accepts both colors if the user enters in 6 numbers at once.
        cout <<"Enter RGB values (0-255) with a single space in between for two unique colors: \n";
        cout << "Color 1: ";
        cin >> r1 >> g1 >> b1;
        cout << endl;
        cout << "Color 2: ";
        cin >> r2 >> g2 >> b2;
        cout << endl;

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

    cout << "[Debug] You have selected the colors (" << r1 << " " << g1 << " " << b1 << ") and (" << r2 << " " << g2 << " " << b2 << ")." << endl;
    cout << "[Debug] r_score: " << r_score << " , g_score: " << g_score << " , b_score: " << b_score << " ; rgb_score: " << rgb_score << " ." << endl;
    // Will remove eventually or format for a more user-friendly display.

    double vis_score = rgb_score / 4; // IDE warning, possible loss of precision due to integer division in a floating point context. How relevant is this?
    cout << "[Debug] vis_score = " << vis_score << endl;

    if (vis_score >= 45) {
        //good
        cout << "These colors should be visible when placed together.\n";
    } else if (vis_score >= 30 && vis_score < 45) {
        //decent
        cout << "These colors should be fairly visible when placed together, but you may want to consider altering your choices for best visibility.\n";
    } else {
        //not good
        cout << "These colors don't seem to be very distinct from each other. You should pick some better colors.\n";
    }

    cout << "[Debug] Program completed all planned operations successfully and will now terminate.";
    return 0;
}