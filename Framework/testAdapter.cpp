#include <iostream>
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>

#define BUFSIZE 1000

using namespace std;

//Testovani zasilani zprav Frameworku

//Argv[1] ... localhost		adresa serveru
//Argv[2] ... 7282			port serveru

int main(int argc, char *argv[])
{
	string text(" ");      // Odes�lan� a p�ij�man� text
	text = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><adapter_server adapter_id=\"0x00caf8\" fw_version=\"1.0\" protocol_version=\"1.0\" state=\"data\" time=\"1424275215\"><device id=\"0x40704020082\"><battery value=\"3020\"/><rssi value=\"69\"/><values count=\"1\"><value offset=\"0x00\" type=\"0x0a\">26.18</value></values></device></adapter_server>";
	cout << text << endl;
    hostent *host;              // Vzd�len� po��ta�;
    sockaddr_in serverSock;     // Vzd�len� "konec potrub�"
    int mySocket;               // Soket
    int port;                   // ��slo portu
    int size;                   // Po�et p�ijat�ch a odeslan�ch byt�
    if (argc != 3)
    {
        cerr << "Syntaxe:\n\t" << argv[0]
                  << " " << "adresa port" << endl;
        return -1;
    }
    port = atoi(argv[2]);
    // Zjist�me info o vzd�len�m po��ta�i
    if ((host = gethostbyname(argv[1])) == NULL)
    {
        cerr << "�patn� adresa" << endl;
        return -1;
    }
    // Vytvo��me soket
    if ((mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        cerr << "Nelze vytvo�it soket" << endl;
        return -1;
    }

	//Nastaven� struktury portBind
	sockaddr_in portBind;
	portBind.sin_family = AF_INET;
	portBind.sin_port = htons(7383);
	portBind.sin_addr.s_addr = INADDR_ANY;

	if (bind(mySocket, (sockaddr *)&portBind, sizeof(portBind)) == -1)
	{
		cerr << "Probl�m s pojmenov�n�m soketu." << endl;
		return -1;
	}

    // Zapln�me strukturu sockaddr_in
    // 1) Rodina protokol�
    serverSock.sin_family = AF_INET;
    // 2) ��slo portu, ke kter�mu se p�ipoj�me
    serverSock.sin_port = htons(port);
    // 3) Nastaven� IP adresy, ke kter� se p�ipoj�me
    memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);
    // P�ipojen� soketu
    if (connect(mySocket, (sockaddr *)&serverSock, sizeof(serverSock)) == -1)
    {
        cerr << "Nelze nav�zat spojen�" << endl;
        return -1;
    }
    // Odesl�n� dat
    if ((size = send(mySocket, text.c_str(), text.size() + 1, 0)) == -1)
    {
        cerr << "Probl�m s odesl�n�m dat" << endl;
        return -1;
    }
    cout << "Odesl�no " << size << endl;

    // Uzav�u spojen�
    close(mySocket);
    return 0;
}