#include<iostream>
#include <string>
#include "CapycitySim.h"
using namespace std;

// main
int main() {

    CapycitySim city = CapycitySim();
    Blueprint blueprint = Blueprint();
    bool done = false;
    int area_length, area_width;
    cout << "Willkommen bei Capycity. Erstelle einen neuen Bauplan: \n" << endl;

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
            // Setzen der Parameter
            blueprint.setAreaLength(area_length);
            blueprint.setAreaWidth(area_width);

        }
        catch (const char* txtMsg) {
            cout << "\nError: " << txtMsg << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }
    }

    // 2D Gebaeudearray
    Building** obj_blueprint = new Building * [blueprint.getAreaWidth()];
    for (int i = 0; i < blueprint.getAreaWidth(); i++) {
        obj_blueprint[i] = new Building[blueprint.getAreaLength()];
    }

    // Initialisierung des 2D- Arrays mit freien Plätzen (0)
    for (int i = 0; i < blueprint.getAreaWidth(); i++) {
        for (int j = 0; j < blueprint.getAreaLength(); j++) {
            obj_blueprint[i][j].label;
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
                blueprint.setBuilding(obj_blueprint);
                wrong_input = false;
                break;
            case 2: // Bereich loeschen
                blueprint.deleteArea(obj_blueprint);
                wrong_input = false;
                break;
            case 3: // Bauplan ausgeben
                blueprint.print_blueprint(obj_blueprint);
                wrong_input = false;
                break;
            case 4: // Programm beenden
                city.endProgram();
                wrong_input = false;
                break;
            default:
                cout << "Keine gueltige Eingabe! Versuche es erneut!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }    return 0;
}


