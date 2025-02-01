// the_final_last_rest_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/**
*
* Solution to course project #1
* Intoduction to programming course
* Faculty of Mathematics and Informatics of Sofia
*University
* Winter semester 2024/2025
* @author Aleksandra Kirilova
* @idnumber 2MI0600448 @compiler VC
*
* solution file
*
*/

#include <iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

const string menuFile = "C:\\Users\\User\\Desktop\\restaurant_project\\menu.txt";
const string workdaysFile = "C:\\Users\\User\\Desktop\\restaurant_project\\workdays.txt";
const string storageFile = "C:\\Users\\User\\Desktop\\restaurant_project\\storage.txt";
const string profitsFile = "C:\\Users\\User\\Desktop\\restaurant_project\\profits.txt";

bool isValidForStoi(const string& str) {
	if (str.empty()) return false; 

	size_t start = 0;

	if (str[0] == '-') {
		if (str.size() == 1) return false;  
		start = 1;
	}

	for (size_t i = start; i < str.size(); ++i) {
		if (!isdigit(str[i])) {
			return false;  
		}
	}

	return true;  
}

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
	int day;
	int month;
	int year;
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

Date strToDate(string& strDate) {
	Date date;
	vector<string> elements = split(strDate, '.');

	date.day = stoi(elements[0]);
	date.month = stoi(elements[1]);
	date.year = stoi(elements[2]);
	return date;
}

//sec-first
bool isAfterDate(Date& first, Date& second) {
	if (first.year > second.year) { return true; }
	if (first.year < second.year) { return false; }
	if (first.month > second.month) { return true; }
	if (first.month < second.month) { return false; }
	return first.day > second.day;
}

void writeNewDateInFile(const string& workdays, Date& date) {
	ofstream ofs(workdays, ios::app);
	if (!ofs.is_open()) {
		cout << "error";
		return;
	}

	goToNextDay(date);

	ofs << (date.day < 10 ? "0" : "") << date.day <<
		"." << (date.month < 10 ? "0" : "") << date.month << "." << date.year << endl;
	ofs.close();
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

void viewFile(const string& fileName) {

	ifstream menu(fileName);

	if (!menu.is_open()) {
		cout << "error";
		return;
	}

	string line;
	while (std::getline(menu, line)) {
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

	while (std::getline(ifs, line1)) {
		std::getline(ifs, line2);
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
	while (std::getline(ifs, line)) {
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
	while (std::getline(ifs, line1)) {
		std::getline(ifs, line2);
		if (!std::getline(ifs, line3)) {
			isLastDish = true;
		}
		if (isLastDish != true) {
			std::getline(ifs, line4);
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

void removeMealFromVec(string& mealToRemove, vector<Meal>& meals) {
	Meal cur;
	for (int i = 0; i < meals.size(); i++) {
		cur = { meals[i].name, meals[i].ingredients, meals[i].price };

		if (cur.name == mealToRemove) {
			meals.erase(meals.begin() + i);
			break;
		}
	}
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
	while (std::getline(ifs, line)) {
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

void addStItemToFile(StorageItem& product, const string& fileName) {
	string linel;
	ofstream ofs(fileName, ios::app);
	if (!ofs.is_open()) {
		cout << "error";
		return;
	}

	ofs << toLowerCase(product.name) << "=" << product.amount << endl;
	ofs.close();
}

//adds a certain amount to a product in the collection!!!!!!!!!!!
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

void removeStItemFromFile(const string& fileName, string& productToRemove) {
	string line1, line2, line3, line4;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	bool isPresent = false;
	bool isLastProduct = false;
	vector<string> newFileLines;
	while (std::getline(ifs, line1)) {
		std::getline(ifs, line2);
		if (!std::getline(ifs, line3)) {
			isLastProduct = true;
		}
		if (isLastProduct != true) {
			std::getline(ifs, line4);
		}

		vector<string> ln1 = split(line1, '-');
		if (ln1[0] == productToRemove) {
			isPresent = true;
			continue;
		}

		newFileLines.push_back(line1);
		newFileLines.push_back(line2);

		if (isLastProduct != true) {
			newFileLines.push_back(line3);
			newFileLines.push_back(line4);
		}
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

void takeOrder(const string& workdays, vector<Meal>& meals, vector<StorageItem>& itemsInStock) {
	cout << "what is the order?" << endl;

	string item;
	std::getline(cin, item);
	if (!isItemInMenu(item, meals)) {
		cout << "no such dish in menu.";
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
		cout << "not enough ingredints.";
		return;
	}

	ofstream ofs(workdaysFile, ios::app);
	if (!ofs.is_open()) {
		cout << "error";
		return;
	}
	ofs << item << "-" << meal.price << endl;
	ofs.close();
}

bool isDigit(char c) {
	return (c < '0' || c > '9');
}

void cancelOrder(const string& fileName, string& orderToCancel) {
	vector<string> lines;
	string line;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	while (std::getline(ifs, line)) {
		lines.push_back(line);
	}

	ifs.close();

	bool isFound = false;
	bool haveGoneToPrevDay = false;
	string currLine;
	for (int i = lines.size() - 1; i >= 0; i--) {
		vector<string> words = split(lines[i], '-');
		for (char c : words[0]) {
			if (isDigit(c)) {
				haveGoneToPrevDay = true;
				isFound = false;
				break;
			}
		}
		if (haveGoneToPrevDay) {
			break;
		}

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

Date getLastDate(const string& fileName) {
	string line;
	Date date;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
	}

	while (std::getline(ifs, line)) {
		if (isDigit(line[0])) {
			vector<string> numbers = split(line, '.');
			
			date.day = stoi(numbers[0]);
			date.month = stoi(numbers[1]);
			date.year = stoi(numbers[2]);
		}
	}
	ifs.close();
	return date;
}

//sums the money earned from the date
int getSumOfProfitsFromDate(const string& fileName, string& date) {
	string line;
	int profit = 0;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
	}


	int priceOfMeal;
	while (std::getline(ifs, line)) {
		if (line == date) {
			while (std::getline(ifs, line)) {
				if (isDigit(line[0])) {
					break;
				}
				vector<string> words = split(line, '-');
				priceOfMeal = stoi(words[1]);
				profit += priceOfMeal;
			}
			break;
		}
	}
	ifs.close();
	return profit;
}

//reads the workdays file and sums up the money earned every day and writes it in a new file 
void fillProfitsFile(const string& workdaysF, const string& profitsF) {
	string line; 
	vector<int> profitsVec;
	vector<Date> datesVec;
	ifstream ifs(workdaysF);
	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	int priceOfMeal;
	Date date;
	bool isFirstDate = true;
	int profit = 0;
	while (std::getline(ifs, line)) {
		if (isdigit(line[0])) {

			if (!isFirstDate) {
				profitsVec.push_back(profit);
				datesVec.push_back(date);
			}
			isFirstDate = false;
			date = strToDate(line);
			profit = 0;
		}
		else {
			vector<string> words = split(line, '-');
			priceOfMeal = stoi(words[1]);
			profit += priceOfMeal;
		}
	}

	if (!isFirstDate) {
		profitsVec.push_back(profit);
		datesVec.push_back(date);
	}

	ifs.close();

	ofstream ofs(profitsF, ios::trunc);
	if (!ofs.is_open()) {
		cout << "error";
		return;
	}
	for (int i = 0; i < profitsVec.size(); i++) {
		ofs << dateToStr(datesVec[i]) << "-" << profitsVec[i] << endl;
	}
	ofs.close();
}

void listOfProfitsFromDate(const string& fileName, string& date) {
	string line;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	while (std::getline(ifs, line)) {
		string currDateStr = line.substr(0, 10);
		Date currD = strToDate(currDateStr);
		Date d = strToDate(date);
		if (currDateStr == date || isAfterDate(currD, d)) {
			cout << line << endl;
		}
	}
	ifs.close();
}

void sortAlpabetically(vector<string>& lines) {
	for (int i = 0; i < lines.size() - 1; i++) {
		for (int j = 0; j < lines.size() - i - 1; j++) {
			if (lines[j] > lines[j + 1]) {
				swap(lines[j], lines[j + 1]);
			}
		}
	}
}

void viewOrdersAlphabetically(const string& fileName) {
	vector<string> lines;
	string line;
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		cout << "error";
		return;
	}

	while (std::getline(ifs, line)) {
		if (isDigit(line[0])) {
			continue;
		}

		lines.push_back(line);
	}

	ifs.close();
	sortAlpabetically(lines);

	for (string s : lines) {
		cout << s << endl;
	}
}

bool isValidStr(string& str) {
	for (char ch : str) {
		if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))) {
			return false;
		}
	}
	return true;
}

bool isValidNumber(int num) {
	return num >= 1;
}

bool containsInSIVec(vector<StorageItem>& vec, string& str) {
	for (StorageItem si : vec) {
		if (si.name == str) {
			return true;
		}
	}
	return false;
}

bool isNumber(const string& str) {
	for (char c : str) {
		if (c < '0' || c > '9') return false;
	}
	return true;
}

bool isValidDate(const string& date) {
	if (date.length() != 10 || date[2] != '.' || date[5] != '.') {
		return false;
	}

	string dayStr = date.substr(0, 2);
	string monthStr = date.substr(3, 2);
	string yearStr = date.substr(6, 4);
	if (!isNumber(dayStr) || !isNumber(monthStr) || !isNumber(yearStr)) {
		return false;
	}

	int day = stoi(dayStr);
	int month = stoi(monthStr);
	int year = stoi(yearStr);
	if (month < 1 || month > 12 || year < 1) {
		return false;
	}

	int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	if (isLeap) {
		daysInMonth[1] = 29;
	}
	if (day < 1 || day > daysInMonth[month - 1]) {
		return false;
	}
	return true;
}

enum Command {
	VIEW_MENU, VIEW_STORAGE,
	VIEW_ORDERS, VIEW_ORDERS_ALPHABETICALLY,
	TAKE_ORDER, CANCEL_ORDER,
	REMOVE_PRODUCT, ADD_PRODUCT,
	ADD_DISH, REMOVE_DISH,
	GET_CURRENT_PROFIT, GET_LIST_OF_PROFITS_FROM_DATE,
	GET_DAILY_REPORT,
	UNKNOWN_COMMAND
};

Command getCommand(string& input) {
	if (input == "1") { return VIEW_MENU; }
	if (input == "2") { return TAKE_ORDER; }
	if (input == "3") { return CANCEL_ORDER; }
	if (input == "4") { return VIEW_ORDERS_ALPHABETICALLY; }
	if (input == "5") { return VIEW_ORDERS; }
	if (input == "6") { return VIEW_STORAGE; }
	if (input == "7") { return REMOVE_PRODUCT; }
	if (input == "8") { return ADD_PRODUCT; }
	if (input == "9") { return GET_CURRENT_PROFIT; }
	if (input == "10") { return GET_DAILY_REPORT; }
	if (input == "11") { return GET_LIST_OF_PROFITS_FROM_DATE; }
	if (input == "12") { return ADD_DISH; }
	if (input == "13") { return REMOVE_DISH; }
	return UNKNOWN_COMMAND;
}

void showServerOptions() {
	cout << "1. view menu" << endl << "2. take order" << endl << "3. cancel order" << endl << "4. view past orders"
		<< endl << "5. view past orders alpabetically" << endl << "6. view current profit of the day" << endl << endl;
}

void showManagerOptions() {
	cout << "1. view menu" << endl << "2. take order" << endl << "3. cancel order" << endl << "4. view past orders alphabetically"
		<< endl << "5. view past orders" << endl << "6. check amount of products in the storage"
		<< endl << "7. remove product from storage" << endl << "8. add product to storage"
		<< endl << "9. view current profit of the day" << endl << "10. get daily report"
		<< endl << "11. view reports from given date" << endl << "12. add a dish to the menu"
		<< endl << "13. remove a dish from the menu" << endl << endl;
}

void runProgramAsServer(Date& date, vector<Meal>& meals, vector<StorageItem> products) {
	cout << "here are all your options. what would you like to do? type the corresponding number" << endl;
	writeNewDateInFile(workdaysFile, date);
	string strCommand;
	std::getline(cin, strCommand);
	Command command;

	while (strCommand != "finish") {
		command = getCommand(strCommand);

		switch (command) {

			case VIEW_MENU:
				viewFile(menuFile);
				cout << "what's next?" << endl;
				break;

			case TAKE_ORDER:
				takeOrder(workdaysFile, meals, products);
				cout << "the order is taken. what's next?" << endl;
				break;

			case CANCEL_ORDER: {
					string orderToCancel;
					cin >> orderToCancel;
					if (!isValidStr(orderToCancel) || !isItemInMenu(orderToCancel, meals)) {
						cout << "invalid dish name, try again";
						break;
					}
					cancelOrder(workdaysFile, orderToCancel);
					cout << "order of " << orderToCancel << "canceled.what's next?" << endl;
					break;
				}

			case VIEW_ORDERS:
				viewFile(workdaysFile);
				cout << "what's next?" << endl;
				break;

			case VIEW_ORDERS_ALPHABETICALLY:
				viewOrdersAlphabetically(workdaysFile);
				cout << "orders dispayed alpabetically. what's next?" << endl;
				break;

			case GET_CURRENT_PROFIT: {
					Date date = getLastDate(workdaysFile);
					string todaysDate = dateToStr(date);
					cout << "the profit from today so far is "
						<< getSumOfProfitsFromDate(workdaysFile, todaysDate) << " lv. what's next?" << endl;
					break;
				};

			default:
				cout << "unknown command, try again" << endl;
				break;
		}

		std::getline(cin, strCommand);
	}
}

void runProgramAsManager(Date& date, vector<Meal>& meals, vector<StorageItem> products) {
	cout << "here are all your options. what would you like to do? type the corresponding number" << endl;
	writeNewDateInFile(workdaysFile, date);
	string strCommand;
	std::getline(cin, strCommand);
	Command command;

	while (strCommand != "finish") {
		command = getCommand(strCommand);

		switch (command) {

			case VIEW_MENU:
				viewFile(menuFile);
				cout << "what's next?" << endl;
				break;

			case TAKE_ORDER:
				takeOrder(workdaysFile, meals, products);
				cout << "the order is taken. what's next?" << endl;
				break;

			case CANCEL_ORDER: {
					string orderToCancel;
					cin >> orderToCancel;
					if (!isValidStr(orderToCancel) || !isItemInMenu(orderToCancel, meals)) {
						cout << "invalid dish name, try again";
						break;
					}
					cancelOrder(workdaysFile, orderToCancel);
					cout << "order canceled. what's next?" << endl;
					break;
				}

			case VIEW_ORDERS:
				viewFile(workdaysFile);
				cout << "what's next?" << endl;
				break;

			case VIEW_ORDERS_ALPHABETICALLY:
				viewOrdersAlphabetically(workdaysFile);
				cout << "orders dispayed alpabetically. what's next?" << endl;
				break;

			case VIEW_STORAGE:
				viewFile(storageFile);
				cout << "what's next?" << endl;
				break;

			case REMOVE_PRODUCT: {
					string product;
					cin >> product;
					if (!isValidStr(product) || !containsInSIVec(products, product)) {
						cout << "invalid input, try again" << endl;
						break;
					}
					removeStItemFromFile(storageFile, product);
					removeStItemFromVec(product, products);
					cout << "product removed from storage. what's next?" << endl;
					break;
				}

			case ADD_PRODUCT: {
					string product;
					int amount;
					cin >> product >> amount;
					if (!isValidNumber(amount) || !isValidStr(product)) {
						cout << "invalid input, try again" << endl;
						break;
					}
					if (containsInSIVec(products, product)) {
						addAmountToStItem(product, amount, products, storageFile);
						cout << "product added to the storage. what's next?" << endl;
						break;
					}
					StorageItem si = { product, amount };
					addStItemToFile(si, storageFile);
					products.push_back(si);
					cout << "product added to the storage. what's next?" << endl;
					break;
				}

			case GET_CURRENT_PROFIT: {
					Date date = getLastDate(workdaysFile);
					string todaysDate = dateToStr(date);
					cout << "the profit from today so far is " << getSumOfProfitsFromDate(workdaysFile, todaysDate)
						<< " lv. what's next?" << endl;
					break;
				}

			case GET_LIST_OF_PROFITS_FROM_DATE: {
					string date;
					cin >> date;
					if (!isValidDate(date)) {
						cout << "invalid date, try again";
						break;
					}
					cout << "the profits from" << date << " untill now are:" << endl;
					listOfProfitsFromDate(profitsFile, date);
					cout << "what's next?" << endl;
					break;
				}

			case GET_DAILY_REPORT: {
					fillProfitsFile(workdaysFile, profitsFile);
					Date date = getLastDate(workdaysFile);
					string todaysDate = dateToStr(date);
					int todaysProfit = getSumOfProfitsFromDate(workdaysFile, todaysDate);
					cout << "today's profit is " << todaysProfit << "lv. it has been written in the profits file."
						<< " what's next?" << endl;
					break;
				}

			case ADD_DISH: {
					Meal meal;
					string name;
					int price;
					vector<StorageItem> ingredients;
					cout << "enter the name of the dish, then its price" << endl;
					cin >> name >> price;
					cout << "now enter the ingredients and their amounts" << endl;
					string productName;
					cin >> productName;

					while (productName != "that's all") {
						StorageItem si;
						si.name = productName;
						int amount;
						cin >> amount;
						if (isValidNumber(amount) && isNumber(to_string(amount))) {
							si.amount = amount;
							ingredients.push_back(si);
						}

						cin >> productName;
					}

					meal.name = name;
					meal.price = price;
					meal.ingredients = ingredients;

					addDishToMenuFile(meal, menuFile);
					meals.push_back(meal);
					cout << "the dish was added to the menu. what's next?" << endl;
					break;
				}

			case REMOVE_DISH: {
					string dishName;
					cin >> dishName;
					if (!isValidStr(dishName) || !isItemInMenu(dishName, meals)) {
						cout << "invalid dish name, try again";
						break;
					}
					removeDishFromMenuFile(menuFile, dishName);
					removeMealFromVec(dishName, meals);
					cout << "the dish was removed from the menu. what's next?" << endl;
					break;
				}

			default:
				cout << "unknown command, try again" << endl;
				break;
		}

		std::getline(cin, strCommand);
	}
}

int main() {
	cout << "are you the manager or a server?" << endl;
	string person;
	cin >> person;

	Date date = getLastDate(workdaysFile);
	vector<Meal> meals = createMenuVec(menuFile);
	vector<StorageItem> products = createStorageVec(storageFile);

	while (person != "server" || person != "manager") {
		cout << "invalid input. please type \"manager\" or \"server\"." << endl;
		cin >> person;
	}

	if (person == "server") {
		runProgramAsServer(date, meals, products);
	}
	else if (person == "manager") {
		runProgramAsManager(date, meals, products);
	}

	return 0;
}


