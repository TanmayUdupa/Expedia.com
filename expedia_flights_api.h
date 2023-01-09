/*
 * expedia_flights_api.h
 *
 *  Created on: Sep 18, 2020
 *      Author: moustafa
 */

#ifndef EXPEDIA_FLIGHTS_API_H_
#define EXPEDIA_FLIGHTS_API_H_

#include <iostream>
#include <vector>

using namespace std;


class AirCanadaCustomerInfo {
public:
	string name;
	string email;
	int adult;
	int children;
	int infants;
	AirCanadaCustomerInfo(string name, string email, int adult, int children, int no_of_infants):
		name(name), email(email), adult(adult), children(children), infants(no_of_infants)
	{

	}
};

class AirCanadaFlight {
public:
	double price;
	string date_time_from;
	string date_time_to;
	string from;
	string to;
	AirCanadaFlight(double price, string date_time_from, string date_time_to, string from, string to):
		price(price), date_time_from(date_time_from), date_time_to(date_time_to), from(from), to(to)
	{

	}
};

class AirCanadaOnlineAPI {
public:
	static vector<AirCanadaFlight> GetFlights() {
		vector<AirCanadaFlight> flights;

		flights.push_back( { 200, "25-01-2022", "10-02-2022", "Cairo", "Vancouver" });
		flights.push_back( { 250, "29-01-2022", "10-02-2022", "Cairo", "Vancouver" });
		return flights;
	}
	static bool ReserveFlight(const AirCanadaFlight& flight, const AirCanadaCustomerInfo &info) {
		return true;
	}
	static bool CancelReserveFlight(const AirCanadaFlight& flight, const AirCanadaCustomerInfo &info)
	{
		return true;
	}
};

class TurkishFlight {
public:
	double cost;
	string datetime_from;
	string datetime_to;
	string from;
	string to;
	TurkishFlight(double price, string date_time_from, string date_time_to, string from, string to):
		cost(price), datetime_from(date_time_from), datetime_to(date_time_to), from(from), to(to)
	{

	}
};

class TurkishCustomerInfo {
public:
	string name;
	string email;
	int adult;
	int children;
	int infants;
	TurkishCustomerInfo(string name, string email, int adult, int children, int no_of_infants):
		name(name), email(email), adult(adult), children(children), infants(no_of_infants)
	{

	}
};

class TurkishAirlinesOnlineAPI {
public:
	vector<TurkishFlight> GetAvailableFlights() const {
		vector<TurkishFlight> flights;

		flights.push_back( { 300, "10-01-2022", "10-02-2022", "Cairo", "Vancouver" });
		flights.push_back( { 320, "12-01-2022", "10-02-2022", "Cairo", "Vancouver" });
		return flights;
	}

	static bool ReserveFlight(const TurkishCustomerInfo &info, const TurkishFlight& flight) {
		return true;
	}
	static bool CancelReserveFlight(const TurkishCustomerInfo &info, const TurkishFlight& flight) {
		return true;
	}
};

#endif /* EXPEDIA_FLIGHTS_API_H_ */
