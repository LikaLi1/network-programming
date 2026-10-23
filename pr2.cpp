#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <locale>

using namespace std;
using namespace chrono;

int main()
{
    vector<string> menu = {
        "1", "2", "3", "4"
    };

    vector<int> price = {
        217, 340, 173, 184
    };

    string order;
    int total = 0;

    auto start = high_resolution_clock::now();

    while (true) {
        cout << "Menu\n";

        for (int i = 0; i < menu.size(); i++) {
            cout << menu[i] << ": " << price[i] << "$\n";
        }
        
        cout << "Choose whatever you’d like from our menu: \n";
        getline(cin, order);

        if (order == "exit") {
            break;
        }

        bool found = false;

        for (int i = 0; i < menu.size(); i++) {
            if (order == menu[i]) {
                total += price[i];

                cout << "Your receipt: " << total << "$\n";

                found = true;
                break;
            }
        }
        if (!found) {
            cout << "No position\n";
        }
        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;

        cout << "Your total amount due " << total << "$\n";

        cout << "The time for your order was " << elapsed.count() << "seconds\n";
    }
}
