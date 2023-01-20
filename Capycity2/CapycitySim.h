#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <map>
using namespace std;

// Materialverwaltung

class Material {
public:
	int price;
	string name;
	Material();
	string toString();
	bool operator< (const Material mat) const;
};

class Holz : public Material {
public:
	Holz();
	string toString();
};

class Metall : public Material {
public:
	Metall();
	string toString();
};

class Kunststoff : public Material {
public:
	Kunststoff();
	string toString();
};


// Gebaeudeverwaltung

class Flaeche {
public:
	int x1, x2, y1, y2;
	int length, width;
	int area_sum;
	Flaeche();
	void setFlaeche(int x1, int x2, int y1, int y2);
	int getLength();
	int getWidth();

};


class Building {
public:
	string label;
	Flaeche flaeche;
	map<Material, int> material;
	int req_wood, req_met, req_pla;
	int baseprice;
	int priceOfRessources;
	int leistung;
	Building(int x1, int x2, int y1, int y2);
	Building();
	string getLabel();
	void toString();
	void resToString();
	void deleteBuilding();
	void removeRessources();
	int getFullPrice();
	int getLeistung();
	
	
};

class Solarpanele : public Building {
public:
	Solarpanele();
	Solarpanele(int x1, int x2, int y1, int y2);
};

class Windkraftwerk : public Building {
public:
	Windkraftwerk();
	Windkraftwerk(int x1, int x2, int y1, int y2);
};

class Wasserkraftwerk : public Building {
public:
	Wasserkraftwerk();
	Wasserkraftwerk(int x1, int x2, int y1, int y2);
};

//------------------------------------------------Blueprint------------------------------------------------------------

class Blueprint {
public:
	int area_width, area_length;
	float kennzahl;
	vector<Building> buildingList;
	// Konstruktor:
	Blueprint();
	Blueprint(int area_l, int area_w);
	// Funktionen:
	bool collision(Building** bluepr, int pos_x, int pos_y, int building_width, int building_length);
	void setBuilding(Building** bluepr);
	void deleteArea(Building** bluepr);
	void print_blueprint(Building** bluepr);
	// Getter und Setter:
	int getAreaLength();
	int getAreaWidth();
	float getKennzahl();
	void setKennzahl();
	void setAreaLength(int length);
	void setAreaWidth(int width);
	//Funktor
	bool operator () (Blueprint& b1);
	void reduceBuildingList(int x1, int x2, int y1, int y2);
};

//----------------------------------------------------------- CapycitySim-----------------------------------------------------------------------
class CapycitySim {
public:
	vector<Blueprint> blueprint_list;
	vector<Building**> obj_blueprint_list;
	// Konstruktor:
	CapycitySim();
	CapycitySim(int area_l, int area_w);
	void menu(CapycitySim& sim);
	void endProgram();
	Building** create_object_blueprint(Blueprint& bluepr);
	Blueprint create_blueprint();
	bool check_if_unique(Blueprint& bluepr);
	void save(Blueprint& bluepr, Building** obj_bluepr);
	void print_all_blueprints();

};

