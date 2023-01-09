/*
 * server.h
 *
 *  Created on: Jan 7, 2023
 *      Author: tanma
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include "expedia_flights_api.h"
#include "expedia_hotels_api.h"
#include "expedia_payments_api.h"
using namespace std;

long int get_unix(string date)
{
	int size = date.size();
	for (int i = 0; i < size; i++)
	{
		if (date[i] == '-')
		{
			date[i] = ' ';
		}
	}

	int year, month, day;

	istringstream iss(date);
	iss >> day >> month >> year;

	int days_year = 0;
	int days_month = 0;
	bool leap_factor = 0;;

	for (int i = 0; i < year; i++)
	{
		if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
		{
			days_year += 366;
		}
		else
		{
			days_year += 365;
		}
	}

	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
	{
		leap_factor = 1;
	}

	for (int i = 1; i < month; i++)
	{
		if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
		{
			days_month += 31;
		}
		else if (i == 2)
		{
			if (leap_factor)
			{
				days_month += 29;
			}
			else
			{
				days_month += 28;
			}
		}
		else
		{
			days_month += 30;
		}
	}

	long int days = days_year + days_month + day;

	return days;
}

long int get_diff(string date1, string date2)
{
	return get_unix(date1) - get_unix(date2);
}

class Card
{
protected:
	string number;
	string name;
	string address;
	string expiration_date;
	int cvv;
	bool debitcardyes;

public:
	Card(string number, string name, string address, string expiration_date, int cvv, bool debitcardyes):
		number(number), name(name), address(address), expiration_date(expiration_date), cvv(cvv), debitcardyes(debitcardyes)
	{

	}
	virtual bool make_payment(double money)
	{
		throw logic_error("This function shouldn't run. Please change the code accordingly.");
		return 0;
	}
	virtual Card* clone() const
	{
		throw logic_error("This function shouldn't run. Please change the code accordingly.");
		return nullptr;
	}
	bool get_debitcardyes() const
	{
		return debitcardyes;
	}
	string get_number() const
	{
		return number;
	}
	virtual ~Card()
	{

	}
};

class DebitCard: public Card
{
private:
	double account_balance;

public:
	DebitCard(string number, string name, string address, string expiration_date, int cvv, double account_balance, bool debitcardyes):
		Card(number, name, address, expiration_date, cvv, debitcardyes), account_balance(account_balance)
	{

	}
	virtual bool make_payment(double money)
	{
		if (money > account_balance)
			{
				cout << "Not enough money present\n";
				return 0;
			}

			int choice;
			do
			{
				cout << "1. PayPal\n2. Stripe\n3. Square\n\nEnter payment mode through which you want to pay (1, 2 or 3): ";
				cin >> choice;

				if (choice == 1)
				{
					PayPalOnlinePaymentAPI P1;
					PayPalCreditCard P(name, address, number, expiration_date, cvv);
					P1.SetCardInfo(&P);
					if(P1.MakePayment(money))
					{
						account_balance -= money;
					}
					return 1;
				}
				else if (choice == 2)
				{
					StripePaymentAPI S1;
					StripeUserInfo SU(name, address);
					StripeCardInfo SC(number, expiration_date);
					if (S1.WithDrawMoney(SU, SC, money))
					{
						account_balance -= money;
					}
					return 1;
				}
				else if (choice == 3)
				{
					SquarePaymentAPI SQ1;
					string JsonQuery = "{\"Name\":\"" + name + "\",\"Address\":\"" + address + "\",\"ID\":\"" + number + "\",\"Expiration Date\":\"" + expiration_date + "\",\"CVV\":" + to_string(cvv) + "}";
					if(SQ1.WithDrawMoney(JsonQuery))
					{
						account_balance -= money;
					}
					return 1;
				}
				else
				{
					cout << "Invalid Choice. Please try again.\n";
				}
			}while(true);
	}
	virtual Card* clone() const
	{
		return new DebitCard(*this);
	}
};

class CreditCard: public Card
{
private:
	double credit_limit;
	double interest;

public:
	CreditCard(string number, string name, string address, string expiration_date, int cvv, double credit_limit, double interest, bool debitcardyes):
		Card(number, name, address, expiration_date, cvv, debitcardyes), credit_limit(credit_limit), interest(interest)
	{

	}
	virtual bool make_payment(double money)
	{
		if (money > credit_limit)
		{
			cout << "Not enough money present\n";
			return 0;
		}

		int choice;
		do
		{
			cout << "1. PayPal\n2. Stripe\n3. Square\n\nEnter payment mode through which you want to pay (1, 2 or 3): ";
			cin >> choice;

			if (choice == 1)
			{
				PayPalOnlinePaymentAPI P1;
				PayPalCreditCard P(name, address, number, expiration_date, cvv);
				P1.SetCardInfo(&P);
				if(P1.MakePayment(money))
				{
					credit_limit -= money;
				}
				return 1;
			}
			else if (choice == 2)
			{
				StripePaymentAPI S1;
				StripeUserInfo SU(name, address);
				StripeCardInfo SC(number, expiration_date);
				if (S1.WithDrawMoney(SU, SC, money))
				{
					credit_limit -= money;
				}
				return 1;
			}
			else if (choice == 3)
			{
				SquarePaymentAPI SQ1;
				string JsonQuery = "[ { \"Name\" : " + name + " }, { \"Address\" : " + address + " }, { \"ID\" : " + number + " }, { \"Expiration Date\" : " + expiration_date + " }, { \"CVV\" : " + to_string(cvv) + " } ]";
				if(SQ1.WithDrawMoney(JsonQuery))
				{
					credit_limit -= money;
				}
				return 1;
			}
			else
			{
				cout << "Invalid Choice. Please try again.\n";
			}
		}while(true);
	}
	virtual Card* clone() const
	{
		return new CreditCard(*this);
	}
};

class Itinerary_Item
{
protected:
	string from_date;
	string to_date;
	int adults;
	int children;
	string name;
	string email;
	double total_cost_item;

public:
	Itinerary_Item(string from_date, string to_date, int adults, int children, string name, string email):
		from_date(from_date), to_date(to_date), adults(adults), children(children), name(name), email(email), total_cost_item(0)
	{

	}
	virtual Itinerary_Item* add()
	{
		throw logic_error("This function shouldn't run. Please change the code accordingly.");
		return nullptr;
	}
	virtual string get_details() const
	{
		throw logic_error("This function shouldn't run. Please change the code accordingly.");
		return "";
	}
	double get_cost() const
	{
		return total_cost_item;
	}
	virtual ~Itinerary_Item()
	{

	}
};

class Flight: public Itinerary_Item
{
private:
	string from;
	string to;
	int no_of_infants;
	string Flight_details;

public:
	Flight(string from_date, string to_date, int adults, int children, string name, string email, string from, string to, int no_of_infants):
	Itinerary_Item(from_date, to_date, adults, children, name, email), from(from), to(to), no_of_infants(no_of_infants)
	{

	}
	virtual Itinerary_Item* add()
	{
		AirCanadaOnlineAPI A1;
		vector<AirCanadaFlight> AF = A1.GetFlights();
		TurkishAirlinesOnlineAPI T1;
		vector<TurkishFlight> TF = T1.GetAvailableFlights();
		vector<int> index1;
		vector<int> index2;
		int tot = 0;
		int i = 0;
		int id = 0;
		for (auto a : AF)
		{
			if (a.date_time_from == from_date && a.date_time_to == to_date && a.from == from && a.to == to)
			{
				tot++;
				index1.push_back(id);
				i++;
				cout << tot << ". Airline: AirCanada Airlines Maximum Cost per ticket: " << a.price << " Departure Date: " << a.date_time_from << " Arrival Date: " << a.date_time_to << endl;
			}
			id++;
		}
		id = 0;
		int j = 0;
		for (auto t : TF)
		{
			if (t.datetime_from == from_date && t.datetime_to == to_date && t.from == from && t.to == to)
			{
				tot++;
				index2.push_back(id);
				j++;
				cout << tot << ". Airline: Turkish Airlines Macimum Cost per ticket: " << t.cost << " Departure Date: " << t.datetime_from << " Arrival Date: " << t.datetime_to << endl;
			}
			id++;
		}

		if (tot <= 0)
		{
			cout << "No flights found." << endl;
			return nullptr;
		}

		cout << endl;

		int choice;
		do
		{
			cout << "Enter -1 to cancel or number in range 1 - " << tot << ": ";
			cin >> choice;
			if (choice == -1)
			{
				return nullptr;
			}
			else if (choice < 1 || choice > tot)
			{
				cout << "Invalid choice. Please try again\n";
			}
			else
			{
				int diff;
				if (choice > i)
				{
					diff = index2[choice - i - 1];
					TurkishFlight t = TF[diff];
					TurkishCustomerInfo TC(name, email, adults, children, no_of_infants);
					if (T1.ReserveFlight(TC, t))
					{
						total_cost_item = TF[diff].cost * adults + 0.5 * TF[diff].cost * children + 0.25 * TF[diff].cost * no_of_infants;
						Flight_details =  "Airline reservation with reservation: Turkish Airlines: From " + TF[diff].from + " on " + TF[diff].datetime_from + "\n\tTo " + TF[diff].to + " on " + TF[diff].datetime_to
								+ "\n\tAdults: " + to_string(adults) + " Children: " + to_string(children) + " Infants: " + to_string(no_of_infants) + "\n\tTotal Cost: " + to_string(total_cost_item) + "\n\n";
					}
				}
				else
				{
					diff = index1[i - 1];
					AirCanadaFlight a = AF[diff];
					AirCanadaCustomerInfo AC(name, email, adults, children, no_of_infants);
					if (A1.ReserveFlight(a, AC))
					{
						total_cost_item = AF[diff].price * adults + 0.5 * AF[diff].price * children + 0.25 * AF[diff].price * no_of_infants;
						Flight_details =  "Airline reservation with reservation: AirCanada Airlines: From " + AF[diff].from + " on " + AF[diff].date_time_from + "\n\tTo " + AF[diff].to + " on " + AF[diff].date_time_to
								+ "\n\tAdults: " + to_string(adults) + " Children: " + to_string(children) + " Infants: " + to_string(no_of_infants) + "\n\tTotal Cost: " + to_string(total_cost_item) + "\n\n";
					}
				}
				return new Flight(*this);
			}
		}while(true);
	}
	virtual string get_details() const
	{
		return Flight_details;
	}
};

class Hotel: public Itinerary_Item
{
private:
	string country;
	string city;
	string hotel_details;
	int needed_rooms;

public:
	Hotel(string from_date, string to_date, int adults, int children, string name, string email, string country, string city, int needed_rooms):
	Itinerary_Item(from_date, to_date, adults, children, name, email), country(country), city(city), needed_rooms(needed_rooms)
	{

	}
	virtual Itinerary_Item* add()
	{
		HiltonHotelAPI H1;
		vector<HiltonRoom> HR = H1.SearchRooms();
		MarriottHotelAPI M1;
		vector<MarriottFoundRoom> MR = M1.FindRooms();
		int tot = 0;
		int i = 0;
		int id = 0;
		vector<int> index1;
		vector<int> index2;
		for (auto h : HR)
		{
			if (get_unix(h.date_from) <= get_unix(from_date) && get_unix(h.date_to) >= get_unix(to_date) && get_unix(from_date) < get_unix(to_date) && h.country == country && h.city == city && h.available >= needed_rooms)
			{
				tot++;
				index1.push_back(id);
				i++;
				cout << tot << ". Hotel: Hilton Hotel Room Type: " << h.room_type << "(" << h.available << ") Price per night: " << h.price_per_night << " From Date " << h.date_from << " to " << h.date_to << endl;
			}
			id++;
		}
		id = 0;
		int j = 0;
		for (auto m : MR)
		{
			if (get_unix(m.date_from) <= get_unix(from_date) && get_unix(m.date_to) >= get_unix(to_date) && get_unix(from_date) < get_unix(to_date) && m.country == country && m.city == city && m.available >= needed_rooms)
			{
				tot++;
				index2.push_back(id);
				j++;
				cout << tot << ". Hotel: Mariott Hotel Room Type: " << m.room_type << "(" << m.available << ") Price per night: " << m.price_per_night << " From Date " << m.date_from << " to " << m.date_to << endl;
			}
			id++;
		}
		cout << endl;

		if (tot == 0)
		{
			cout << "No hotels available.\n";
			return nullptr;
		}

		int choice;
		do
		{
			cout << "Enter -1 to cancel or number in range 1 - " << tot << ": ";
			cin >> choice;
			if (choice == -1)
			{
				return nullptr;
			}
			else if (choice < 1 || choice > tot)
			{
				cout << "Invalid choice. Please try again\n";
			}
			else
			{
				int diff;
				if (choice > i)
				{
					diff = index2[choice - i - 1];
					MarriottFoundRoom m = MR[diff];
					MarriottFoundRoomCustomerInfo MC(name, email, adults, children, needed_rooms);
					if (M1.ReserveRoom(m, MC))
					{
						total_cost_item = needed_rooms * m.price_per_night * get_diff(to_date, from_date);
						hotel_details =  "Hotel reservation: Marriott Hotel: " + m.city + " @ " + m.country + "\n\t" + from_date + " to " + to_date
								+ "\n\tAdults: " + to_string(adults) + " Children: " + to_string(children) + "\n\tTotal Cost: " + to_string(total_cost_item) + "\n\n";
					}
				}
				else
				{
					diff = index1[i - 1];
				    HiltonRoom h = HR[diff];
					HiltonRoomCustomerInfo HC(name, email, adults, children, needed_rooms);
					if (H1.ReserveRoom(h, HC))
					{
						total_cost_item = needed_rooms * h.price_per_night * get_diff(to_date, from_date);
						hotel_details =  "Hotel reservation: Hilton Hotel: " + h.city + " @ " + h.country + "\n\t" + from_date + " to " + to_date
								+ "\n\tAdults: " + to_string(adults) + " Children: " + to_string(children) + "\n\tTotal Cost: " + to_string(total_cost_item) + "\n\n";
					}
				}
				return new Hotel(*this);
			}
		}while(true);
	}
	virtual string get_details() const
	{
		return hotel_details;
	}
};


class Itinerary
{
private:
	vector<Itinerary_Item*> itinerary_items;
	double itinerary_cost;

public:
	Itinerary()
	{
		itinerary_cost = 0;
	}
	void add_itinerary_item(Itinerary_Item& itinerary_item)
	{
		Itinerary_Item* I1 = itinerary_item.add();

		if (I1 == nullptr)
		{
			return;
		}
		itinerary_items.push_back(I1);
	}

	void calculate_cost()
	{
		for (auto IItemPtr: itinerary_items)
		{
			itinerary_cost += IItemPtr -> get_cost();
		}
	}

	double get_itinerary_cost() const
	{
		return itinerary_cost;
	}

	int get_items_size() const
	{
		int size = itinerary_items.size();
		return size;
	}

	void cancel_itinerary()
	{
		for (auto IPtr: itinerary_items)
		{
			delete IPtr;
		}
		itinerary_items.clear();
	}

	void list_items()
	{
		for (auto IItemPtr: itinerary_items)
		{
			cout << IItemPtr -> get_details() << endl;
		}
	}

	~Itinerary()
	{
		for (auto IPtr : itinerary_items)
		{
			delete IPtr;
		}
		itinerary_items.clear();
	}
};

class User
{
private:
	string user_name;
	string password;
	string name;
	string email;
	vector<Itinerary*> itineraries;
	vector<Card*> cards;

public:
	User(string user_name, string password, string name, string email) :
		user_name(user_name), password(password), name(name), email(email)
	{

	}
	void view_profile()
	{
		cout << "\nName: " << name << "\nEmail: " << email << "\nUser Name: " << user_name << "\n\n";
	}
	void make_itinerary()
	{
		Itinerary* itinerary = new Itinerary();

		int choice;

		do
		{
			cout << "Menu:\n\t1: Add Flight\n\t2: Add Hotel\n\t3: Done\n\t4: Cancel\n\n";
			cout << "Enter number in range 1 - 4: ";
			cin >> choice;

			if (choice > 4 || choice < 1)
			{
				cout << "Invalid choice. Please try again.\n";
			}
			else if (choice == 1)
			{
				string from_date, to_date, from, to;
				int adults, children, infants;
				cout << "Enter From: ";
				cin >> from;
				cout << "Enter From Date (dd-mm-yyyy): ";
				cin >> from_date;
				cout << "Enter To: ";
				cin >> to;
				cout << "Enter To Date (dd-mm-yyyy): ";
				cin >> to_date;
				cout << "Enter no. of adults, children and infants: ";
				cin >> adults >> children >> infants;
				Flight F1(from_date, to_date, adults, children, name, email, from, to, infants);
				itinerary -> add_itinerary_item(F1);
			}
			else if (choice == 2)
			{
				string from_date, to_date, country, city;
				int adults, children, needed_rooms;
				cout << "Enter From Date (dd-mm-yyyy): ";
				cin >> from_date;
				cout << "Enter To Date (dd-mm-yyyy): ";
				cin >> to_date;
				cout << "Enter Country: ";
				cin >> country;
				cout << "Enter City: ";
				cin >> city;
				cout << "Enter number of adults and children: ";
				cin >> adults >> children;
				cout << "Enter number of rooms needed: ";
				cin >> needed_rooms;
				Hotel H1(from_date, to_date, adults, children, name, email, country, city, needed_rooms);
				itinerary -> add_itinerary_item(H1);
			}
			else if (choice == 3)
			{
				itinerary -> calculate_cost();
				double cost = itinerary -> get_itinerary_cost();
				cout << "Select a payment choice:\n";
				int i = 0;
				for (auto c : cards)
				{
					i++;
					cout << i << ": [";
					if (c -> get_debitcardyes())
					{
						cout << "Debit Card";
					}
					else
					{
						cout << "Credit Card";
					}
					cout << "]: Owner: " << name << " Card number: " << c -> get_number() << endl;
				}
				int choice2;
				do
				{
					cout << "\n\nEnter number in range 1 - " << i << ": ";
					cin >> choice2;
					if (choice2 < 1 || choice2 > i)
					{
						cout << "Invalid choice. Please try again.\n";
					}
					else
					{
						Card* c = cards[i - 1];
						if (c -> make_payment(cost))
						{
							itineraries.push_back(itinerary);
							cout << "Money withdraw successfully\n";
							cout << "Reservation confirmed\n";
							cout << "Itinerary reserved\n\n";
						}
						break;
					}
				}while(true);
				return;
			}
			else
			{
				itinerary -> cancel_itinerary();
				delete itinerary;
				cout << "Itinerary cancelled\n\n";
			}
		}while(true);
	}
	void list_itineraries()
	{
		cout << "---------------------------------------------\n";
		for (auto IPtr: itineraries)
		{
			cout << "\n*********************************************\n";
			int size = IPtr -> get_items_size();
			cout << "Itinerary of " << size << " sub-reservations\n";
			IPtr -> list_items();
			cout << "Total Itinerary cost: " << IPtr -> get_itinerary_cost() << endl;
		}
		cout << "---------------------------------------------\n";
	}
	void initialize_credit_card(string number, string name, string address, string expiry_date, int cvv, double credit_limit, double interest)
	{
		CreditCard* C1 = new CreditCard(number, name, address, expiry_date, cvv, credit_limit, interest, 0);
		Card* C = C1 -> clone();
		cards.push_back(C);
	}
	void initialize_debit_card(string number, string name, string address, string expiry_date, int cvv, double account_balance)
	{
		DebitCard* D1 = new DebitCard(number, name, address, expiry_date, cvv, account_balance, 1);
		Card* C = D1 -> clone();
		cards.push_back(C);
	}

	string get_user_name() const
	{
		return user_name;
	}

	string get_password() const
	{
		return password;
	}

	string get_name() const
	{
		return name;
	}

	~User()
	{
		for (auto IPtr: itineraries)
		{
			delete IPtr;
		}
		itineraries.clear();
		for (auto CPtr: cards)
		{
			delete CPtr;
		}
		cards.clear();
	}
};

class User_Manager
{
private:
	vector<User*> users;

public:
	void load_database()
	{
		User* U1 = new User("ram_sharma", "123", "Ram Sharma", "sharmaram@gmail.com");
		U1 -> initialize_credit_card("1646-4567-8633-2234", "Ram Sharma", "Delhi", "05/29", 023, 85000, 0.025);
		U1 -> initialize_debit_card("1223-5454-5343-2330", "Ram Sharma", "Delhi", "02/25", 128, 500000);
		users.push_back(U1);
	}
	User* login()
	{
		string username, password;
		cout << "Enter username: ";
		cin >> username;
		cout << "Enter password: ";
		cin >> password;

		for (auto UPtr: users)
		{
			if (UPtr -> get_user_name() == username && UPtr -> get_password() == password)
			{
				return UPtr;
			}
		}

		cout << "Invalid login credentials\n";
		return nullptr;
	}
	User* signup()
	{
		string username, password, name, email;
		cout << "Enter username: ";
		cin >> username;
		cout << "Enter password: ";
		cin >> password;
		cout << "Enter name: ";
		cin >> name;
		cout << "Enter email: ";
		cin >> email;
		User* U1 = new User(username, password, name, email);
		cout << "Cards Details: ";
		bool debit;
		do
		{
			string number, name, address, expiry_date;
			int cvv;
			double account_balance, credit_limit, interest;
			cout << "Is your card a debit or credit card? (1 for DebitCard, 0 for CreditCard): ";
			cin >> debit;
			if (debit != 0 && debit != 1)
			{
				cout << "Invalid response. Please try again\n";
				continue;
			}
			cout << "Enter 16 digit card number: ";
			cin >> number;
			cout << "Enter card owner name: ";
			cin >> name;
			cout << "Enter address: ";
			cin >> address;
			cout << "Enter expiry date: ";
			cin >> expiry_date;
			cout << "Enter CVV: ";
			cin >> cvv;
			if (debit)
			{
				cout << "Enter account balance: ";
				cin >> account_balance;
				U1 -> initialize_debit_card(number, name, address, expiry_date, cvv, account_balance);
			}
			else
			{
				cout << "Enter credit limit: ";
				cin >> credit_limit;
				cout << "Enter rate of interest: ";
				cin >> interest;
				U1 -> initialize_credit_card(number, name, address, expiry_date, cvv, credit_limit, interest);
			}
			int choice;
			do
			{
				cout << "Do you want to add more cards? (1 for Yes, 0 for No): ";
				cin >> choice;
				if (choice != 0 && choice != 1)
				{
					cout << "Invalid choice. Please try again\n";
					continue;
				}
				else
				{
					break;
				}
			} while (true);
			if (choice)
			{
				continue;
			}
			else
			{
				break;
			}
		}while(true);

		users.push_back(U1);
		return U1;
	}

	~User_Manager()
	{
		for (auto UPtr: users)
		{
			delete UPtr;
		}
		users.clear();
	}
};

#endif /* SERVER_H_ */
