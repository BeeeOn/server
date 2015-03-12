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
	string text(" ");      // Odesílaný a pøijímaný text
	text = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><adapter_server adapter_id=\"0x00caf8\" fw_version=\"1.0\" protocol_version=\"1.0\" state=\"data\" time=\"1424275215\"><device id=\"0x40704020082\"><battery value=\"3020\"/><rssi value=\"69\"/><values count=\"1\"><value offset=\"0x00\" type=\"0x0a\">26.18</value></values></device></adapter_server>";
	cout << text << endl;
    hostent *host;              // Vzdálený poèítaè;
    sockaddr_in serverSock;     // Vzdálený "konec potrubí"
    int mySocket;               // Soket
    int port;                   // Èíslo portu
    int size;                   // Poèet pøijatých a odeslaných bytù
    if (argc != 3)
    {
        cerr << "Syntaxe:\n\t" << argv[0]
                  << " " << "adresa port" << endl;
        return -1;
    }
    port = atoi(argv[2]);
    // Zjistíme info o vzdáleném poèítaèi
    if ((host = gethostbyname(argv[1])) == NULL)
    {
        cerr << "Špatná adresa" << endl;
        return -1;
    }
    // Vytvoøíme soket
    if ((mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        cerr << "Nelze vytvoøit soket" << endl;
        return -1;
    }

	//Nastavení struktury portBind
	sockaddr_in portBind;
	portBind.sin_family = AF_INET;
	portBind.sin_port = htons(7383);
	portBind.sin_addr.s_addr = INADDR_ANY;

	if (bind(mySocket, (sockaddr *)&portBind, sizeof(portBind)) == -1)
	{
		cerr << "Problém s pojmenováním soketu." << endl;
		return -1;
	}

    // Zaplníme strukturu sockaddr_in
    // 1) Rodina protokolù
    serverSock.sin_family = AF_INET;
    // 2) Èíslo portu, ke kterému se pøipojíme
    serverSock.sin_port = htons(port);
    // 3) Nastavení IP adresy, ke které se pøipojíme
    memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);
    // Pøipojení soketu
    if (connect(mySocket, (sockaddr *)&serverSock, sizeof(serverSock)) == -1)
    {
        cerr << "Nelze navázat spojení" << endl;
        return -1;
    }
    // Odeslání dat
    if ((size = send(mySocket, text.c_str(), text.size() + 1, 0)) == -1)
    {
        cerr << "Problém s odesláním dat" << endl;
        return -1;
    }
    cout << "Odesláno " << size << endl;

    // Uzavøu spojení
    close(mySocket);
    return 0;
}