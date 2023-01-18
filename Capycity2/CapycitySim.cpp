#include <iostream>
#include "CapycitySim.h"
using namespace std;


//-------------------------------------Capycity---------------------------------------------------------

Capycity::Capycity() {
    area_length = 0;
    area_width = 0;
    vector<Building> buildingList = {};
}

Capycity::Capycity(int area_l, int area_w) {
    area_length = area_l;
    area_width = area_w;
    vector<Building> buildingList = {};
}
// Pruefung ob Teile eines zu bauenden Gebäudes mit anderen Gebäuden kollidiert oder außerhalb des Baubereichs liegt
bool Capycity::collision(Building** obj_blueprint, int pos_x, int pos_y, int building_width, int building_length) {


    // Pruefung, ob Building innerhalb der Baufläche
    if (((pos_x + building_length) >= area_length) || ((pos_y + building_width) >= area_width)) {
        cout << "Gebaeude ausserhalb des Baubereichs! ";
        return true;
    }
    // Pruefung, ob benoetigte Felder bereits besetzt sind
    for (int i = pos_y; i < (pos_y + building_width); i++) {
        for (int j = pos_x; j < (pos_x + building_length); j++) {
            if (obj_blueprint[i][j].label != "Leer") {
                cout << "Gebaeude kollidiert mit anderem Gebaeude! ";
                return true;
            }
        }
    }
    return false;
}

// Gebaeude setzen
void Capycity::setBuilding(Building** obj_blueprint) {
    // Lokale 
    int building_width, building_length, pos_x, pos_y;
    bool done = false;
    int b_type;

    // Userabfrage mit Exceptions
    while (!done) {
        try {
            cout << "Windkraftwerk (1), Wasserkraftwerk (2) oder Solarpanele (3) bauen?" << endl;
            cin >> b_type;
            if (!((b_type == 1) || (b_type == 2) || (b_type == 3)))
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
            // wenn keine fehler, dann fertig mit while schleife und user abfrage
            done = true;
            // user eingaben als flaeche für das gebaeude setzen
        }
        catch (const char* txtException) {
            cout << "\nError: " << txtException << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Gebaeude einfügen
    if (!collision(obj_blueprint, pos_x, pos_y, building_width, building_length)) {
        for (int i = pos_y; i < (pos_y + building_width); i++) {
            for (int j = pos_x; j < (pos_x + building_length); j++) {
                switch (b_type) {
                case 1:
                    obj_blueprint[i][j] = Windkraftwerk(pos_x, (pos_x + building_length) - 1, pos_y, (pos_y + building_width) - 1);
                    break;
                case 2:
                    obj_blueprint[i][j] = Wasserkraftwerk(pos_x, (pos_x + building_length) - 1, pos_y, (pos_y + building_width) - 1);
                    break;
                case 3:
                    obj_blueprint[i][j] = Solarpanele(pos_x, (pos_x + building_length) - 1, pos_y, (pos_y + building_width) - 1);
                    break;
                }
            }
        }
        // Gebaeude der buildingList hinzufuegen
        if(b_type == 1)
            buildingList.push_back(Windkraftwerk(pos_x, (pos_x + building_length) - 1, pos_y, (pos_y + building_width) - 1));
        if(b_type == 2)
            buildingList.push_back(Wasserkraftwerk(pos_x, (pos_x + building_length) - 1, pos_y, (pos_y + building_width) - 1));
        if(b_type == 3)
            buildingList.push_back(Solarpanele(pos_x, (pos_x + building_length) - 1, pos_y, (pos_y + building_width) - 1));
}
    else {
        cout << "Gebaeude konnte nicht eingefuegt werden!" << endl;
    }

}
// Bauflaeche freigeben
void Capycity::deleteArea(Building** obj_blueprint) {
    int delete_x1, delete_x2, delete_y1, delete_y2, delete_length, delete_width;
    bool done = false;
    while (!done) {
        try {
            cout << "An welchen Koordinaten (x, y) soll ein Bereich geloescht werden?";
            cin >> delete_x1 >> delete_y1;
            if ((delete_x1 < 0 || (delete_x1 >= area_length - 1)) || (delete_y1 < 0) || (delete_y1 > area_width - 1))
                throw "Ungueltiger Koordinatenbereich!";

            cout << "Welche Groesse soll der Bereich haben (Laenge, Breite)?";
            cin >> delete_length >> delete_width;
            if ((delete_length < 0 || (delete_length >= area_length - 1)) || (delete_width < 0) || (delete_width > area_width - 1))
                throw "Ungueltiger Koordinatenbereich!";
            //Userabfrage war erfolgreich
            done = true;
            //Berechnung der restlichen Koordinaten x2 und y2
            delete_x2 = delete_x1 + delete_length - 1;
            delete_y2 = delete_y1 + delete_width - 1;
            //Löschen jedes angegebenen Feldes
            int count_deletedBuildings = 0;
            vector<Building>& ref = buildingList;
            for (int i = delete_y1; i <= delete_y2; i++) {
                for (int j = delete_x1; j <= delete_x2; j++) {
                    if (obj_blueprint[i][j].getLabel() != "Leer") {
                        count_deletedBuildings += 1;
                        for (auto b : buildingList) {
                            auto& b_ref = b;
                            b.removeRessources(ref, b_ref);
                        }
                        obj_blueprint[i][j].deleteBuilding();
                        
                    }
                }
            }updateBuildingList(obj_blueprint, buildingList);
            
        }
        catch (const char* txtException) {
            cout << "\nError: " << txtException << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

}
// Ausgabe Bauplan
void Capycity::print_blueprint(Building** obj_blueprint) {
    // Ausgabe des Bauplans
    for (int i = 0; i < area_width; i++) {
        for (int j = 0; j < area_length; j++) {
            cout << obj_blueprint[i][j].label << "\t";
        }
        cout << endl;
    }

    // Ausgabe der Gebaeudeliste
    cout << "\nListe der gebauten Gebaeude: " << endl;
    for (int i = 0; i < buildingList.size(); i++) {
        cout << buildingList[i].toString() << endl;
    }
    cout << endl;

    // Ausgabe des Gesamtpreises aller Gebaeude
    int result = 0;
    for (int i = 0; i < buildingList.size(); i++) {
        result += (buildingList[i].baseprice + buildingList[i].priceOfRessources);
    }
    cout << "\nGesamtpreis aller Gebaeude: " << result << endl;
}
// Programm beenden
void Capycity::endProgram() {
    cout << "Programm wird beendet...";
    exit(0);
}

// Getter
int Capycity::getAreaLength() {
    return area_length;
}
int Capycity::getAreaWidth() {
    return area_width;
}
// Setter
void Capycity::setAreaLength(int length) {
    area_length = length;
}
void Capycity::setAreaWidth(int width) {
    area_width = width;
}
void Capycity::reduceBuildingList(int x1, int x2, int y1, int y2) {
    // Fall 1: Zu löschender Bereich umfasst gesamtes Gebaeude
    int index = 0;
    for (auto i : buildingList) {
        if (x1 <= i.flaeche.x1 && x2 >= i.flaeche.x2 && y1 <= i.flaeche.y1 && y2 >= i.flaeche.y2)
            buildingList.erase(buildingList.begin() + index);
        index++;


    }

}
vector<Building> Capycity::updateBuildingList(Building** blueprint, vector<Building>& buildingList) {
    buildingList.clear();
       
    for(int i = 0; i < area_length-1; i++)
        for (int j = 0; j < area_width-1; j++) {
            if((blueprint[i][j].getLabel() != blueprint[i+1][j].getLabel()) && (blueprint[i][j].getLabel() != blueprint[i][j+1].getLabel()) && blueprint[i][j].getLabel() != "Leer")
                buildingList.push_back(blueprint[i][j]);
        }
    return buildingList;
}


//-------------------------------------Materialien---------------------------------------------------------

//Konstruktoren
Material::Material() {
    price = 0;
    name = "Leer";
}
Holz::Holz() {
    price = 50;
    name = "Holz";
}
Metall::Metall() {
    price = 150;
    name = "Metall";
}
Kunststoff::Kunststoff() {
    price = 30;
    name = "Kunststoff";
}

string Material::toString() {
    return name;
}
string Holz::toString() {
    return "Holz";
}
string Metall::toString() {
    return "Metall";
}
string Kunststoff::toString() {
    return "Kunststoff";
}



//-------------------------------------Buildings---------------------------------------------------------

// StandardKonstruktoren

Building::Building() {
    label = "Leer";
    baseprice = 0;
    this->flaeche.setFlaeche(0, 0, 0, 0);
}
Solarpanele::Solarpanele() {
    label = "Solar";
    baseprice = 1000;
    this->flaeche.setFlaeche(0, 0, 0, 0);
    priceOfRessources = 0;
    for (int i = 0; i < 1 * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Holz());
        priceOfRessources += Holz().price;
    }
    for (int i = 0; i < 2 * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Metall());
        priceOfRessources += Metall().price;
    }
    for (int i = 0; i < 1 * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Kunststoff());
        priceOfRessources += Kunststoff().price;
    }
}
Windkraftwerk::Windkraftwerk() {
    label = "WiKra";
    baseprice = 1500;
    this->flaeche.setFlaeche(0, 0, 0, 0);
    priceOfRessources = 0;
    for (int i = 0; i < 3 * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Holz());
        priceOfRessources += Holz().price;
    }
    for (int i = 0; i < 2 * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Metall());
        priceOfRessources += Metall().price;
    }
    for (int i = 0; i < 1 * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Kunststoff());
        priceOfRessources += Kunststoff().price;
    }
}
Wasserkraftwerk::Wasserkraftwerk() {
    label = "WaKra";
    baseprice = 2000;
    this->flaeche.setFlaeche(0, 0, 0, 0);
    priceOfRessources = 0;
    for (int i = 0; i < 2 * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Holz());
        priceOfRessources += Holz().price;
    }
    for (int i = 0; i < 1 * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Metall());
        priceOfRessources += Metall().price;
    }
    for (int i = 0; i < 1 * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Kunststoff());
        priceOfRessources += Kunststoff().price;
    }
}

// andere Konstruktoren
Building::Building(int x1, int x2, int y1, int y2) {
    label = "Leer";
    baseprice = 0;
    this->flaeche.setFlaeche(x1, x2, y1, y2);
}
Solarpanele::Solarpanele(int x1, int x2, int y1, int y2) {
    label = "Solar";
    baseprice = 1000;
    this->flaeche.setFlaeche(x1, x2, y1, y2);
    priceOfRessources = 0;
    req_wood = 1;
    req_met = 2;
    req_pla = 3;
    for (int i = 0; i < req_wood * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Holz());
        priceOfRessources += Holz().price;
    }
    for (int i = 0; i < req_met * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Metall());
        priceOfRessources += Metall().price;
    }
    for (int i = 0; i < req_pla * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Kunststoff());
        priceOfRessources += Kunststoff().price;
    }
}
Windkraftwerk::Windkraftwerk(int x1, int x2, int y1, int y2) {
    label = "WiKra";
    baseprice = 1500;
    this->flaeche.setFlaeche(x1, x2, y1, y2);
    priceOfRessources = 0;
    req_wood = 3;
    req_met = 2;
    req_pla = 1;
    for (int i = 0; i < req_wood * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Holz());
        priceOfRessources += Holz().price;
    }
    for (int i = 0; i < req_met * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Metall());
        priceOfRessources += Metall().price;
    }
    for (int i = 0; i < req_pla * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Kunststoff());
        priceOfRessources += Kunststoff().price;
    }
}
Wasserkraftwerk::Wasserkraftwerk(int x1, int x2, int y1, int y2) {
    label = "WaKra";
    baseprice = 2000;
    this->flaeche.setFlaeche(x1, x2, y1, y2);
    priceOfRessources = 0;
    req_wood = 2;
    req_met = 1;
    req_pla = 2;
    for (int i = 0; i < req_wood * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Holz());
        priceOfRessources += Holz().price;
    }
    for (int i = 0; i < req_met * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Metall());
        priceOfRessources += Metall().price;
    }
    for (int i = 0; i < req_pla * (this->flaeche.getLength() * this->flaeche.getWidth()); i++) {
        ressources.push_back(Kunststoff());
        priceOfRessources += Kunststoff().price;
    }
}

// Methoden


// Label ausgeben
string Building::getLabel() {
    return label;
}

string Building::resToString() {
    ostringstream list;
    list << "[";
    for (auto i : ressources)
        list << i.toString() << ", ";
    list << "]";
    return list.str();
}
// toString
string Building::toString() {
    ostringstream strout;
    string listRes = "";
    //string str(ressources.begin(), ressources.end());
    strout << " Gebaeudetyp: " << this->label << "\n Gebaeudepreis = " << (this->baseprice + this->priceOfRessources) << "\n Ressourcenliste: " << this->resToString() << "\n" << endl;
    return strout.str();
}
// Gebaeude abreissen
void Building::deleteBuilding() {
    label = "Leer";
    ressources.clear();
    baseprice = 0;
}
void Building::removeRessources(vector<Building>& buildingList, Building& b) {
        // Holz loeschen
        b.ressources.erase(ressources.begin(), (ressources.begin() + req_wood - 1));
        //Metall loeschen
        b.ressources.erase(ressources.begin() + (ressources.size() / 2), ((ressources.begin() + (ressources.size() / 2)) + req_met));
        //Plastik loeschen
        b.ressources.erase(ressources.end() - req_pla - 1, ressources.end());
    

}

//--------------------------------------------------------Flaeche----------------------------------------

Flaeche::Flaeche() {
    this->x1 = 0;
    this->x2 = 0;
    this->y1 = 0;
    this->y2 = 0;
    this->length = 0;
    this->width = 0;
    this->area_sum = 0;
}

void Flaeche::setFlaeche(int x1, int x2, int y1, int y2) {
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    this->length = x2 - x1 + 1;
    this->width = y2 - y1 + 1;
    this->area_sum = width * length;
}

int Flaeche::getLength() {
    return this->length;
}
int Flaeche::getWidth() {
    return this->width;
}



