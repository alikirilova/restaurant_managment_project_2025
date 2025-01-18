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
		return {};
	}

	string line;
	vector<StorageItem> items;
	while (getline(stFile, line)) {
		vector<string> ln = split(line, '=');
		StorageItem item;
		item.name = ln[0];
		item.amount = stoi(ln[1]);
		items.push_back(item);
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

vector<Meal> createMenuVec(const string& fileName) {
	ifstream mFile(fileName);

	if (!mFile.is_open()) {
		cout << "error";
		return {};
	}

	string line1, line2;
	vector<Meal> meals;

	while (getline(mFile, line1)) {
		getline(mFile, line2);
		if (line1.empty() || line2.empty() || line1 == "MENU") {
			continue;
		} 

        Meal meal;
		vector<string> ln1 = split(line1, '-');
		meal.name = ln1[0];
		meal.price = stod(ln1[1]);

		vector<string> ln2 = split(line2, '|');
		vector<StorageItem> ingredients;

		for (int i = 0; i < ln2.size(); i++) {
			vector<string> cur = split(ln2[i], '-');

			StorageItem  ingredient;
			ingredient.name = cur[0];
			ingredient.amount = stoi(cur[1]);
			ingredients.push_back(ingredient);
		}
		meal.ingredients = ingredients;
		meals.push_back(meal);
	}
	mFile.close();
	return meals;
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

void updateProductAmount(const string& fileName, string product, string amount) {
	string line;
	string updatedLine;
	vector<string> fileLines;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	bool isPresent = false;
	while (getline(ifs, line)) {
		vector<string> words = split(line, '=');
		if (words[0] == product) {
			words[1] = amount;
			isPresent = true;
		}
		updatedLine = words[0] + "=" + words[1];
		fileLines.push_back(updatedLine);
	}
	ifs.close();

	if (isPresent == false) {
		cout << "No such product found" << endl;
		return;
	}

    ofstream ofs(fileName, ios::out);
    if (!ofs.is_open()) {
        cout << "error";
        ifs.close();
        return;
    }

	for (int i = 0; i < fileLines.size(); i++) {
			ofs << fileLines[i] << endl;
	   }

	ofs.close();
}

void addToItem(string itemToAddTo, int amountToAdd, vector<StorageItem>& items, const string& fileName) {
	StorageItem cur;
	for (int i = 0; i < items.size(); i++) {
		cur = { items[i].name, items[i].amount };
		if (items[i].name == itemToAddTo) {
			items[i].amount += amountToAdd;
			updateProductAmount(fileName, itemToAddTo, to_string(items[i].amount));
			break;
		}
	}
}
void removeFromItem(string itemToRemoveFrom, int amountToRemove, vector<StorageItem>& items, const string& fileName) {
	for (int i = 0; i < items.size(); i++) {
		if (items[i].name == itemToRemoveFrom) {

			if (amountToRemove > items[i].amount) {
				cout << "not enough " << items[i].name;
				break;
			}
			else {
				items[i].amount -= amountToRemove;
                updateProductAmount(storageFile, itemToRemoveFrom, to_string(items[i].amount));
				break;
			}
		}
	}
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




