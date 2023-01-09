/*
 * expedia_hotels_api.h
 *
 *  Created on: Sep 22, 2020
 *      Author: moustafa
 */

#ifndef EXPEDIA_HOTELS_API_H_
#define EXPEDIA_HOTELS_API_H_

#include <iostream>
#include <vector>
using namespace std;

class HiltonRoom {
public:
	string room_type;
	int available;
	double price_per_night;
	string date_from;
	string date_to;
	string country;
	string city;
	HiltonRoom(string room_type, int available, double price, string date_time_from, string date_time_to, string country, string city):
		room_type(room_type), available(available), price_per_night(price), date_from(date_time_from), date_to(date_time_to), country(country), city(city)
	{

	}
};

class HiltonRoomCustomerInfo {
public:
	string name;
	string email;
	int adults;
	int children;
	int needed_rooms;
	HiltonRoomCustomerInfo(string name, string email, int adult, int children, int needed_rooms):
		name(name), email(email), adults(adult), children(children), needed_rooms(needed_rooms)
	{

	}
};

class HiltonHotelAPI {
public:
	static vector<HiltonRoom> SearchRooms() {
		vector<HiltonRoom> rooms;

		rooms.push_back( { "Interior View", 6, 200.0, "29-01-2022", "10-02-2022", "Canada", "Vancouver" });
		rooms.push_back( { "City View", 3, 300.0, "29-01-2022", "10-02-2022", "Canada", "Vancouver" });
		rooms.push_back( { "Deluxe View", 8, 500.0, "29-01-2022", "10-02-2022", "Canada", "Vancouver" });

		return rooms;
	}

	static bool ReserveRoom(const HiltonRoom& room, const HiltonRoomCustomerInfo &info) {
		return true;
	}
	static bool CancelReserveRoom(const HiltonRoom& room, const HiltonRoomCustomerInfo &info) {
		return true;
	}
};

class MarriottFoundRoom {
public:
	string room_type;
	int available;
	double price_per_night;
	string date_from;
	string date_to;
	string country;
	string city;
	MarriottFoundRoom(string room_type, int available, double price, string date_time_from, string date_time_to, string country, string city):
		room_type(room_type), available(available), price_per_night(price), date_from(date_time_from), date_to(date_time_to), country(country), city(city)
	{

	}
};

class MarriottFoundRoomCustomerInfo {
public:
	string name;
	string email;
	int adults;
	int children;
	int needed_rooms;
	MarriottFoundRoomCustomerInfo(string name, string email, int adult, int children, int needed_rooms):
		name(name), email(email), adults(adult), children(children), needed_rooms(needed_rooms)
	{

	}
};

class MarriottHotelAPI {
public:
	static vector<MarriottFoundRoom> FindRooms() {
		vector<MarriottFoundRoom> rooms;

		rooms.push_back( { "City View", 8, 320.0, "29-01-2022", "10-02-2022", "Canada", "Vancouver" });
		rooms.push_back( { "Interior View", 8, 220.0, "29-01-2022", "10-02-2022", "Canada", "Vancouver" });
		rooms.push_back( { "Private View", 5, 600.0, "29-01-2022", "10-02-2022", "Canada", "Vancouver" });

		return rooms;
	}

	static bool ReserveRoom(const MarriottFoundRoom& room, const MarriottFoundRoomCustomerInfo &info) {
		return true;
	}
	static bool CancelReserveRoom(const MarriottFoundRoom& room, const MarriottFoundRoomCustomerInfo &info) {
		return true;
	}
};

#endif /* EXPEDIA_HOTELS_API_H_ */
