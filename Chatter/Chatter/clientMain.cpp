#include <iostream>
#include "Client.h"
#include <thread>


using namespace std;

int clientMain()
{
	Client client;
	bool isConnected = false;
	string input = "";
	int action = 0;
	string userTo = "";
	string message = "";
	bool exit = false;

	do
	{
		cout << "Enter username: ";
		cin >> input;
		client.setUsername(input);

		cout << "Enter password: ";
		cin >> input;
		client.setPassword(input);
		isConnected = client.Login();

		if (!isConnected)
			cout << "Incorrect username or password\n";
	} while (!isConnected);

	cout << "Enter Action(1-6):\n1)Get List of active users\n2)Start chat\n3)Send message to all users\n4)block a user\n5)unblock a user\n6)exit";
	while (!exit)
	{
		cin >> action;
		switch (action)
		{
			case 1:
				client.GetActiveUsers();
				break;
			case 2:
                cout << "Enter user to chat with: ";
				cin >> userTo;
				cout << "attempting to start chat with: " + input<<endl;
				cout << "Enter message to send: ";
				cin >> message;
				client.Send(client.getUsername(), userTo, message);
				break;
			case 3:
				cout << "Enter message to send to all users: ";
				cin >> input;
				client.SendToAllUsers(client.getUsername(), input);
				break;
			case 4:
				cout << "Enter user to ban: ";
				cin >> input;
				client.Ban(client.getUsername(), input);
				break;
			case 5:
				cout << "Enter user to unban: ";
				cin >> input;
				client.Unban(client.getUsername(), input);
				break;
			case 6:
				exit = true;
				break;
			default:
				cout << "invalid input. Must input 1-6\n)";
				break;
		}
	}

	return 0;
}