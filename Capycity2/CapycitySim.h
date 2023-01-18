#pragma once
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// Materialverwaltung

class Material {
public:
	int price;
	string name;
	Material();
	string toString();
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
	int req_wood, req_met, req_pla;
	int baseprice;
	int priceOfRessources;
	vector<Material> ressources;
	Building(int x1, int x2, int y1, int y2);
	Building();
	string getLabel();
	string toString();
	string resToString();
	void deleteBuilding();
	void removeRessources(vector<Building>& buildingList, Building& b);
	
	
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

// Capycity
class Capycity {
public:
	int area_width, area_length;
	vector<Building> buildingList;
	// Konstruktor:
	Capycity();
	Capycity(int area_l, int area_w);
	// Funktionen:
	bool collision(Building** bluepr, int pos_x, int pos_y, int building_width, int building_length);
	void setBuilding(Building** bluepr);
	void deleteArea(Building** bluepr);
	void print_blueprint(Building** bluepr);
	void endProgram();
	// Getter und Setter:
	int getAreaLength();
	int getAreaWidth();
	void setAreaLength(int length);
	void setAreaWidth(int width);
	void reduceBuildingList(int x1, int x2, int y1, int y2);
	vector<Building> updateBuildingList(Building** bluepr, vector<Building>& list);
};