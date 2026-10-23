#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

int main() {
    int allCoffee = 500;
    int forOneCup = 40;
    int coffeeCup = 0;

    if (allCoffee < forOneCup) {
        cout << "Its impossible to make coffee, there isnt enough of it\n";
    }
    else {
        cout << "There is some coffee left for " << allCoffee / forOneCup << " cups\n";
    }

    while (allCoffee >= forOneCup) {
        cout << "Enter the start of the command (Brew coffee or not): \n";
        string answer;
        getline(cin, answer);
        // у меня есть пробелы в ответе

        if (answer == "Brew coffee") {
            cout << "Starting the coffee preparation process\n";
            allCoffee -= forOneCup;

            for (int i = 0; i < 10; i++) {
                cout << "-";

                this_thread::sleep_for(seconds(1));
            }
            cout << endl;

            coffeeCup++;
            cout << "Your coffee is ready\n";
            cout << "Total number of cups of coffee: " << coffeeCup << endl;
        }
        else if (answer == "not") {
            cout << "You declined a cup of coffee\n";
            break;
        }
        else {
            cout << "There is no such command. Type it correctly!!!\n";
        }
    }
}
