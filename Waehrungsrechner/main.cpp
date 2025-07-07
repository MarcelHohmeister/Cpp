#include <iostream>
#include <iomanip>  // Für setprecision und fixed
#include <cctype> // für toupper
#include <windows.h>
#include <string>
#include <map>
#include <cmath>

using namespace std;

#pragma region structs
struct CurrencyInfo {
    string name;
    double rateToEuro;
};
#pragma endregion

#pragma region Funktionen
void mainMenu(const map<string, CurrencyInfo>& currencyMap);
void selection(const map<string, CurrencyInfo>& currencyMap);
void showAllKeys(const map<string, CurrencyInfo>& currencyMap);
void startCalculation(const map<string, CurrencyInfo>& currencyMap, string rootCurrency, string destinationCurrency, double amount);
double runde(double wert, int stellen);
string toUpperCase(const string& str);
#pragma endregion

int main() {
    #pragma region Konsoleneinstellungen
    SetConsoleOutputCP(CP_UTF8);  // Ausgabe-Encoding auf UTF-8 setzen
    SetConsoleCP(CP_UTF8);        // Eingabe-Encoding (optional)
    #pragma endregion

    #pragma region Umrechnungsfunktionen
    map<string, CurrencyInfo> currencyMap;

    currencyMap["EUR"] = {"Euro", 1.0};                     //Euro
    currencyMap["USD"] = {"US-Dollar", 0.84895};            //US-Dollar
    currencyMap["CAD"] = {"Kanadischer Dollar", 0.62366};   //Kanadischer Dollar
    currencyMap["AUD"] = {"Australischer Dollar", 0.55665}; //Australischer Dollar
    currencyMap["GBP"] = {"Britisches Pfund", 1.15891};     //Britisches Pfund
    currencyMap["JPY"] = {"Japanischer Yen", 0.00588};      //Japanischer Yen
    currencyMap["INR"] = {"Indische Rupie", 0.00989};       //Indische Rupie
    currencyMap["SEK"] = {"Schwedische Krone", 0.08890};    //Schwedische Krone
    currencyMap["NOK"] = {"Norwegische Krone", 0.08438};    //Norwegische Krone
    currencyMap["TRY"] = {"Türkische Lira", 0.02124};       //Türkische Lira
    currencyMap["RUB"] = {"Russischer Rubel", 0.01077};     //Russischer Rubel

    #pragma endregion

    mainMenu(currencyMap);

    return 0;
}

void mainMenu(const map<string, CurrencyInfo>& currencyMap) {
    cout << endl;
    cout << "≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡\n";
    cout << "Willkommen beim Währungsrechner\n";
    cout << "≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡\n" << endl;

    cout << "===========================================\n";
    cout << "Disclaimer:\n";
    cout << "Bitte beachte, dass die Werte gerundet sind\n";
    cout << "Angaben ohne Gewähr\n" << endl;
    cout << "Stand der Umrechnungskurse 6.7.2025 23:30\n";
    cout << "===========================================\n" << endl;

    cout << "-------------------------------------------\n";
    cout << "Verfügbare Optionen:\n";
    cout << "1: Verfügbare Währungen anzeigen\n";
    cout << "2: Währung umrechnen\n";
    cout << "3: Programm beenden\n";
    cout << "-------------------------------------------\n" << endl;

    cout << "\nEingabe:";

    selection(currencyMap);
}

void selection(const map<string, CurrencyInfo>& currencyMap) {  //const = Daten werden nur gelesen, nicht verändert, & = Referenz übergeben
    double gerundet;
    int selection;

    string rootCurrency;
    string destinationCurrency;
    double amount;

    cin >> selection;

    switch (selection) {
        case 1:
            showAllKeys(currencyMap);
            mainMenu(currencyMap);
            break;

        case 2:
            while (true) {
                cout << "Welche Währung möchtest du umrechnen?\n";
                cout << "Bitte gib das Währungskürzel ein\n";
                cout << "Eine Auflistung der Kürzel kannst du durch Eingeben von 'Liste' einsehen\n" << endl;

                cout << "Eingabe:";

                cin >> rootCurrency;

                rootCurrency = toUpperCase(rootCurrency);

                if (rootCurrency == toUpperCase("Liste")) {
                    showAllKeys(currencyMap);
                    cout << endl;
                    continue;
                }

                if (currencyMap.find(rootCurrency) != currencyMap.end()) {
                    cout << "Bitte gib die Zielwährung ein: ";
                    cin >> destinationCurrency;
                    destinationCurrency = toUpperCase(destinationCurrency);

                    if (currencyMap.find(destinationCurrency) != currencyMap.end()) {
                        cout << "Wieviel " + rootCurrency + " soll umgerechnet werden?";
                        cin >> amount;
                        amount = runde(amount, 2); //Runde auf 2 Nachkommastellen
                        startCalculation(currencyMap, rootCurrency, destinationCurrency, amount);
                    }
                    else {
                        cout << "Dieses Kürzel ist nicht in der Liste vorhanden\n" << endl;
                        continue;
                    }
                    break;
                }
                else {
                    cout << "Dieses Kürzel ist nicht in der Liste vorhanden\n" << endl;
                }
            }
            break;

        case 3:
            //Beenden
            break;

        default:
            break;

    }

    /*Testfall:
                gerundet = runde(currencyMap.at("USD"), 2);
                //cout << fixed << setprecision(2); //zur Sicherheit auf 2 Nachkommastellen festlegen
                cout << "1 US-Dollar entspricht " << gerundet << " EUR" << endl; //.at wird verwendet, da currencyMap["USD"] die map verändert, obwohl sie als const markiert ist
                gerundet = runde(1 / currencyMap.at("USD"), 2);  // Runde den Wert
                cout << "1 Euro entspricht " << gerundet << " EUR" << endl;
                //Verfügbare Währungen anzeigen
                break;
            */
}

void showAllKeys(const map<string, CurrencyInfo>& currencyMap) {
    for (const auto& [key, value] : currencyMap) { //Alle einträge der map mit den variablen key und value (CurrencyInfo) ausgeben
        cout << "Kürzel: " << key << " | "
             << "Name: " << value.name << endl;
    }
}

void startCalculation(const map<string, CurrencyInfo>& currencyMap, string rootCurrency, string destinationCurrency, double amount) {
    //Convert rootCurrency to EUR
    double amountInEuro = amount * currencyMap.at(rootCurrency).rateToEuro;

    //Convert EUR to destinationCurrency
    double finalAmount = amountInEuro / currencyMap.at(destinationCurrency).rateToEuro;
    finalAmount = runde(finalAmount, 2);

    cout << amount << rootCurrency << " entspricht etwa " << finalAmount << destinationCurrency << endl;
}

double runde(double wert, int stellen) {
    double faktor = pow(10.0, stellen);
    return round(wert * faktor) / faktor;
}

string toUpperCase(const string& str) {
    string result = str;
    for (char& c : result) {
        c = toupper(c); // Umwandeln jedes Zeichens in Großbuchstaben
    }
    return result;
}
