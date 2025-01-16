// 2restaurant_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

const string menu = "C:\\Users\\User\\Desktop\\restaurant_project\\menu.txt";
const string workdays = "C:\\Users\\User\\Desktop\\restaurant_project\\workdays.txt";
const string storage = "C:\\Users\\User\\Desktop\\restaurant_project\\storage.txt";

vector<string> split(const string& str, char delimiter) {
	vector<string> result;
	string current;

	for (char ch : str) {
		if (ch == delimiter) {
			if (!current.empty()) {
				result.push_back(current);
				current.clear();
			}
		}
		else {
			current += ch;
		}
	}
	if (!current.empty()) {
		result.push_back(current);
	}
	return result;
}


struct Date {
	int day = 1;
	int month = 1;
	int year = 2025;

	bool isLeapYear() {
		return (year % 100 != 0 && year % 4 == 0) || (year % 400 == 0);
	}

	int getDaysInMonth() {
		switch (month) {
			case 4:
			case 6:
			case 9:
			case 11:
				return 30;
			case 2:
				return isLeapYear() ? 29 : 28;
			default: return 31;
		}
	}

	void goToNextDay() {
		day++;
		if (day > getDaysInMonth()) {
			day = 1;
			month++;
			if (month > 12) {
				month = 1;
				year++;
			}
		}
	}

	void printDate() {
		cout << (day < 10 ? "0" : "") << day << "." 
			<< (month < 10 ? "0" : "") << month << "." << year << endl;
	}
};

void newDate(const string& workdays) {
	ofstream workFile(workdays, ios::app);
	Date currDate;
	currDate.goToNextDay();

	workFile << (currDate.day < 10 ? "0" : "") << currDate.day <<
		"." << (currDate.month < 10 ? "0" : "") << currDate.month << "." << currDate.year << endl;
	workFile.close();
}

struct Dish {
	string name;
	vector<string> ingredients;
	double price;
};

struct StorageItem {
	string name;
	int amount;
};

vector<StorageItem> createStorageVec(const string& fileName) {
	ifstream stFile(fileName);

	if (!stFile.is_open()) {
		cout << "error";
	}

	string line;
	vector<StorageItem> items;
	int i = 0;
	while (getline(stFile, line)) {
		vector<string> ln = split(line, ' ');
		items[i].name = ln[0];
		items[i].amount = stoi(ln[1]);
		i++;
	}
	stFile.close();
	return items;
}

void takeOrder(const string& workdays) {
	cout << "What would you like to order?" << endl;

	string item;
	cin >> item;

	ofstream workFile(workdays, ios::app);

	workFile << item << endl;
	workFile.close();
}

void viewMenu(const string& fileName) {

	ifstream menu(fileName);

	if (!menu.is_open()) {
		cout << "error";
	}

	string line;
	while (getline(menu, line)) {
		cout << line << endl;
	}
	menu.close();
}

void viewStorage(const string& fileName) {
	ifstream stFile(fileName);

	if (!stFile.is_open()) {
		cout << "error";
	}

	string line;
	while (getline(stFile, line)) {
		cout << line << endl;
	}
	stFile.close();
}

int ingredientsCount(const string& fileName) {
	ifstream stFile(fileName);

	if (!stFile.is_open()) {
		cout << "error";
	}

	string line;
	int count = 0;
	while (getline(stFile, line)) {
		count++;
	}
	stFile.close();
	return count;
}

void addToStorage(string itemToAddTo, int amountToAdd, vector<StorageItem> items) {

	for (int i = 0; i < items.size(); i++) {

		if (items[i].name == itemToAddTo) {
			items[i].amount += amountToAdd;
			break;
		}
	}

	updateProductAmount(storageFile, itemToAddTo, to_string(amountToAdd), items);
}

void removeFromStorage(string itemToRemoveFrom, int amountToRemove, vector<StorageItem> items) {
	StorageItem cur;

	for (int i = 0; i < items.size(); i++) {
		cur = { items[i].name, items[i].amount };

		if (items[i].name == itemToRemoveFrom) {

			if (amountToRemove > cur.amount) {
				cur.amount = 0;
				break;
			}
			else {
				cur.amount -= amountToRemove;
				break;
			}
		}
	}
	updateProductAmount(storageFile, itemToRemoveFrom, to_string(amountToRemove), items);
}


void updateProductAmount(const string& fileName, string product, string amount, vector<StorageItem> items) {
	string line;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	ofstream ofs(fileName, ios::app);
	if (!ifs.is_open()) {
		cout << "error";
		ifs.close();
		return;
	}
	
	vector<string> words;
	while (getline(ifs, line)) {
		words = split(line, ' ');
		if (words[0] == product) {
			words[1] = amount;
			break;
		}
	}

	ifs.close();
	ofs.close();
}


int main() {

	Dish d[8] = {
		{"Nachos with Guacamole", {"nachos", "guacamole", "cheese"}, 11},
		{"Chicken Tacos", {"tortilla", "chicken", "iceberg lettuce", "tomato suace"}, 15},
		{"Chicken Enchiladas", {"tortilla", "chicken", "tomato sauce", "cheese"}, 18},
		{"Shrimp Fajitas", {"shrimp", "bell peppers", "onions", "tortilla", "avocado"}, 20},
		{"Churros with Chocolate Sauce", {"dough", "cinnamon sugar", "chocolate sauce"}, 11},
		{"Flan", {"eggs", "condensed milk", "caramel", "vanilla"}, 10},
		{"Margarita", {"tequila", "lime", "orange liqueur"}, 13},
		{"Horchata", {"rice milk", "cinnamon sugar", "vanilla"}, 7}
	};
	return 0;
}




