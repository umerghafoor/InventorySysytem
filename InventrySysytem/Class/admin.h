#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include <sstream>
#include <vector>
#include"user.h"
#include"../comman/constants.h"

struct Log
{
	float salePrice;
	float price;
	int quantity;
	std::string item;
	int day;
	int month;
	int year;
};

class Admin :private user 
{
	//private data
	std::vector<user> allUser;
	//private funtions
	std::vector<Log> returnReport();
	std::vector<user> returnAllUser();
public:
	Admin(int, std::string);
	Admin(int, std::string, std::string, std::string);

	//Customer managment
	bool deleteCustomer(int);
	bool markSpecial(int,bool);

	//Inventory mangment
	bool deleteItem(int);
	bool reduceItemQuantity(int, int);

	//Report managment
	bool SaleLog(int&, float&, float&, float&);
	bool PurchaseLog(int&, float&, float&);
};

Admin::Admin(int _ID, std::string password)
{
	this->ID = _ID;

	logedIn = false;
	std::ifstream adminDataBase(adminDataBaseFile);
	std::string _id, _name, _email, _password;
	std::string line;
	while (std::getline(adminDataBase, line))
	{
		std::stringstream ss(line);
		std::getline(ss, _id, ',');
		std::getline(ss, _name, ',');
		std::getline(ss, _email, ',');
		std::getline(ss, _password, ',');

		int id = std::stoi(_id);
		if (id == _ID && checkID(_password, password))
		{
			ID = id;
			name = _name;
			email = _email;
			password = _password;
			logedIn = true;
			break;
		}
	}
	adminDataBase.close();
	if (!logedIn)
	{
		ID = -1;
		name = '\0';
		email = '\0';
		password = '\0';
	}
}
Admin::Admin(int _ID, std::string _name, std::string _email, std::string _password)
{
	this->ID = _ID;
	this->name = _name;
	this->email = _email;
	this->password = _password;

	std::ifstream adminDataBase(adminDataBaseFile);
	std::string line;
	bool adminExists = false;
	while (std::getline(adminDataBase, line))
	{
		std::stringstream ss(line);
		std::string _ID;
		std::getline(ss, _ID, ',');

		int existingID = std::stoi(_ID);
		if (existingID == ID)
		{
			adminExists = true;
			break;
		}
	}
	adminDataBase.close();

	if (!adminExists)
	{
		std::ofstream adminDataBase(adminDataBaseFile, std::ios::app);
		adminDataBase << ID << "," << name << "," << email << "," << password << "\n";
		adminDataBase.close();
	}
	logedIn = false;
}

std::vector<Log> Admin::returnReport()
{
	if (logedIn)
	{
		std::vector<Log> allLog;
		Log _log;
		std::string _price, _salePrice, _quantity, _item, _buffer, _day, _month, _year;
		std::ifstream log(logDataBaseFile);
		std::string line;
		while (std::getline(log, line))
		{
			std::stringstream ss(line);

			std::getline(ss, _salePrice, ',');
			std::getline(ss, _price, ',');
			std::getline(ss, _quantity, ',');

			std::getline(ss, _item, ',');
			std::getline(ss, _buffer, ',');
			std::getline(ss, _buffer, ',');

			std::getline(ss, _day, '/');
			std::getline(ss, _month, '/');
			std::getline(ss, _year, ' ');

			_log.price = std::stof(_price);
			_log.salePrice = std::stof(_salePrice);
			_log.quantity = std::stoi(_quantity);

			_log.item = _item;
			_log.day = std::stoi(_day);
			_log.month = std::stoi(_month);
			_log.year = std::stoi(_year);

			allLog.push_back(_log);
		}
		log.close();
		return allLog;
	}
}
std::vector<user> Admin::returnAllUser()
{
	if (logedIn)
	{
		std::vector<user> allUser;
		user *temp;
		std::string _ID, _name, _email, _password, _specialUser;
		std::ifstream customer(customerDataBaseFile);
		std::string line;
		while (std::getline(customer, line))
		{
			std::stringstream ss(line);

			std::getline(ss, _ID, ',');
			std::getline(ss, _name, ',');
			std::getline(ss, _email, ',');
			std::getline(ss, _password, ',');

			int ID = stoi(_ID);
			bool specialUser = _specialUser == "1";
			temp = new user(ID, _name, _email, _password, specialUser);
			allUser.push_back(*temp);
		}
		customer.close();
		return allUser;
	}
}

bool Admin::deleteCustomer(int deleteID)
{
	if (logedIn)
	{
		std::ofstream tempCustomerDataBase(tempDataBaseFile);

		std::ifstream customerDataBase(customerDataBaseFile);
		std::string line;
		while (getline(customerDataBase, line))
		{
			std::istringstream ss(line);
			std::string id;
			getline(ss, id, ',');

			if (stoi(id) == deleteID)
			{
				continue;
			}
			else
			{
				tempCustomerDataBase << line << "\n";
			}
		}
		customerDataBase.close();
		tempCustomerDataBase.close();

		std::remove(customerDataBaseFile.c_str());
		std::rename(tempDataBaseFile.c_str(), customerDataBaseFile.c_str());
		return true;
	}
	else
	{
		return false;
	}
}
bool Admin::markSpecial(int markID,bool special=true)
{
	if (logedIn)
	{
		std::ofstream tempCustomerDataBase(tempDataBaseFile);
		std::ifstream customerDataBase(customerDataBaseFile);

		std::string line;
		while (getline(customerDataBase, line))
		{
			std::istringstream ss(line);
			std::string id;
			getline(ss, id, ',');

			if (stoi(id) == markID)
			{
				std::string name, email, password;
				getline(ss, name, ',');
				getline(ss, email, ',');
				getline(ss, password, ',');
				tempCustomerDataBase << ID << "," << name << "," << email << "," << password << ',' << special << '\n';
				continue;
			}
			else
			{
				tempCustomerDataBase << line << "\n";
			}
		}
		customerDataBase.close();
		tempCustomerDataBase.close();

		std::remove(customerDataBaseFile.c_str());
		std::rename(tempDataBaseFile.c_str(), customerDataBaseFile.c_str());
		return true;
	}
	else
	{
		return false;
	}
}

bool Admin::deleteItem(int deleteID) 
{
	if (logedIn)
	{
		std::ifstream itemDataBase(itemDataBaseFile);
		std::ofstream tempDataBase(tempDataBaseFile);

		std::string line;
		bool itemDeleted = false;

		while (std::getline(itemDataBase, line)) {
			std::stringstream ss(line);
			std::string _ID;

			std::getline(ss, _ID, ',');
			int id = std::stoi(_ID);

			if (id == deleteID) 
			{
				itemDeleted = true;
			}
			else 
			{
				tempDataBase << line << "\n";
			}
		}
		itemDataBase.close();
		tempDataBase.close();

		if (!itemDeleted) 
		{
			return false;
		}
		std::remove(itemDataBaseFile.c_str());
		std::rename(tempDataBaseFile.c_str(), itemDataBaseFile.c_str());
		return true;
	}
	return false;
}
bool Admin::reduceItemQuantity(int itemNo, int changeQuantity)
{
	if (logedIn && changeQuantity<0)
	{
		std::ifstream itemDataBase(itemDataBaseFile);
		std::ofstream tempDataBase(tempDataBaseFile);

		std::string line;
		while (std::getline(itemDataBase, line))
		{
			std::stringstream ss(line);
			std::string _itemNo, _itemName, _price, _salePrice, _quantity, _category;
			std::getline(ss, _itemNo, ',');
			std::getline(ss, _itemName, ',');
			std::getline(ss, _price, ',');
			std::getline(ss, _quantity, ',');
			std::getline(ss, _category, ',');
			std::getline(ss, _salePrice, ',');

			int dbItemNo = std::stoi(_itemNo);

			if (dbItemNo == itemNo)
			{
				int dbQuantity = std::stoi(_quantity);
				dbQuantity += changeQuantity;
				tempDataBase << dbItemNo << "," << _itemName << "," << _price << "," << dbQuantity << ',' << _category << ',' << _salePrice << '\n';
			}
			else
			{
				tempDataBase << line << "\n";
			}
		}
		itemDataBase.close();
		tempDataBase.close();
		std::remove(itemDataBaseFile.c_str());
		std::rename(tempDataBaseFile.c_str(), itemDataBaseFile.c_str());
		return true;
	}
	return false;
}

bool Admin::SaleLog(int& totalItemSold,float& totalRevenue,float& totalCost,float& profit)
{
	if (logedIn)
	{
		std::vector<Log> log = returnReport();
		int _totalItemSold = 0;
		float _totalRevenue = 0;
		float _totalCost = 0;
		for (int i = 0;i < log.size();i++)
		{
			if (log[i].price > 0)
			{
				_totalItemSold += log[i].quantity;
				_totalRevenue += (log[i].salePrice * log[i].quantity);
				_totalCost += (log[i].price * log[i].quantity);
			}
		}
		totalItemSold = _totalItemSold;
		totalRevenue = _totalRevenue;
		totalCost = _totalCost;
		profit = _totalRevenue - totalCost;
		return true;
	}
	return false;
}
bool Admin::PurchaseLog(int& totalItemPurchased, float& totalCost, float& netTotal)
{
	if (logedIn)
	{
		std::vector<Log> log = returnReport();
		int _totalItemPurchased = 0;
		float _totalCost = 0;
		float _netTotal = 0;
		for (int i = 0;i < log.size();i++)
		{
			if (log[i].price < 0)
			{
				_totalItemPurchased += log[i].quantity;
				_totalCost += (log[i].salePrice * log[i].quantity);
				_netTotal += -(log[i].price * log[i].quantity);
			}
		}
		totalItemPurchased = _totalItemPurchased;
		totalCost = _totalCost;
		netTotal = _netTotal;
		return true;
	}
	return false;
}