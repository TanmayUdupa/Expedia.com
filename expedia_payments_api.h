/*
 * expedia_payments_api.h
 *
 *  Created on: Sep 18, 2020
 *      Author: moustafa
 */

#ifndef EXPEDIA_PAYMENTS_API_H_
#define EXPEDIA_PAYMENTS_API_H_

#include <iostream>

using namespace std;

#include "json.hpp"
using namespace json;


class PayPalCreditCard {
public:
	string name;
	string address;
	string id;
	string expiry_date;
	int cvv;
	PayPalCreditCard(string name, string address, string id, string expiry_date, int cvv) :
		name(name), address(address), id(id), expiry_date(expiry_date), cvv(cvv)
	{

	}
};

class PayPalOnlinePaymentAPI {
public:
	void SetCardInfo(const PayPalCreditCard* const card) {
	}
	bool MakePayment(double money) {
		return true;
	}
	bool CancelPayment(const PayPalCreditCard* const card) {
		return true;
	}
};

class StripeUserInfo {
public:
	string name;
	string address;
	StripeUserInfo(string name, string address) :
		name(name), address(address)
	{

	}
};

class StripeCardInfo {
public:
	string id;
	string expiry_date;
	StripeCardInfo(string id, string expiry_date) :
	id(id), expiry_date(expiry_date)
	{

	}
};

class StripePaymentAPI {
public:
	bool static WithDrawMoney(StripeUserInfo user, StripeCardInfo card, double money) {
		return true;
	}
	bool CancelPayment(StripeUserInfo user, StripeCardInfo card) {
		return true;
	}
};

class SquarePaymentAPI {
public:
	bool static WithDrawMoney(string JsonQuery) {
		//cout << JsonQuery << "\n";
		json::JSON obj = JSON::Load(JsonQuery);
		return true;
	}
	bool CancelPayment(string JsonQuery) {
		return true;
	}
};


#endif /* EXPEDIA_PAYMENTS_API_H_ */
