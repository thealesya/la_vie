#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
using namespace std;
void poisk(string path, string fileName, string device, bool& isEmpty, int i)
{
	fstream readFile; 
	readFile.open(path);  

	while (!readFile.eof()) 
	{
		string info, time, kapusta, str, deviceName;
		int kto = 0; 
		getline(readFile, str);
		if (str.length() == 0 || str.find("garbage") != -1) 
		{
			continue;
		}

		for (int i = 0; i < str.length(); i++) {
			
			if (str[i] == ':')
			{ 
				kto = i;  
				break; 
			}
		}
		if (kto == 2)
		{
			kapusta = str.substr(0, kto);
		}
		else
		{
			kapusta = "0" + str.substr(0, kto);
		}

		str.replace(0, kto + 1, "");
		
		kto = 0; 

		for (int i = 0; i < str.length(); i++) 
		{ 
			if (str[i] == ':')
			{
				kto = i;
				break;
			}
		}
		
		deviceName = str.substr(0, kto);
		
		if (deviceName != device) 
		{ 
			continue;
		}
		
		str.replace(0, kto + 1, ""); 
		info = str; 
		isEmpty = false;  
		time = fileName.substr(5, fileName.length() - 10);
		cout << i << " ДАТА  | " << time << "-" << kapusta << " | ИНФА | " << info <<endl;
	}
	readFile.close(); 
}

int main()
{
	{
		setlocale(LC_ALL, "ru");
	};

	string path, device;
	cout << "Введи имя девайса:"; getline(cin, device);
	cout << "Путь к нему:"; getline(cin, path);

	vector<string> v_pathFile;
	vector<string> v_File;
	bool isEmpty = true;
	try 
	{
		for (auto& pointer :filesystem::directory_iterator(path))
		{ 

			auto fileName = pointer.path().filename().string(); 
			auto pathFile = pointer.path().string(); 
			v_pathFile.push_back(pathFile); 
			v_File.push_back(fileName);
		}
	}
	catch (const exception e) 
	{
		cout << "Еще разок" << endl;
		return 0;
	}

	for (int i = 0; i < v_File.size(); i++)
	{ 
		poisk(v_pathFile[i], v_File[i], device, isEmpty, i);
	}

	if (isEmpty) 
	{ 
		cout << "Одна ошибка и ты ошибся" << endl;
	}
}

