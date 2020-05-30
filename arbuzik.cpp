#include <iostream>
#include <fstream>
#include <string>
#include "time.h"
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

class Eboy 
{
public:  
	virtual void gen_key(string& gde_alph, string& kuda_sohr) = 0;
	virtual void Shifr(string& key_put, string& gde_sohranit, string& chto_za_sh) = 0;
	virtual void Neshifr(string& key_put, string& gde_sohranit, string& chto_za_ne) = 0;
};

class Zamena : public Eboy  
{
private:

	vector<int> poryadok; 
	string poisk(string str, json key)
	{
		string sh_str; 
		for (int i = 0; i < str.size(); i++)
		{ 
			for (int j = 0; j < key.at("key").size(); j++) 
			{
				string a = key.at("key").at(j).at(0); 
				if (str[i] == a[0])
				{
					string sh_elem = key.at("key").at(j).at(1);
					sh_str.push_back(sh_elem[0]); 
				}
			}

		}
		return sh_str; 
	}

	string kuda(string str, json key) 
	{
		string Ne_str; 
		for (int i = 0; i < str.size(); i++)
		{
			for (int j = 0; j < key.at("key").size(); j++)
			{
				string a = key.at("key").at(j).at(1);
				if (str[i] == a[0])
				{
					string Ne_elem = key.at("key").at(j).at(0);
					Ne_str.push_back(Ne_elem[0]);
				}
			}
		}
		return Ne_str;
	}

	bool repeat_check(int okno) 
	{
		if (poryadok.size() != 0)
		{
			for (int i = 0; i < poryadok.size(); i++)
			{
				if (poryadok[i] == okno)
					return true;
				if (i == poryadok.size() - 1 && poryadok[i] != okno)
				{
					poryadok.push_back(okno);
					return false;
				}
			}
		}
		else
		{
			poryadok.push_back(okno);
			return false;
		}
	}

public:

	void gen_key(string& gde_alph, string& kuda_sohr) override 

	{
		{
			setlocale(LC_ALL, "ru");
		};
		srand(time(0));
		ifstream alph_file(gde_alph);
		if (!alph_file.is_open())
		{
			cout << "\n\nТакой файл мы открывать конечно не будем\n\n";
			exit(0);
		}
		json alph;
		json key = { {"tip", "zamena"},{"key",{}} }; 
		
		cout << key;
		string alph_data_str;
		int okno;
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
				string text = *it;
				alph_data_str.push_back(text[0]); //выбираем ячейку
				it++;
			}

			for (int i = 0; i < alph_data_str.size(); i++)
			{
				okno = rand() % alph_data_str.size(); 
				elem.push_back(alph_data_str[okno]);
				if (repeat_check(okno) == false) 
					key.at("key").push_back(json::array({ alph_data_str.substr(i, 1), elem }));
				else
					i--;
				elem.clear();
			}

			ofstream _dot_key(kuda_sohr);
			_dot_key << key;
			_dot_key.close();
		}
		else
		{
			cout << "\n\nНеверный алфавитный файл\n\n";
			exit(0);
		}
	}

	void Shifr(string& zam_key_path, string& gde_sohranit, string& chto_za_sh) override
	{
		{
			setlocale(LC_ALL, "ru");
		};
		fstream key_file(zam_key_path);
		if (!key_file.is_open())
		{
			cout << "\n\nНе получается открыть файл\n\n";
			exit(0);
		}

		json key;
		key_file >> key;
		key_file.close();
		if (key.at("tip") != "zamena")
		{
			cout << "\n\nНеверный ключ!\n\n";
			exit(0);
		}
		ofstream sh_file(gde_sohranit);

		fstream default_file(chto_za_sh);
		if (!default_file.is_open())
		{
			cout << "\n\nДосада, но с файлом беда\n\n";
			exit(0);
		}

		string str;

		while (!default_file.eof())
		{
			getline(default_file, str);
			sh_file << poisk(str, key); 
			sh_file << "\n";
		}
		default_file.close();
		encr_file.close();
	}

	void Neshifr(string& zam_key_path, string& gde_sohranit, string& chto_za_sh) override
	{
		{
			setlocale(LC_ALL, "ru");
		};
		fstream key_file(zam_key_path);
		if (!key_file.is_open())
		{
			cout << "\n\nФайл открыть не удалось!\n\n";
			exit(0);
		}

		json key;
		key_file >> key;
		key_file.close();
		if (key.at("tip") != "zamena")
		{
			cout << "\n\nКлюч косячный!\n\n";
			exit(0);
		}

		ofstream Ne_file(gde_sohranit);

		fstream default_file(chto_za_sh);
		if (!default_file.is_open())
		{
			cout << "\n\nНе получается открыть файл!\n\n";
			exit(0);
		}

		string str;

		while (!default_file.eof())
		{
			getline(default_file, str);
			Ne_file << kuda(str, key);  
			Ne_file << "\n";
		}
		default_file.close();
		Ne_file.close();
	}
};

class Perestanovka : public Eboy   
{
private:

	vector<int> poryadok;

	bool repeat_check(int key_elem)
	{
		if (poryadok.size() != 0)
		{
			for (int i = 0; i < poryadok.size(); i++)
			{
				if (poryadok[i] == key_elem)
					return true;
				if (i == poryadok.size() - 1 && poryadok[i] != key_elem)
				{
					poryadok.push_back(key_elem);
					return false;
				}
			}
		}
		else
		{
			poryadok.push_back(key_elem);
			return false;
		}
	}

	string shifraciya(string str, json key)
	{
		srand(time(0));
		int str_default_size = str.size();
		int counter;
		int rand_elem;
		string sh_str;
		int block_size = key.at("key").at("dlina");
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
				sh_str.push_back(new_str[key.at("key").at("dlina").at(j)]);
			}

		}
		return sh_str;
	}

	string neshifraciya(string str, json key)
	{
		string ne_str;
		int block_size = key.at("key").at("dlina");

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
						ne_str.push_back(new_str[j]);
						found_elem++;
						break;
					}
				}
			}
		}
		return ne_str;
	}

public:

	void gen_key(string& gde_alph, string& kuda_sohr) override
	{
		{
			setlocale(LC_ALL, "ru");
		};
		srand(time(0));
		json key = { {"tip", "perestan"},{"key",{{"dlina", {}}, {"znach", {}}} } };
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
		key.at("key").at("dlina") = block_length;
		int key_elem;

		for (int i = 0; i < block_length; i++)
		{
			key_elem = rand() % block_length;
			if (repeat_check(key_elem) == false)
				key.at("key").at("znach").push_back(key_elem);
			else
				i--;
		}

		ofstream _dot_key(kuda_sohr);
		_dot_key << key;
		_dot_key.close();
	}

	void Shifr(string& key_put, string& gde_sohranit, string& chto_za_sh) override
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
		if (key.at("tip") != "perestan")
		{
			cout << "\n\nБеды с ключом\n\n";
			exit(0);
		}

		ofstream sh_file(gde_sohranit);

		fstream default_file(chto_za_sh);
		if (!default_file.is_open())
		{
			cout << "\n\nФайл открыть не получилось\n\n";
			exit(0);
		}

		string str;

		while (!default_file.eof())
		{
			getline(default_file, str);
			sh_file << shifraciya(str, key);
			sh_file << "\n";
		}
		default_file.close();
		sh_file.close();

	}

	void Neshifr(string& key_put, string& gde_sohranit, string& chto_za_ne) override
	{
		{
			setlocale(LC_ALL, "ru");
		};
		fstream key_file(key_put);
		json key;
		key_file >> key;
		key_file.close();
		if (key.at("tip") != "Perestan")
		{
			cout << "\n\nТип ключа неверный\n\n";
			exit(0);
		}

		ofstream ne_file(gde_sohranit);

		fstream shifro_file(chto_za_ne);
		if (!shifro_file.is_open())
		{
			cout << "\n\nНе получается открыть файлик!\n\n";
			exit(0);
		}

		string str;

		while (!shifro_file.eof())
		{
			getline(shifro_file, str);
			ne_file << neshifraciya(str, key);
			ne_file << "\n";
		}
		shifro_file.close();
		ne_file.close();
	}
};

class Gamming : public Eboy  
{
private:

	vector<int> poryadok;

	bool value_check(int& value)
	{
		if (poryadok.size() != 0)
		{
			for (int i = 0; i < poryadok.size(); i++)
			{
				if (poryadok[i] == value)
					return false;
				if (i == poryadok.size() - 1 && poryadok[i] != value)
				{
					poryadok.push_back(value);
					return true;
				}
			}
		}
		else
		{
			poryadok.push_back(value);
			return true;
		}
	}

	string shifraciya(string str, json key)
	{
		string sh_str;
		string sh_elem;
		int size = key.at("key").at("dlina");
		int last = key.at("key").at("alph").size();
		for (int i = 0; i < str.size(); i++)
		{
			for (int j = 0; j < key.at("key").at("alph").size(); j++)
			{
				string a = key.at("key").at("alph").at(j).at(0);
				if (str[i] == a[0])
				{
					int counter;
					if (i >= size)
						counter = i % size;
					else
						counter = i;

					int res;
					int x = key.at("key").at("alph").at(j).at(1);

					int y = key.at("key").at("znach").at(counter);

					res = (x + y) % last;
					sh_elem = key.at("key").at("alph").at(res).at(0);
					sh_str.push_back(sh_elem[0]);
					sh_elem.clear();
					break;
				}
			}
		}
		return sh_str;
	}

	string neshifraciya(string str, json key)
	{
		string ne_str;
		string ne_elem;
		int size = key.at("key").at("dlina");
		int last = key.at("key").at("alph").size();
		for (int i = 0; i < str.size(); i++)
		{
			for (int j = 0; j < key.at("key").at("alph").size(); j++)
			{
				string a = key.at("key").at("alph").at(j).at(0);
				if (str[i] == a[0])
				{
					int counter;
					if (i >= size)
						counter = i % size;
					else
						counter = i;

					int res;

					int y = key.at("key").at("znach").at(counter);

					res = (j - y);
					while (res < 0)
					{
						res += last;
					}
					res %= last;

					ne_elem = key.at("key").at("alph").at(res).at(0);
					ne_str.push_back(ne_elem[0]);
					ne_elem.clear();
					break;
				}
			}
		}
		return ne_str;
	}

public:

	void gen_key(string& gde_alph, string& kuda_sohr)
	{
		{
			setlocale(LC_ALL, "ru");
		};
		ifstream alph_file(gde_alph);
		if (!alph_file.is_open())
		{
			cout << "\n\nНе получается открыть файлик!\n\n";
			exit(0);
		}
		json alph;
		json key = { {"tip", "gamming"},{"key",  {  {"alph",  {}  }, {"dlina",    {}   },     {"znach",     {}    }     }    } };
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
			key.at("key").at("dlina") = block_length;

			for (int i = 0; i < block_length; i++)
			{
				value_elem = rand() % 1000;
				if (value_check(value_elem) == true)
				{
					key.at("key").at("znach").push_back(value_elem);
				}
				else
					i--;
			}

			ofstream _dot_key(kuda_sohr);
			_dot_key << key;
			_dot_key.close();
		}
		else
		{
			cout << "\n\nУстрани проблемы с файлом!\n\n";
			exit(0);
		}
	}

	void Shifr(string& gamm_key_put, string& gde_sohranit, string& chto_za_ne)

	{
		{
			setlocale(LC_ALL, "ru");
		};
		fstream key_file(gamm_key_put);
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

		ofstream sh_file(gde_sohranit);

		fstream default_file(chto_za_ne);
		if (!default_file.is_open())
		{
			cout << "\n\nС твоим файлом беда\n\n";
			exit(0);
		}

		string str;

		while (!default_file.eof())
		{
			getline(default_file, str);
			sh_file << shifraciya(str, key);
			sh_file << "\n";
		}
		default_file.close();
		sh_file.close();
	}

	void Neshifr (string& gamm_key_put, string& gde_sohranit, string& chto_za_ne)
	{
		fstream key_file(gamm_key_put);
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

		ofstream ne_file(gde_sohranit);

		fstream shifro_file(chto_za_ne);
		if (!shifro_file.is_open())
		{
			cout << "\n\nЯ так не играю\n\n";
			exit(0);
		}

		string str;

		while (!shifro_file.eof())
		{
			getline(shifro_file, str);
			ne_file << neshifraciya(str, key);
			ne_file << "\n";
		}
		shifro_file.close();
		ne_file.close();
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
				cout << "\n\nВот что имеем:\n1)Можно заменить твои буковки на кое-какие другие\n2)Поменяем твои буквы местами\n3)Метод гения инженерной мысли\n\nВперёд: ";
				cin >> x;
				if (x == 1)
				{
					x = 0;

					cout << "\n\nДавай сюда путь к файлу в формате txt: "; string chto_za_sh; cin >> chto_za_sh;


					cout << "Теперь путь к ключику в формате key: "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nЧто-то ты мне тут шляпу подсунул, давай по новой\n\n";
						return 0;
					}

					cout << "И на последок путь к новому дому, вот в таком формате - encrypt: "; string kuda_sohr; cin >> kuda_sohr;
					if (kuda_sohr.substr(kuda_sohr.size() - 8, 8) != ".encrypt")
					{
						cout << "\n\nТы налагал, прочти выше как надо\n\n";
						return 0;
					}
					Zamena rep;
					rep.Shifr(key, kuda_sohr, chto_za_sh);
				}
				else if (x == 2)
				{
					x = 0;

					cout << "\n\nНу раз такое дело, давай путь к своим игрушкам ( файл в формате txt должен быть): "; string chto_za_sh; cin >> chto_za_sh;


					cout << "Теперь путь к ключику в формате key соответственно: "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nТы что-то напутал, давай заново\n\n";
						return 0;
					}

					cout << "Куда тебе его сохранить? Формат encrypt нужен: "; string kuda_sohr; cin >> kuda_sohr;
					if (kuda_sohr.substr(kuda_sohr.size() - 8, 8) != ".encrypt")
					{
						cout << "\n\nНе, не пойдет, почитай ещё раз как надо\n\n";
						return 0;
					}
					Perestanovka rearr;
					rearr.Shifr(key, kuda_sohr, chto_za_sh);
				}
				else if (x == 3)
				{
					x = 0;

					cout << "\n\nДавай сюда путь к файлу в формате txt, засекретим тебе: "; string chto_za_sh; cin >> chto_za_sh;


					cout << "Теперь к ключику в формате key: "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nВсё фигня, давай по новой\n\n";
						return 0;
					}

					cout << "Сохранять будем в формате encrypt, только куда? "; string kuda_sohr; cin >> kuda_sohr;
					if (kuda_sohr.substr(kuda_sohr.size() - 8, 8) != ".encrypt")
					{
						cout << "\n\nПерепутал что-то, давай еще раз\n\n";
						return 0;
					}
					Gamming gamm;
					gamm.encrypt(key, kuda_sohr, chto_za_sh);
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

					cout << "\n\nВведи путь к абракадабре в формате encrypt: "; string chto_za_ne; cin >> chto_za_ne;


					cout << "Теперь елюч в формате key: "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nНеправильно ввёл что-то\n\n";
						return 0;
					}

					cout << "Куда хочешь сохранить? Формат encrypt.txt: "; string kuda_sohr; cin >> kuda_sohr;

					Zamena rep;
					rep.Neshifr(key, kuda_sohr, chto_za_ne);
				}
				else if (x == 2)
				{
					x = 0;

					cout << "\n\nВведи путь к нелепице в формате encrypt: "; string chto_za_ne; cin >> chto_za_ne;

					cout << "Введи путь к ключику от нелепицы в формате key: "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nВсё фигня, давай по новой\n\n";
						return 0;
					}

					cout << "Куда будем сохранять? В encrypt.txt только: "; string kuda_sohr; cin >> kuda_sohr;

					Perestanovka rearr;
					rearr.Neshifr(key, kuda_sohr, chto_za_ne);
				}
				else if (x == 3)
				{
					x = 0;

					cout << "\n\nВведи путь к белиберде в формате encrypt: "; string chto_za_ne; cin >> chto_za_ne;


					cout << "Теперь антибелеберду в формате key: "; string key; cin >> key;
					if (key.substr(key.size() - 4, 4) != ".key")
					{
						cout << "\n\nВсё фигня, давай по новой\n\n";
						return 0;
					}

					cout << "Сохранять будем в формат encrypt.txt, только куда? "; string kuda_sohr; cin >> kuda_sohr;

					Gamming gamm;
					gamm.Neshifr(key, kuda_sohr, chto_za_ne);
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


				cout << "\n\nУкажи путь к файлику в формате alph: "; string alph; cin >> alph;
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
				Zamena rep;
				rep.gen_key(alph, key);
			}
			else if (x == 2)
			{
				x = 0;


				cout << "\n\nПуть для сохранения ключика в формате key: "; string key; cin >> key;
				string empty = "";
				Perestanovka rearr;
				rearr.gen_key(empty, key);
			}
			else if (x == 3)
			{
				x = 0;

				cout << "\n\nВведи путь к алфавиту в фомате alph: "; string alph; cin >> alph;
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
				cout << "\n\nКуда сохранить ключ? Формат key: "; string key; cin >> key;
				Gamming gamm;
				gamm.gen_key(alph, key);
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