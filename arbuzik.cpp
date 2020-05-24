#include <iostream>
#include "nlohmann/json.hpp"
#include <fstream>
#include <string>
#include "time.h"
using namespace std;
using json = nlohmann::json;

class Shifr
{
public:  
	virtual void key_gen(string& path_to_alph, string& path_to_save) = 0;
	virtual void encrypt(string& key_path, string& where_to_save, string& what_to_encrypt) = 0;
	virtual void decrypt(string& key_path, string& where_to_save, string& what_to_decrypt) = 0;
};

class Replacement : public Shifr
{
private:

	vector<int> numbers; 
	string search(string str, json key)
	{
		string encr_str;
		for (int i = 0; i < str.size(); i++)
		{ 
			for (int j = 0; j < key.at("key").size(); j++) 
			{
				string tmp = key.at("key").at(j).at(0); 
				if (str[i] == tmp[0])
				{
					string encr_elem = key.at("key").at(j).at(1);
					encr_str.push_back(encr_elem[0]);
				}
			}

		}
		return encr_str;
	}

	string rev_search(string str, json key)
	{
		string decr_str;
		for (int i = 0; i < str.size(); i++)
		{
			for (int j = 0; j < key.at("key").size(); j++)
			{
				string tmp = key.at("key").at(j).at(1);
				if (str[i] == tmp[0])
				{
					string decr_elem = key.at("key").at(j).at(0);
					decr_str.push_back(decr_elem[0]);
				}
			}
		}
		return decr_str;
	}

	bool repeat_check(int place) 
	{
		if (numbers.size() != 0)
		{
			for (int i = 0; i < numbers.size(); i++)
			{
				if (numbers[i] == place)
					return true;
				if (i == numbers.size() - 1 && numbers[i] != place)
				{
					numbers.push_back(place);
					return false;
				}
			}
		}
		else
		{
			numbers.push_back(place);
			return false;
		}
	}

public:

	void key_gen(string& path_to_alph, string& path_to_save) override
	{
		{
			setlocale(LC_ALL, "ru");
		};
		srand(time(0));
		ifstream alph_file(path_to_alph);
		if (!alph_file.is_open())
		{
			cout << "\n\nТакой файл мы открывать конечно не будем\n\n";
			exit(0);
		}
		json alph;
		json key = { {"alg_type", "replacement"},{"key",{}} };
		cout << key;
		string alph_data_str;
		int place;
		string elem;

		alph_file >> alph;
		alph_file.close();
		auto array_alph = alph.find("alp");
		if (array_alph != alph.end())
		{
			json alph_data = alph.at("alp");

			json::iterator it = alph_data.begin();

			for (int i = 0; i < alph_data.size(); i++)
			{
				string letter = *it;
				alph_data_str.push_back(letter[0]);
				it++;
			}

			for (int i = 0; i < alph_data_str.size(); i++)
			{
				place = rand() % alph_data_str.size();
				elem.push_back(alph_data_str[place]);
				if (repeat_check(place) == false)
					key.at("key").push_back(json::array({ alph_data_str.substr(i, 1), elem }));
				else
					i--;
				elem.clear();
			}

			ofstream _dot_key(path_to_save);
			_dot_key << key;
			_dot_key.close();
		}
		else
		{
			cout << "\n\nНеверный алфавитный файл\n\n";
			exit(0);
		}
	}

	void encrypt(string& rep_key_path, string& where_to_save, string& what_to_encrypt) override
	{
		{
			setlocale(LC_ALL, "ru");
		};
		fstream key_file(rep_key_path);
		if (!key_file.is_open())
		{
			cout << "\n\nНе получается открыть файл\n\n";
			exit(0);
		}

		json key;
		key_file >> key;
		key_file.close();
		if (key.at("alg_type") != "replacement")
		{
			cout << "\n\nНеверный ключ!\n\n";
			exit(0);
		}
		ofstream encr_file(where_to_save);

		fstream default_file(what_to_encrypt);
		if (!default_file.is_open())
		{
			cout << "\n\nДосада, но с файлом беда\n\n";
			exit(0);
		}

		string str;

		while (!default_file.eof())
		{
			getline(default_file, str);
			encr_file << search(str, key);
			encr_file << "\n";
		}
		default_file.close();
		encr_file.close();
	}

	void decrypt(string& rep_key_path, string& where_to_save, string& what_to_decrypt) override
	{
		{
			setlocale(LC_ALL, "ru");
		};
		fstream key_file(rep_key_path);
		if (!key_file.is_open())
		{
			cout << "\n\nФайл открыть не удалось!\n\n";
			exit(0);
		}

		json key;
		key_file >> key;
		key_file.close();
		if (key.at("alg_type") != "replacement")
		{
			cout << "\n\nКлюч косячный!\n\n";
			exit(0);
		}

		ofstream decr_file(where_to_save);

		fstream default_file(what_to_decrypt);
		if (!default_file.is_open())
		{
			cout << "\n\nНе получается открыть файл!\n\n";
			exit(0);
		}

		string str;

		while (!default_file.eof())
		{
			getline(default_file, str);
			decr_file << rev_search(str, key);
			decr_file << "\n";
		}
		default_file.close();
		decr_file.close();
	}
};

class Rearrangement : public Shifr
{
private:

	vector<int> numbers;

	bool repeat_check(int key_elem)
	{
		if (numbers.size() != 0)
		{
			for (int i = 0; i < numbers.size(); i++)
			{
				if (numbers[i] == key_elem)
					return true;
				if (i == numbers.size() - 1 && numbers[i] != key_elem)
				{
					numbers.push_back(key_elem);
					return false;
				}
			}
		}
		else
		{
			numbers.push_back(key_elem);
			return false;
		}
	}

	string encryption(string str, json key)
	{
		srand(time(0));
		int str_default_size = str.size();
		int counter;
		int rand_elem;
		string encr_str;
		int block_size = key.at("key").at("block_length");
		if (str.size() % block_size != 0)
		{
			if (str.size() < block_size)
			{
				while (str.size() != block_size)
				{
					rand_elem = rand() % str_default_size;
					str.push_back(str[rand_elem]);
				}
			}
			else
			{
				while (str.size() % block_size != 0)
				{
					rand_elem = rand() % str_default_size;
					str.push_back(str[rand_elem]);
				}
			}
		}

		string new_str;
		for (int i = 0; i < str.size() / block_size; i++)
		{

			new_str = str.substr(i * block_size, block_size);
			for (int j = 0; j < block_size; j++)
			{
				encr_str.push_back(new_str[key.at("key").at("block_value").at(j)]);
			}

		}
		return encr_str;
	}

	string decryption(string str, json key)
	{
		string decr_str;
		int block_size = key.at("key").at("block_length");

		string new_str;
		int found_elem = 0;
		for (int i = 0; i < str.size() / block_size; i++)
		{
			found_elem = 0;
			new_str = str.substr(i * block_size, block_size);
			for (int k = 0; k < block_size; k++)
			{
				for (int j = 0; j < block_size; j++)
				{
					if (key.at("key").at("block_value").at(j) == found_elem)
					{
						decr_str.push_back(new_str[j]);
						found_elem++;
						break;
					}
				}
			}
		}
		return decr_str;
	}

public:

	void key_gen(string& path_to_alph, string& path_to_save) override
	{
		{
			setlocale(LC_ALL, "ru");
		};
		srand(time(0));
		json key = { {"alg_type", "rearrangement"},{"key",{{"block_length", {}}, {"block_value", {}}} } };
		cout << "\n\nВведите длину блока:";
		int block_length;
		cin >> block_length;
		if (block_length <= 0)
		{
			cout << "\n\nДлина блока неверная\n\n";
			exit(0);
		}
		if (block_length > 1000)
		{
			cout << "\n\nДлина блока слишком велика\n\n";
			exit(0);
		}
		key.at("key").at("block_length") = block_length;
		int key_elem;

		for (int i = 0; i < block_length; i++)
		{
			key_elem = rand() % block_length;
			if (repeat_check(key_elem) == false)
				key.at("key").at("block_value").push_back(key_elem);
			else
				i--;
		}

		ofstream _dot_key(path_to_save);
		_dot_key << key;
		_dot_key.close();
	}

	void encrypt(string& key_path, string& where_to_save, string& what_to_encrypt) override
	{
		{
			setlocale(LC_ALL, "ru");
		};
		fstream key_file(key_path);
		if (!key_file.is_open())
		{
			cout << "\n\nНе получается твой файл открыть\n\n";
			exit(0);
		}

		json key;
		key_file >> key;
		key_file.close();
		if (key.at("alg_type") != "rearrangement")
		{
			cout << "\n\nБеды с ключом\n\n";
			exit(0);
		}

		ofstream encr_file(where_to_save);

		fstream default_file(what_to_encrypt);
		if (!default_file.is_open())
		{
			cout << "\n\nФайл открыть не получилось\n\n";
			exit(0);
		}

		string str;

		while (!default_file.eof())
		{
			getline(default_file, str);
			encr_file << encryption(str, key);
			encr_file << "\n";
		}
		default_file.close();
		encr_file.close();

	}

	void decrypt(string& key_path, string& where_to_save, string& what_to_decrypt) override
	{
		{
			setlocale(LC_ALL, "ru");
		};
		fstream key_file(key_path);
		json key;
		key_file >> key;
		key_file.close();
		if (key.at("alg_type") != "rearrangement")
		{
			cout << "\n\nТип ключа неверный\n\n";
			exit(0);
		}

		ofstream decr_file(where_to_save);

		fstream encrypted_file(what_to_decrypt);
		if (!encrypted_file.is_open())
		{
			cout << "\n\nНе получается открыть файлик!\n\n";
			exit(0);
		}

		string str;

		while (!encrypted_file.eof())
		{
			getline(encrypted_file, str);
			decr_file << decryption(str, key);
			decr_file << "\n";
		}
		encrypted_file.close();
		decr_file.close();
	}
};

class Gamming : public Shifr
{
private:

	vector<int> numbers;

	bool value_check(int& value)
	{
		if (numbers.size() != 0)
		{
			for (int i = 0; i < numbers.size(); i++)
			{
				if (numbers[i] == value)
					return false;
				if (i == numbers.size() - 1 && numbers[i] != value)
				{
					numbers.push_back(value);
					return true;
				}
			}
		}
		else
		{
			numbers.push_back(value);
			return true;
		}
	}

	string encryption(string str, json key)
	{
		string encr_str;
		string encr_elem;
		int size = key.at("key").at("block_length");
		int last = key.at("key").at("alph").size();
		for (int i = 0; i < str.size(); i++)
		{
			for (int j = 0; j < key.at("key").at("alph").size(); j++)
			{
				string tmp = key.at("key").at("alph").at(j).at(0);
				if (str[i] == tmp[0])
				{
					int counter;
					if (i >= size)
						counter = i % size;
					else
						counter = i;

					int res;
					int x = key.at("key").at("alph").at(j).at(1);

					int y = key.at("key").at("block_value").at(counter);

					res = (x + y) % last;
					encr_elem = key.at("key").at("alph").at(res).at(0);
					encr_str.push_back(encr_elem[0]);
					encr_elem.clear();
					break;
				}
			}
		}
		return encr_str;
	}

	string decryption(string str, json key)
	{
		string decr_str;
		string decr_elem;
		int size = key.at("key").at("block_length");
		int last = key.at("key").at("alph").size();
		for (int i = 0; i < str.size(); i++)
		{
			for (int j = 0; j < key.at("key").at("alph").size(); j++)
			{
				string tmp = key.at("key").at("alph").at(j).at(0);
				if (str[i] == tmp[0])
				{
					int counter;
					if (i >= size)
						counter = i % size;
					else
						counter = i;

					int res;

					int y = key.at("key").at("block_value").at(counter);

					res = (j - y);
					while (res < 0)
					{
						res += last;
					}
					res %= last;

					decr_elem = key.at("key").at("alph").at(res).at(0);
					decr_str.push_back(decr_elem[0]);
					decr_elem.clear();
					break;
				}
			}
		}
		return decr_str;
	}

public:

	void key_gen(string& path_to_alph, string& path_to_save)
	{
		{
			setlocale(LC_ALL, "ru");
		};
		ifstream alph_file(path_to_alph);
		if (!alph_file.is_open())
		{
			cout << "\n\nНе получается открыть файлик!\n\n";
			exit(0);
		}
		json alph;
		json key = { {"alg_type", "gamming"},{"key",  {  {"alph",  {}  }, {"block_length",    {}   },     {"block_value",     {}    }     }    } };
		string alph_data_str;
		int block_length, value_elem;

		alph_file >> alph;
		alph_file.close();

		auto array_alph = alph.find("alp");
		if (array_alph != alph.end())
		{
			json alph_data = alph.at("alp");

			json::iterator it = alph_data.begin();

			for (int i = 0; i < alph_data.size(); i++)
			{
				string letter = *it;
				alph_data_str.push_back(letter[0]);
				it++;
			}

			for (int i = 0; i < alph_data_str.size(); i++)
			{
				key.at("key").at("alph").push_back(json::array({ alph_data_str.substr(i, 1), i }));
			}

			cout << "\n\nВведи длину блока: ";
			cin >> block_length;
			if (block_length <= 0)
			{
				cout << "\n\nДлина блока неверная!\n\n";
				exit(0);
			}
			if (block_length > 1000)
			{
				cout << "\n\nНу очень уж длинный блок получился!\n\n";
				exit(0);
			}
			key.at("key").at("block_length") = block_length;

			for (int i = 0; i < block_length; i++)
			{
				value_elem = rand() % 1000;
				if (value_check(value_elem) == true)
				{
					key.at("key").at("block_value").push_back(value_elem);
				}
				else
					i--;
			}

			ofstream _dot_key(path_to_save);
			_dot_key << key;
			_dot_key.close();
		}
		else
		{
			cout << "\n\nУстрани проблемы с файлом!\n\n";
			exit(0);
		}
	}

	void encrypt(string& gamm_key_path, string& where_to_save, string& what_to_encrypt)

	{
		{
			setlocale(LC_ALL, "ru");
		};
		fstream key_file(gamm_key_path);
		if (!key_file.is_open())
		{
			cout << "\n\nС твоим файлом беда\n\n";
			exit(0);
		}

		json key;
		key_file >> key;
		key_file.close();
		if (key.at("alg_type") != "gamming")
		{
			cout << "\n\nКлюч не такой как надо\n\n";
			exit(0);
		}

		ofstream encr_file(where_to_save);

		fstream default_file(what_to_encrypt);
		if (!default_file.is_open())
		{
			cout << "\n\nС твоим файлом беда\n\n";
			exit(0);
		}

		string str;

		while (!default_file.eof())
		{
			getline(default_file, str);
			encr_file << encryption(str, key);
			encr_file << "\n";
		}
		default_file.close();
		encr_file.close();
	}

	void decrypt(string& gamm_key_path, string& where_to_save, string& what_to_decrypt)
	{
		fstream key_file(gamm_key_path);
		if (!key_file.is_open())
		{
			cout << "\n\nНе получается открыть файл\n\n";
			exit(0);
		}

		json key;
		key_file >> key;
		key_file.close();
		if (key.at("alg_type") != "gamming")
		{
			cout << "\n\nБеды с ключом\n\n";
			exit(0);
		}

		ofstream decr_file(where_to_save);

		fstream encrypted_file(what_to_decrypt);
		if (!encrypted_file.is_open())
		{
			cout << "\n\nЯ так не играю\n\n";
			exit(0);
		}

		string str;

		while (!encrypted_file.eof())
		{
			getline(encrypted_file, str);
			decr_file << decryption(str, key);
			decr_file << "\n";
		}
		encrypted_file.close();
		decr_file.close();
	}
};

int main()
{ 
	{
		setlocale(LC_ALL, "ru");
	};
	int x;
	while (1)
	{
		cout << "Ну и что хотим:\n\t1)Запутать кого нибудь или запутать \n\t2)Го ключ\n\t3)Не надо ничего\n\nТак и что: ";
		cin >> x;
		if (x == 1)
		{
			x = 0;
			cout << "\n\n1)Запутать\n2)Распутать\n\nМ?: ";
			cin >> x;
			if (x == 1)
			{
				x = 0;
				cout << "\n\nНу выбирай что за секрктик:\n1)Можно заменить твои буковки на кое-какие другие\n2)Поменяем твои буквы местами\n3)Метод гения инженерной мысли\n\nВаляй: ";
				cin >> x;
				if (x == 1)
				{
					x = 0;

					cout << "\n\nДавай сюда путь к файлу в формате txt только: "; string what_to_encrypt; cin >> what_to_encrypt;


					cout << "Теперь путь к ключику в формате key соответственно: "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nЧто-то ты мне тут шляпу подсунул, давай по новой\n\n";
						return 0;
					}

					cout << "И на последок путь к новому дому, вот в таком формате - encrypt: "; string save_path; cin >> save_path;
					if (save_path.substr(save_path.size() - 8, 8) != ".encrypt")
					{
						cout << "\n\nТы налагал, прочти выше как надо\n\n";
						return 0;
					}
					Replacement rep;
					rep.encrypt(key, save_path, what_to_encrypt);
				}
				else if (x == 2)
				{
					x = 0;

					cout << "\n\nНу раз такое дело, давай путь к своим игрушкам ( файл в формате txt должен быть): "; string what_to_encrypt; cin >> what_to_encrypt;


					cout << "Теперь путь к ключику в формате key соответственно): "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nТы что-то напутал, давай заново\n\n";
						return 0;
					}

					cout << "Куда тебе его сохранить? Формат encrypt нужен): "; string save_path; cin >> save_path;
					if (save_path.substr(save_path.size() - 8, 8) != ".encrypt")
					{
						cout << "\n\nНе, не пойдет, почитай ещё раз как надо\n\n";
						return 0;
					}
					Rearrangement rearr;
					rearr.encrypt(key, save_path, what_to_encrypt);
				}
				else if (x == 3)
				{
					x = 0;

					cout << "\n\nДавай сюда путь к файлу в формате txt, засекретим тебе): "; string what_to_encrypt; cin >> what_to_encrypt;


					cout << "Теперь к ключику в формате key): "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nВсё фигня, давай по новой\n\n";
						return 0;
					}

					cout << "Сохранять будем в формате encrypt, только куда?): "; string save_path; cin >> save_path;
					if (save_path.substr(save_path.size() - 8, 8) != ".encrypt")
					{
						cout << "\n\nПерепутал что-то, давай еще раз\n\n";
						return 0;
					}
					Gamming gamm;
					gamm.encrypt(key, save_path, what_to_encrypt);
				}
				else
				{
					x = 0;
					cout << "\n\nЧушь какая-то\n\n";
					return 0;
				}
			}
			else if (x == 2)
			{
				x = 0;
				cout << "\n\nКак будем распутывать?:\n1)Поставьть буквы в привычном алфавите\n2)Вернуть буквы на свои места\n3)Не тут сами думайте, просто вернуть норм текст\n\nОпределяйся: ";
				cin >> x;
				if (x == 1)
				{
					x = 0;

					cout << "\n\nВведи путь к абракадабре в формате encrypt): "; string what_to_decrypt; cin >> what_to_decrypt;


					cout << "Теперь елюч в формате key): "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nНеправильно ввёл что-то\n\n";
						return 0;
					}

					cout << "Куда хочешь сохранить? Формат encrypt.txt): "; string save_path; cin >> save_path;

					Replacement rep;
					rep.decrypt(key, save_path, what_to_decrypt);
				}
				else if (x == 2)
				{
					x = 0;

					cout << "\n\nВведи путь к нелепице в формате encrypt): "; string what_to_decrypt; cin >> what_to_decrypt;

					cout << "Введи путь к ключику от нелепицы в формате key): "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nВсё фигня, давай по новой\n\n";
						return 0;
					}

					cout << "Куда будем сохранять? В encrypt.txt только): "; string save_path; cin >> save_path;

					Rearrangement rearr;
					rearr.decrypt(key, save_path, what_to_decrypt);
				}
				else if (x == 3)
				{
					x = 0;

					cout << "\n\nВведи путь к белиберде в формате encrypt): "; string what_to_decrypt; cin >> what_to_decrypt;


					cout << "Теперь антибелеберду в формате key): "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nВсё фигня, давай по новой\n\n";
						return 0;
					}

					cout << "Сохранять будем в формат encrypt.txt, только куда?): "; string save_path; cin >> save_path;

					Gamming gamm;
					gamm.decrypt(key, save_path, what_to_decrypt);
				}
				else
				{
					x = 0;
					cout << "\n\nЧушь какая-то\n\n";
					return 0;
				}
			}
			else
			{
				x = 0;
				cout << "\n\nЧушь какая-то\n\n";
				return 0;
			}
		}
		else if (x == 2)
		{
			x = 0;

			cout << "\n\nВыбери метод шифрования:\n1)Замена\n2)Перестановка\n3)Гаммирование\n\nНу шо там? ";
			cin >> x;
			if (x == 1)
			{
				x = 0;


				cout << "\n\nУкажи путь к файлику в формате alph): "; string alph; cin >> alph;
				if (alph.size() < 5)
				{
					cout << "\n\nЧушь какая-то\n\n";
					exit(0);
				}
				if (alph.substr(alph.size() - 5, 5) != ".alph")
				{
					cout << "\n\n.alph блин! Давай еще раз";
					return 0;
				}
				cout << "\n\nПуть для сохранения ключика в формате key: "; string key; cin >> key;
				Replacement rep;
				rep.key_gen(alph, key);
			}
			else if (x == 2)
			{
				x = 0;


				cout << "\n\nПуть для сохранения ключика в формате key: "; string key; cin >> key;
				string empty = "";
				Rearrangement rearr;
				rearr.key_gen(empty, key);
			}
			else if (x == 3)
			{
				x = 0;

				cout << "\n\nВведи путь к алфавиту в фомате alph): "; string alph; cin >> alph;
				if (alph.size() < 5)
				{
					cout << "\n\nЧтот не то\n\n";
					exit(0);
				}
				if (alph.substr(alph.size() - 5, 5) != ".alph")
				{
					cout << "\n\nНу посмотри же какой формат! Заново.";
					return 0;
				}
				cout << "\n\nКуда сохранить ключ? Формат key): "; string key; cin >> key;
				Gamming gamm;
				gamm.key_gen(alph, key);
			}
			else
			{
				cout << "\n\nЧушь какая-то\n\n";
				return 0;
			}
		}
		else if (x == 3)
		{
			return 0;
		}
		else
		{
			x = 0;
			cout << "\n\nЧушь какая-то\n\n";
			return 0;
		}
	}
	return 0;
}