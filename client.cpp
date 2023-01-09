/*
 * client.cpp
 *
 *  Created on: Jan 7, 2023
 *      Author: tanma
 */

#include "server.h"

int main()
{
	User_Manager U;
	U.load_database();
	User* current;
	int choice;
	int count = 0;

	do
	{
		cout << "Menu:\n\t1: Login\n\t2. Sign Up\n\n";
		cout << "Enter number in range 1 - 2: ";
		cin >> choice;
		if (choice < 1 || choice > 2)
		{
			cout << "Invalid choice. Pleae try again.\n";
			continue;
		}
		else if (choice == 1)
		{
			current = U.login();
			if (!current)
			{
				if (count != 3)
				{
					count++;
					cout << "Try again\n";
					continue;
				}
				if (count == 3)
				{
					cout << "You have exceeded no. of attempts. Please restart.\n";
					break;
				}
			}
			break;
		}
		else
		{
			current = U.signup();
			break;
		}
	}while(true);

	cout << "\n\n";

	if (current)
	{
		cout << "Hello " << current -> get_name() << " | User View\n\n";
		int choice2;
		do
		{
			cout << "Menu:\n\t1: View Profile\n\t2: Make Itinerary\n\t3: List my itineraries\n\t4: Logout\n\n";
			cout << "Enter number in the range 1 - 4: ";
			cin >> choice2;
			if (choice2 < 1 || choice2 > 4)
			{
				cout << "Invalid choice. Please try again\n";
				continue;
			}
			else if (choice2 == 1)
			{
				current -> view_profile();
			}
			else if (choice2 == 2)
			{
				current -> make_itinerary();
			}
			else if (choice2 == 3)
			{
				current -> list_itineraries();
			}
			else
			{
				break;
			}
		}while(true);
	}

	return 0;
}


