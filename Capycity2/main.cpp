#include<iostream>
#include <string>
using namespace std;

// Enums für Gebaeudearten
enum building {
    Leer = 0, Wasserkraftwerk = 1, Windkraftwerk = 2, Solarpanele = 3
};
string enum_str[] =
{ "Leer", "WaKra", "WiKra", "Solar" };

// Globale Variablen
int area_length, area_width;

// Pruefung ob Teile eines zu bauenden Gebäudes mit anderen Gebäuden kollidiert oder außerhalb des Baubereichs liegt
bool collision(string** blueprint, int pos_x, int pos_y, int building_width, int building_length) {


    // Pruefung, ob Building innerhalb der Baufläche
    if (((pos_x + building_length) >= area_length) || ((pos_y + building_width) >= area_width)) {
        cout << "Gebaeude ausserhalb des Baubereichs! ";
        return true;
    }
    // Pruefung, ob benoetigte Felder bereits besetzt sind
    for (int i = pos_y; i < (pos_y + building_width); i++) {
        for (int j = pos_x; j < (pos_x + building_length); j++) {
            if (blueprint[i][j] != enum_str[0]) {
                cout << "Gebaeude kollidiert mit anderem Gebaeude! ";
                return true;
            }
        }
    }
    return false;
}

// Gebaeude setzen
void setBuilding(string** blueprint) {
    // Lokale Variablen
    int building_width, building_length, pos_x, pos_y;
    bool done = false;
    string b_type = "";

    // Userabfrage mit Exceptions
    while (!done) {
        try {
            cout << "Gebaeudeart (WaKra, WiKra oder Solar): ";
            cin >> b_type;
            if ((b_type != enum_str[1]) && (b_type != enum_str[2]) && (b_type != enum_str[3]))
                throw "Ungueltiger Gebaeudetyp!";
            cout << "Laenge: ";
            cin >> building_length;
            if (building_length <= 0)
                throw "Ungueltige Gebaeudelaenge!";
            cout << "Breite: ";
            cin >> building_width;
            if (building_width <= 0)
                throw "Ungueltige Gebaeudebreite!";

            cout << "Position der linken oberen Ecke (x y): ";
            cin >> pos_x >> pos_y;
            if ((pos_x < 0) || (pos_y < 0))
                throw "Ungueltige Position! ";
            done = true;
        }
        catch (const char* txtException) {
            cout << "\nError: " << txtException << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Gebaeude einfügen
    if (!collision(blueprint, pos_x, pos_y, building_width, building_length)) {
        for (int i = pos_y; i < (pos_y + building_width); i++) {
            for (int j = pos_x; j < (pos_x + building_length); j++) {
                blueprint[i][j] = b_type;
            }
        }
    }
    else {
        cout << "Gebaeude konnte nicht eingefuegt werden!" << endl;
    }

}
// Bauflaeche freigeben
void deleteArea(string** blueprint) {
    int delete_x1, delete_x2, delete_y1, delete_y2;
    bool done = false;
    while (!done) {
        try {
            cout << "Welchen x- Bereich loeschen (x1 x2)?";
            cin >> delete_x1 >> delete_x2;
            if ((delete_x1 < 0 || (delete_x1 >= area_length - 1)) || (delete_x2 < 0) || (delete_x2 > area_length - 1))
                throw "Ungueltiger x-Bereich!";

            cout << "Welchen y-Bereich loeschen (y1 y2)?";
            cin >> delete_y1 >> delete_y2;
            if ((delete_y1 < 0 || (delete_y1 >= area_width - 1)) || (delete_y2 < 0) || (delete_y2 > area_width - 1))
                throw "Ungueltiger y-Bereich!";

            for (int i = delete_y1; i <= delete_y2; i++) {
                for (int j = delete_x1; j <= delete_x2; j++) {
                    blueprint[i][j] = enum_str[0];
                }
            }
            done = true;
        }
        catch (const char* txtException) {
            cout << "\nError: " << txtException << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

}
// Bauflaeche ausgeben
void print_blueprint(string** blueprint) {
    // Ausgabe des Bauplans
    for (int i = 0; i < area_width; i++) {
        for (int j = 0; j < area_length; j++) {
            cout << blueprint[i][j] << "\t";
        }
        cout << endl;
    }
}
// Programm beenden
void endProgram() {
    cout << "Programm wird beendet...";
    exit(0);
}
// main
int main() {

    bool done = false;
    cout << "Erstelle einen Plan: \n\n";

    // Benutzereingabe der Länge und Breite des Baubereichs
    while (!done) {
        try {
            cout << "Laenge des Baubereichs: ";
            cin >> area_length;
            if ((area_length <= 0) || !cin.good())
                throw "Keine gueltige Laenge!";
            cout << "Breite des Baubereichs: ";
            cin >> area_width;
            if ((area_width <= 0) || !cin.good())
                throw "Keine gueltige Breite";
            done = true;

        }
        catch (const char* txtMsg) {
            cout << "\nError: " << txtMsg << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }
    }

    // Array für Baubereich
    string** blueprint = new string * [area_width];
    for (int i = 0; i < area_width; i++) {
        blueprint[i] = new string[area_length];
    }

    // Initialisierung des 2D- Arrays mit freien Plätzen (0)
    for (int i = 0; i < area_width; i++) {
        for (int j = 0; j < area_length; j++) {
            blueprint[i][j] = enum_str[0];
        }

    }
    while (true) {

        // Aktionsmenue
        int input;
        bool wrong_input = true;
        while (wrong_input) {

            cout << "Menue: \n Gebaeude setzen (1) \n Bereich loeschen (2) \n Bauplan ausgeben (3) \n Programm beenden (4)\n";
            cin >> input;

            switch (input) {
            case 1: // Gebaeude setzen
                setBuilding(blueprint);
                wrong_input = false;
                break;
            case 2: // Bereich loeschen
                deleteArea(blueprint);
                wrong_input = false;
                break;
            case 3: // Bauplan ausgeben
                print_blueprint(blueprint);
                wrong_input = false;
                break;
            case 4: // Programm beenden
                endProgram();
                wrong_input = false;
                break;
            default:
                cout << "Keine gueltige Eingabe! Versuche es erneut.";
            }
        }
    }
    return 0;
}
