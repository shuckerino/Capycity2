#include <iostream>
#include "CapycitySim.h"
#include <algorithm>
using namespace std;


//-------------------------------------CapycitySim---------------------------------------------------------

CapycitySim::CapycitySim() {
    vector<Blueprint> blueprint_list = {};
}

// Programm beenden
void CapycitySim::endProgram() {
    cout << "Programm wird beendet...";
    exit(0);
}


void CapycitySim::save(Blueprint& blueprint) {
    // überprüfen auf Gleichheit von Plänen
    bool needs_save = true;
    for (auto i : blueprint_list) {
        if (blueprint(i)) { // Aufrufen des Funktors 
            needs_save = false;
        }
    }
    if(needs_save)
        blueprint_list.push_back(blueprint);
    // wenn noch keine Baupläne gespeichert, dann auf jeden Fall speichern
    if (blueprint_list.empty()) {
        blueprint_list.push_back(blueprint);
    }


        
}

void CapycitySim::menu(CapycitySim& sim) {

    //Ersten Blueprint automatisch erstellen
    Blueprint blueprint = create_blueprint();


    while (true) {

        // Aktionsmenue
        int input;
        bool wrong_input = true;
        while (wrong_input) {

            cout << "Menue: \n Gebaeude setzen (1) \n Bereich loeschen (2) \n Bauplan ausgeben (3) \n Neuen Bauplan erstellen (4) \n Alle Bauplaene ausgeben (5) \n Programm beenden (6)\n";
            cin >> input;

            switch (input) {
            case 1: // Gebaeude setzen
                blueprint.setBuilding(blueprint.obj_blueprint);
                wrong_input = false;
                break;
            case 2: // Bereich loeschen
                blueprint.deleteArea(blueprint.obj_blueprint);
                wrong_input = false;
                break;
            case 3: // Bauplan ausgeben
                blueprint.print_blueprint(blueprint.obj_blueprint);
                wrong_input = false;
                break;
            case 4: //Neuen Bauplan erstellen
                sim.save(blueprint);
                blueprint = create_blueprint();
                wrong_input = false;
                break;
            case 5: //Alle Baupläne ausgeben
                sim.print_all_blueprints();
                wrong_input = false;
                break;
            case 6: // Programm beenden
                sim.endProgram();
                wrong_input = false;
                break;
            default:
                cout << "Keine gueltige Eingabe! Versuche es erneut!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }
}

void CapycitySim::print_all_blueprints() {
    
    // Lambda Funktion, um Bauplaene absteigend nach der Kennzahl zu sortieren
    sort(blueprint_list.begin(),
        blueprint_list.end(),
        [](Blueprint bp, Blueprint bp2)
        {return (bp.getKennzahl() > bp2.getKennzahl()); });

    //Ausgabe der Bauplanliste
    int index = 1;
    for (auto i : this->blueprint_list) {
        cout << " Bauplan Nummer: " << index << endl;
        i.print_blueprint(i.obj_blueprint);
        index++;

    }
        
}



//-----------------------------------------------Blueprint----------------------------------------------------------------------
Blueprint::Blueprint() {
    kennzahl = 0;
    area_length = 0;
    area_width = 0;
    vector<Building> buildingList = {};
    this->create_object_blueprint();
}

Blueprint::Blueprint(int area_l, int area_w) {
    kennzahl = 0;
    area_length = area_l;
    area_width = area_w;
    vector<Building> buildingList = {};
    this->create_object_blueprint();
    
}
Building** Blueprint::create_object_blueprint() {
    bool done = false;
    int area_length, area_width;
    cout << "Erstelle einen neuen Bauplan: \n" << endl;

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
            this->setAreaLength(area_length);
            this->setAreaWidth(area_width);

        }
        catch (const char* txtMsg) {
            cout << "\nError: " << txtMsg << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }
    }

    // 2D Gebaeudearray
    obj_blueprint = new Building * [this->getAreaWidth()];
    for (int i = 0; i < this->getAreaWidth(); i++) {
        obj_blueprint[i] = new Building[this->getAreaLength()];
    }

    // Initialisierung des 2D- Arrays mit freien Plätzen (0)
    for (int i = 0; i < this->getAreaWidth(); i++) {
        for (int j = 0; j < this->getAreaLength(); j++) {
            obj_blueprint[i][j].label;
        }

    }
    return obj_blueprint;
}

Blueprint CapycitySim::create_blueprint() {
    Blueprint blueprint = Blueprint();
    return blueprint;
}

// Pruefung ob Teile eines zu bauenden Gebäudes mit anderen Gebäuden kollidiert oder außerhalb des Baubereichs liegt
bool Blueprint::collision(Building** object_bluepr, int pos_x, int pos_y, int building_width, int building_length) {


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
void Blueprint::setBuilding(Building** object_blueprint) {
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
        }catch (const char* txtException) {
                cout << "\nError: " << txtException << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }


        // Gebaeude einfügen
        if (!collision(this->obj_blueprint, pos_x, pos_y, building_width, building_length)) {
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
            if (b_type == 1)
                buildingList.push_back(Windkraftwerk(pos_x, (pos_x + building_length) - 1, pos_y, (pos_y + building_width) - 1));
            if (b_type == 2)
                buildingList.push_back(Wasserkraftwerk(pos_x, (pos_x + building_length) - 1, pos_y, (pos_y + building_width) - 1));
            if (b_type == 3)
                buildingList.push_back(Solarpanele(pos_x, (pos_x + building_length) - 1, pos_y, (pos_y + building_width) - 1));

            // Kennzahl des Bauplans aktualisieren
            setKennzahl();
        }
    
    else {
        cout << "Gebaeude konnte nicht eingefuegt werden!" << endl;
    }

}
// Bauflaeche freigeben
void Blueprint::deleteArea(Building** object_blueprint) {
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
            //Wenn Gebaeude vollständig gelöscht, dann aus building_list entfernen
            reduceBuildingList(delete_x1, delete_x2, delete_y1, delete_y2);
            //Löschen jedes angegebenen Feldes im Building Array
            int count_deletedWiKra = 0;
            int count_deletedWaKra = 0;
            int count_deletedSolar = 0;
            for (int i = delete_y1; i <= delete_y2; i++) {
                for (int j = delete_x1; j <= delete_x2; j++) {
                    if (obj_blueprint[i][j].getLabel() != "Leer") {
                        if (obj_blueprint[i][j].getLabel() != "WiKra")
                            obj_blueprint[i][j].removeRessources();
                        if (obj_blueprint[i][j].getLabel() != "WaKra")
                            obj_blueprint[i][j].removeRessources();
                        if (obj_blueprint[i][j].getLabel() != "Solar")
                            obj_blueprint[i][j].removeRessources();

                        obj_blueprint[i][j].deleteBuilding();

                    }
                }
            
            }
            //Löschen der Fläche im Blueprint 
            setKennzahl();

        }
        catch (const char* txtException) {
            cout << "\nError: " << txtException << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

}
// Ausgabe Bauplan
void Blueprint::print_blueprint(Building** object_bluepr) {
    // Ausgabe des Bauplans
    for (int i = 0; i < this->getAreaWidth(); i++) {
        for (int j = 0; j < this->getAreaLength(); j++) {
            cout << obj_blueprint[i][j].label << "\t";
        }
        cout << endl;
    }

    // Ausgabe der Kennzahl eines Bauplans
    this->setKennzahl();
    cout << "Kennzahl: " << this->getKennzahl() << endl;

    // Ausgabe der Gebaeudeliste
    cout << "\nListe der gebauten Gebaeude: " << endl;
    for (int i = 0; i < buildingList.size(); i++) {
        buildingList[i].toString();

    }

    // Ausgabe des Gesamtpreises aller Gebaeude
    int result = 0;
    for (int i = 0; i < buildingList.size(); i++) {
        result += (buildingList[i].baseprice + buildingList[i].priceOfRessources);
    }
    cout << "\n Gesamtpreis aller Gebaeude: " << result << "\n" << endl;
   
}
// Programm beenden

// Getter
int Blueprint::getAreaLength() {
    return area_length;
}
int Blueprint::getAreaWidth() {
    return area_width;
}

double Blueprint::getKennzahl() {
    return this->kennzahl;
}
// Setter
void Blueprint::setKennzahl() {
    int sum_leistung = 0, sum_price = 0;
    for (auto i : this->buildingList) {
        sum_price += i.getFullPrice();
        sum_leistung += i.getLeistung();
    }
    if (this->buildingList.empty())
        this->kennzahl = 0;
    else
        this->kennzahl = (sum_leistung / (sum_price * (static_cast<double> (this->area_length * this->area_width))));
}
void Blueprint::setAreaLength(int length) {
    area_length = length;
}
void Blueprint::setAreaWidth(int width) {
    area_width = width;
}
void Blueprint::reduceBuildingList(int x1, int x2, int y1, int y2) {
    // Fall 1: Zu löschender Bereich umfasst gesamtes Gebaeude
    int index = 0;
    for (auto i : buildingList) {
        if (x1 <= i.flaeche.x1 && x2 >= i.flaeche.x2 && y1 <= i.flaeche.y1 && y2 >= i.flaeche.y2) {
            buildingList.erase(buildingList.begin() + index);
        }
            
        index++;


    }

}

//Funktor, Rückgabewert true falls der zu speichernde Plan mit einem bereits gespeichertem übereinstimmt
bool Blueprint::operator () (Blueprint& b1)  {
    if ((b1.getKennzahl() == getKennzahl()) && (b1.getAreaLength() == getAreaLength()) && (b1.getAreaWidth() == getAreaWidth()))
        return true;
    return false;
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
bool Material::operator< (const Material mat) const {
    if (price < mat.price)
        return true;
    return false;
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
    priceOfRessources = 0;
    this->leistung = 200;
    this->flaeche.setFlaeche(0, 0, 0, 0);

}
Windkraftwerk::Windkraftwerk() {
    label = "WiKra";
    baseprice = 1500;
    this->leistung = 300;
    this->flaeche.setFlaeche(0, 0, 0, 0);
    priceOfRessources = 0;

}
Wasserkraftwerk::Wasserkraftwerk() {
    label = "WaKra";
    baseprice = 2000;
    this->leistung = 500;
    this->flaeche.setFlaeche(0, 0, 0, 0);
    priceOfRessources = 0;

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
    flaeche.setFlaeche(x1, x2, y1, y2);
    priceOfRessources = 0;
    leistung = 200 * (this->flaeche.getLength() * this->flaeche.getWidth());
    req_wood = 1 * (this->flaeche.getLength() * this->flaeche.getWidth());
    req_met = 2 * (this->flaeche.getLength() * this->flaeche.getWidth());
    req_pla = 3 * (this->flaeche.getLength() * this->flaeche.getWidth());
    material.insert(make_pair(Holz(), req_wood));
    material.insert(make_pair(Metall(), req_met));
    material.insert(make_pair(Kunststoff(), req_pla));
    for (auto i = material.begin(); i != material.end(); i++)
        priceOfRessources += i->first.price * i->second;


}
Windkraftwerk::Windkraftwerk(int x1, int x2, int y1, int y2) {
    label = "WiKra";
    baseprice = 1500;
    this->flaeche.setFlaeche(x1, x2, y1, y2);
    priceOfRessources = 0;
    this->leistung = 300 * (this->flaeche.getLength() * this->flaeche.getWidth());
    req_wood = 3 * (this->flaeche.getLength() * this->flaeche.getWidth());
    req_met = 2 * (this->flaeche.getLength() * this->flaeche.getWidth());
    req_pla = 1 * (this->flaeche.getLength() * this->flaeche.getWidth());
    material.insert(make_pair(Holz(), req_wood));
    material.insert(make_pair(Metall(), req_met));
    material.insert(make_pair(Kunststoff(), req_pla));
    for (auto i = material.begin(); i != material.end(); i++)
        priceOfRessources += i->first.price * i->second;
}
Wasserkraftwerk::Wasserkraftwerk(int x1, int x2, int y1, int y2) {
    label = "WaKra";
    baseprice = 2000;
    this->flaeche.setFlaeche(x1, x2, y1, y2);
    priceOfRessources = 0;
    this->leistung = 500 * (this->flaeche.getLength() * this->flaeche.getWidth());
    req_wood = 2 * (this->flaeche.getLength() * this->flaeche.getWidth());
    req_met = 1 * (this->flaeche.getLength() * this->flaeche.getWidth());
    req_pla = 1 * (this->flaeche.getLength() * this->flaeche.getWidth());
    material.insert(make_pair(Holz(), req_wood));
    material.insert(make_pair(Metall(), req_met));
    material.insert(make_pair(Kunststoff(), req_pla));
    for (auto i = material.begin(); i != material.end(); i++)
        priceOfRessources += i->first.price * i->second;
}

// Methoden


// Label ausgeben
string Building::getLabel() {
    return label;
}

void Building::resToString() {
    cout << " Ressourcen: ";
    for (auto i = material.begin(); i != material.end(); i++)
        cout << i->first.name << " - " << i->second << ", ";
    cout << "\n" << endl;
}
// toString
void Building::toString() {
    ostringstream strout;
    string listRes = "";
    //string str(ressources.begin(), ressources.end());
    strout << " Gebaeudetyp: " << this->label << "\n Gebaeudepreis = " << (this->baseprice + this->priceOfRessources) << endl;
    cout << strout.str();
    this->resToString();

}
// Gebaeude abreissen
void Building::deleteBuilding() {
    label = "Leer";
    material.clear();
    baseprice = 0;
}
void Building::removeRessources() {

    int new_length = this->flaeche.getLength() - 1;
    this->material.clear();
    this->flaeche.setFlaeche(new_length, this->flaeche.getWidth());
    req_wood = req_wood * (new_length * this->flaeche.getWidth());
    req_met = req_met * (new_length) * this->flaeche.getWidth();
    req_pla = req_pla * (new_length) * this->flaeche.getWidth();
    material.insert(make_pair(Holz(), req_wood));
    material.insert(make_pair(Metall(), req_met));
    material.insert(make_pair(Kunststoff(), req_pla));
    for (auto i = material.begin(); i != material.end(); i++)
        priceOfRessources += i->first.price * i->second;

}
// Vollen Preis eines Gebaeudes zurückgeben
int Building::getFullPrice() {
    return this->baseprice + this->priceOfRessources;
}
// Leistung zurückgeben
int Building::getLeistung() {
    return this->leistung;
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
void Flaeche::setFlaeche(int length, int width) {
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



