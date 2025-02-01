# restaurant_project_2025
Restaurant Management System

Overview

This is a C++ project for restaurant managment. The system uses files to store the information and provides different functionalities for servers and managers, including order management, menu updates, storage tracking, and financial reports.

Server Functions:

View menu

Take order

Cancel order

View past orders

View past orders alphabetically

View current profit of the day

Manager Functions:

View menu

Take order

Cancel order

View past orders alphabetically

View past orders

Check amount of products in storage

Remove product from storage

Add product to storage

View current profit of the day

Get daily report

View reports from a given date

Add a dish to the menu

Remove a dish from the menu

In the beginning the user chooses thir role ("server" or "manager"), then follows the instructions, 
provided from the program.

File info:

menu.txt: Stores the menu items. It's witten in the following format
"MENU

name-price

ingredient-amount in grams|ingredient-amount in grams|ingredient-amount in grams



name-price

ingredient-amount in grams|ingredient-amount in grams



name-price

ingredient-amount in grams|ingredient-amount in grams|ingredient-amount in grams|ingredient-amount in grams"

(two lines between different items, no empty line betweem name and ingredients)

workdays.txt: Keeps track of daily orders. It's witten in the following format:
"date

order-price

order-price

date

order-price"
(no empty lines inbetween)
storage.txt: Stores amounts of the products in grams (only aggs and tortillas are as count) . It's witten in the following format:
"product=amount

product=amount"
(no empty lines inbetween)
profits.txt: Stores profit reports for different days. It's witten in the following format:
"date-profit"
(no empty lines inbetween)
