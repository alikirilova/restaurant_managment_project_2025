// finalRestaurantPr.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

const string menuFile = "C:\\Users\\User\\Desktop\\restaurant_project\\menu.txt";
const string workdaysFile = "C:\\Users\\User\\Desktop\\restaurant_project\\workdays.txt";
const string storageFile = "C:\\Users\\User\\Desktop\\restaurant_project\\storage.txt";

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
};

bool isLeapYear(Date& date) {
	return (date.year % 100 != 0 && date.year % 4 == 0) || (date.year % 400 == 0);
}

int getDaysInMonth(Date& date) {
	switch (date.month) {
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		case 2:
			return isLeapYear(date) ? 29 : 28;
		default: return 31;
	}
}

void goToNextDay(Date& date) {
	date.day++;
	if (date.day > getDaysInMonth(date)) {
		date.day = 1;
		date.month++;
		if (date.month > 12) {
			date.month = 1;
			date.year++;
		}
	}
}

string dateToStr(Date& date) {
	string str = (date.day < 10 ? "0" : "") + to_string(date.day) + "." +
		(date.month < 10 ? "0" : "") + to_string(date.month) + "." + to_string(date.year);
	return str;
}

void newDate(const string& workdays, Date& date) {
	ofstream workFile(workdays, ios::app);
	goToNextDay(date);

	workFile << (date.day < 10 ? "0" : "") << date.day <<
		"." << (date.month < 10 ? "0" : "") << date.month << "." << date.year << endl;
	workFile.close();
}

struct StorageItem {
	string name;
	int amount;
};

struct Meal {
	string name;
	vector<StorageItem> ingredients;
	int price;
};

void viewMenu(const string& fileName) {

	ifstream menu(fileName);

	if (!menu.is_open()) {
		cout << "error";
		return;
	}

	string line;
	while (getline(menu, line)) {
		cout << line << endl;
	}
	menu.close();
}

vector<Meal> createMenuVec(const string& fileName) {
	ifstream ifs(fileName);

	if (!ifs.is_open()) {
		cout << "error";
		return {};
	}

	string line1, line2;
	vector<Meal> meals;

	while (getline(ifs, line1)) {
		getline(ifs, line2);
		if (line1.empty() || line2.empty() || line1 == "MENU") {
			continue;
		}

		Meal meal;
		vector<string> ln1 = split(line1, '-');
		meal.name = ln1[0];
		meal.price = stoi(ln1[1]);

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
	ifs.close();
	return meals;
}

vector<StorageItem> createStorageVec(const string& fileName) {
	ifstream ifs(fileName);

	if (!ifs.is_open()) {
		cout << "error";
		return {};
	}

	string line;
	vector<StorageItem> items;
	while (getline(ifs, line)) {
		vector<string> ln = split(line, '=');
		StorageItem item;
		item.name = ln[0];
		item.amount = stoi(ln[1]);
		items.push_back(item);
	}
	ifs.close();
	return items;
}

string toLowerCase(string& word) {
	for (char& c : word) {
		c = tolower(c);
	}
	return word;
}

void addDishToMenuFile(Meal meal, const string& fileName) {
	string linel;
	ofstream ofs(fileName, ios::app);
	if (!ofs.is_open()) {
		cout << "error";
		return;
	}

	ofs << endl << endl << toLowerCase(meal.name) << "-" << meal.price << endl;

	for (int i = 0; i < meal.ingredients.size(); i++) {
		StorageItem si = meal.ingredients[i];
		if (i != (meal.ingredients.size() - 1)) {
			ofs << si.name << "-" << si.amount << "|";
		}
		else {
			ofs << si.name << "-" << si.amount;
		}
	}
	ofs.close();
}

void removeDishFromMenuFile(const string& fileName, string dishToRemove) {
	string line1, line2, line3, line4;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	bool isPresent = false;
	bool isLastDish = false;
	vector<string> newFileLines;
	while (getline(ifs, line1)) {
		getline(ifs, line2);
		if (!getline(ifs, line3)) {
			isLastDish = true;
		}
		if (isLastDish != true) {
			getline(ifs, line4);
		}

		vector<string> ln1 = split(line1, '-');
		if (ln1[0] == dishToRemove) {
			isPresent = true;
			continue;
		}

		newFileLines.push_back(line1);
		newFileLines.push_back(line2);

		if (isLastDish != true) {
			newFileLines.push_back(line3);
			newFileLines.push_back(line4);
		}
	}
	ifs.close();

	if (isPresent == false) {
		cout << "No such dish found" << endl;
		return;
	}

	ofstream ofs(fileName, ios::trunc);
	if (!ofs.is_open()) {
		cout << "error";
		return;
	}

	for (int i = 0; i < newFileLines.size(); i++) {
		ofs << newFileLines[i] << endl;
	}
	ofs.close();
}

void viewStorage(const string& fileName) {
	ifstream ifs(fileName);

	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	string line;
	while (getline(ifs, line)) {
		cout << line << endl;
	}
	ifs.close();
}

//returns the number of lines(products) in the storage file
int ingredientsCount(const string& fileName) {
	ifstream ifs(fileName);

	if (!ifs.is_open()) {
		cout << "error";
		return 1;
	}

	string line;
	int count = 0;
	while (getline(ifs, line)) {
		count++;
	}
	ifs.close();
	return count;
}

//updates the amount of a product in the storage file
void updateStItemAmountInFile(const string& fileName, string product, string amount) {
	string line;
	string updatedLine;
	vector<string> newFileLines;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	bool isPresent = false;
	while (getline(ifs, line)) {
		vector<string> lineVec = split(line, '=');
		if (lineVec[0] == product) {
			lineVec[1] = amount;
			isPresent = true;
		}
		updatedLine = lineVec[0] + "=" + lineVec[1];
		newFileLines.push_back(updatedLine);
	}
	ifs.close();

	if (isPresent == false) {
		cout << "No such product found" << endl;
		return;
	}

	ofstream ofs(fileName, ios::trunc);
	if (!ofs.is_open()) {
		cout << "error";
		return;
	}

	for (int i = 0; i < newFileLines.size(); i++) {
		ofs << newFileLines[i] << endl;
	}

	ofs.close();
}

//adds a certain amount to a product in the collection
void addAmountToStItem(string itemToAddTo, int amountToAdd, vector<StorageItem>& items, const string& fileName) {
	StorageItem cur;
	for (int i = 0; i < items.size(); i++) {
		cur = { items[i].name, items[i].amount };
		if (items[i].name == itemToAddTo) {
			items[i].amount += amountToAdd;
			updateStItemAmountInFile(fileName, itemToAddTo, to_string(items[i].amount));
			break;
		}
	}
}

//removes a certain amount of a product from the collection
void removeAmountFromStItem(string itemToRemoveFrom, int amountToRemove, vector<StorageItem>& items, const string& fileName) {
	for (int i = 0; i < items.size(); i++) {
		if (items[i].name == itemToRemoveFrom) {

			if (amountToRemove > items[i].amount) {
				cout << "not enough " << items[i].name;
				break;
			}
			else {
				items[i].amount -= amountToRemove;
				updateStItemAmountInFile(storageFile, itemToRemoveFrom, to_string(items[i].amount));
				break;
			}
		}
	}
}

//removes a desired product from the collection
void removeStItemFromVec(string productToRemove, vector<StorageItem>& items) {
	StorageItem cur;
	for (int i = 0; i < items.size(); i++) {
		cur = { items[i].name, items[i].amount };

		if (cur.name == productToRemove) {
			items.erase(items.begin() + i);
			break;
		}
	}
}

//removes a desired meal from the collection
void removeMealFromVec(string mealToRemove, vector<Meal>& meals) {
	Meal cur;
	for (int i = 0; i < meals.size(); i++) {
		cur = { meals[i].name, meals[i].ingredients, meals[i].price };

		if (cur.name == mealToRemove) {
			meals.erase(meals.begin() + i);
			break;
		}
	}
}

bool isItemInMenu(string& name, vector<Meal>& meals) {
	for (Meal m : meals) {
		if (m.name == name) {
			return true;
		}
	}
}

bool areProductsInStock(Meal& meal, vector<StorageItem>& itemsInStock) {
	vector<StorageItem> neededIngredients = meal.ingredients;
	for (StorageItem si1 : itemsInStock) {
		for (StorageItem si2 : neededIngredients) {
			if (si1.name == si2.name) {
				if (si1.amount < si2.amount) {
					return false;
				}
			}
		}
	}
	return true;
}

void takeOrder(const string& workdays, vector<Meal>& meals, vector<StorageItem>& itemsInStock, Date& date) {
	cout << "What would you like to order?" << endl;

	string item;
	getline(cin, item);
	if (!isItemInMenu(item, meals)) {
		cout << "No such dish in menu.";
		return;
	}

	Meal meal;
	for (Meal m : meals) {
		if (m.name == item) {
			meal = m;
			break;
		}
	}

	if (areProductsInStock(meal, itemsInStock)) {
		for (StorageItem si : meal.ingredients) {
			removeAmountFromStItem(si.name, si.amount, itemsInStock, storageFile);
		}
	}
	else {
		cout << "Not enough ingredints.";
		return;
	}

	ofstream ofs(workdaysFile, ios::app);
	if (!ofs.is_open()) {
		cout << "error";
		return;
	}
	ofs << item << "-" << meal.price << "-" << dateToStr(date) << endl;
	ofs.close();
}

void cancelOrder(const string& fileName, string& orderToCancel) {
	vector<string> lines;
	string line;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	while (getline(ifs, line)) {
		lines.push_back(line);
	}
	ifs.close();

	bool isFound = false;
	string currLine;
	for (int i = lines.size() - 1; i >= 0; i--) {
		vector<string> words = split(lines[i], '-');
		if (words[0] == orderToCancel) {
			lines.erase(lines.begin() + i);
			isFound = true;
		}
	}

	if (!isFound) {
		cout << "order not found" << endl;
		return;
	}

	ofstream ofs(fileName);
	if (!ofs.is_open()) {
		cout << "error";
		return;
	}
	for (int i = 0; i < lines.size(); i++) {
		ofs << lines[i] << endl;
	}
	ofs.close();
}

int getProfitsFromDate(const string& fileName, string& date) {
	string line;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	int profit = 0;
	int priceOfMeal;
	while (std::getline(ifs, line)) {
		if (line == date) {
			while (std::getline(ifs, line)) {
				if (isDigit(line[0])) {
					continue;
				}
				vector<string> words = split(line, '-');
				priceOfMeal = stoi(words[1]);
				profit += priceOfMeal;
			}
			break;
		}
	}
	return profit;
}



enum Command {
	ORDER,
	VIEW_MENU,
	CANCEL_ORDER,
	VIEW_ALPHABETICALLY,
	UNKNOWN_COMMAND
};

Command getCommand(string& input) {
	if (input == "order") {
		return ORDER;
	}
	else if (input == "view menu") {
		return VIEW_MENU;
	}
	else if (input == "cancel order") {
		return CANCEL_ORDER;
	}
	else if (input == "view orders alphabetically") {
		return VIEW_ALPHABETICALLY;
	}
	return UNKNOWN_COMMAND;
}

int main() {
	Date date;
	vector<Meal> meals = createMenuVec(menuFile);
	vector<StorageItem> products = createStorageVec(storageFile);

	newDate(workdaysFile, date);
	cout << "What would you like to do?" << endl;
	string strCommand;
	getline(cin, strCommand);
	Command command;

	while (strCommand != "finish") {
		command = getCommand(strCommand);
		switch (command) {
			case ORDER:
				takeOrder(workdaysFile, meals, products, date);
				cout << "order taken" << endl;
				break;
			case VIEW_MENU:
				viewMenu(menuFile);
				break;
			case CANCEL_ORDER: {
					string orderToCancel;
					cin >> orderToCancel;
					cancelOrder(workdaysFile, orderToCancel);
					cout << "order canceled" << endl;
					break;
				}
			case VIEW_ALPHABETICALLY:
				break;
			default:
				cout << "unknown command, try again" << endl;
				break;
		}
		getline(cin, strCommand);
	}

	return 0;
}


