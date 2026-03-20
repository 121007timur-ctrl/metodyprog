#include "singletonclient.h"
#include <QCoreApplication>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SingletonClient* client = SingletonClient::getInstance();

    cout << "Connecting to server..." << endl;
    if (client->connectToServer("localhost", 33333)) {
        cout << "Connected!" << endl;
        cout << client->waitForResponse().toStdString() << endl;
        
        cout << "Enter command (REGISTER, LOGIN, STATS) or 'exit' to quit:" << endl;
        string command;
        
        while (true) {
            cout << "> ";
            getline(cin, command);
            if (command == "exit") break;
            if (!command.empty()) {
                client->sendCommand(QString::fromStdString(command));
                cout << client->waitForResponse().toStdString() << endl;
            }
        }
        client->disconnectFromServer();
    } else {
        cout << "Failed to connect to server" << endl;
    }
    return 0;
}