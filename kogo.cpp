#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <condition_variable>
#include <functional>
#include <atomic>
using namespace std;

vector<int>keyvar;
vector<int> statuses;
string encrypttext;
vector<string>dictionary;
vector<char>decryptword;
vector<int> resultkey;
condition_variable triger;
mutex blocker;
bool flag = 1;

bool check(string pathtofile)  // проверка на то, существует файл или нет
{
    bool enable = 0;
    ifstream file(pathtofile);  //создаем переменную для открытия файла
    if (file.is_open())
    {
        enable = 1;
    }
    else
    {
        enable = 0;
        cout << ">> Ошибка:: файла не существует" << endl;
    }
    file.close();
    return enable;
}
void check2(string pathtofile)
{
    ifstream file(pathtofile);  // конец кода, проверяет сохр ли файл
    if (file.is_open())
    {
        cout << ">> Ваш файл был сохранён: " << pathtofile << endl;

    }
    else
    {
        cout << ">> Ошибка:: не удалось сохранить файл" << endl;
    }
    file.close();
    return;
}
void swap(vector<int>& a, int i, int j)  // перебирает разные варианты ключа
{
    int s = a[i];
    a[i] = a[j];
    a[j] = s;
}
bool NextSet(vector<int>& a, int n)
{
    int j = n - 2;
    while (j != -1 && a[j] >= a[j + 1]) j--;
    if (j == -1)
    {
        return false;
    }
    int k = n - 1;
    while (a[j] >= a[k]) { k--; }
    swap(a, j, k);
    int l = j + 1, r = n - 1;
    while (l < r)
    {
        swap(a, l++, r--);
    }
    return true;   // конец кода из инета
}
void decrypt(vector<int>a)
{
    int k = 0;   // показатель сколько блоков уже прошли 
    int n = 0;   // счетчик символов в блоке
    vector<char>tempdecryptword(encrypttext.size()); // временный расшифрованный текст, размером такой же как и зашифрованный
    for (int i = 0; i < encrypttext.size(); i++)  // цикл - расшифровка, здесь идем по тексту
    {     // ЗДЕСЬ МЫ ТИПА ДЕЛИМ НА БЛОКИ И ПЕРЕСТАВЛЯЕМ ПО КЛЮЧУ
        if (n > a.size() - 1)   // как только блок закончился он обнулится
        {
            n = 0;
            k += a.size();
        }
        tempdecryptword[(a[n] - 1) + k] = encrypttext[i]; // выитаем 1 потому что отсчет начинается с 0, на 3 элемент временного массива мы ставим
        n++;             // потом видим что блок закончился, обнудяем n и прибавляем k   82 и 85, затем идем с самого начала вектора, затем                                                                                            // 1 элемент основного
    }                          // преходим к следующему блоку для этого прибавляем k, теперь 4 элемент встанет на 6 место
    vector<string> words;  // массив слов, сюда записываются слова из расшифрованного текста
    string tempword;
    for (int i = 0; i < tempdecryptword.size(); i++) // бежит по тому, что рассшифровано, всё что до бробела переписывает в tempword
    {
        if (tempdecryptword[i] != ' ')
        {
            tempword += tempdecryptword[i];
        }
        else
        {
            words.push_back(tempword);
            tempword.clear();
        }
    }
    int count = 0;
    for (int k = 0; k < words.size(); k++)    // а здесь он сравнивает со словами из словаря
    {
        for (int i = 0; i < dictionary.size(); i++)
        {
            if (words[k] == dictionary[i]) // если нашел, то увеличивает счетчик
            {
                count++;
                break;
            }
        }
        if (count >= (words.size() / 1.5))  // если больше половины совпадений, то все круто
        {
            decryptword = tempdecryptword;
            resultkey = a;
            flag = 0;  // перестают бегать
            cout << "Текст расшифрован" << endl;
            triger.notify_one(); // сигнал что текст расшифрован ( ждали с 260)
            return;
        }
    }
}
void decryption(int size)
{
    vector<int> a(size);  // создаем массив из цифр размером с длину строки
    for (int i = 0; i < size; i++)
        a[i] = i + 1;
    while (NextSet(a, size))  // хрень из инета, крч перебирает ключ и ПЕРЕСТАНОВКА КЛЮЧА передаем переменную в декрипт и идем 75
    {
        decrypt(a);
    }
}
class threadpool
{
private:
    vector<thread> threads;
    void forThread()  // каждый поток выполняет эту функцию, пока флаг равен 1
    {
        while (flag)   // это метка что мы нашли файл, как только нашли файл стр 119
        {

            for (int i = statuses.size() - 1; i >= 0; i--)  // этим циклом пробегаем по статуса, ищем свободный вариант ключа
            {
                unique_lock<mutex>mtx2(blocker);
                if (statuses[i] == 0)  // начинаем перебирать, затем
                {
                    statuses[i] = 1;  // ставим статус 1 и его уже никто не трогает, это делается разными потоками
                    mtx2.unlock();  // один поток взял поменял статус, второй видит что статус =1 и тдет брать следующие и тд
                    decryption(keyvar[i]); // в этих потоках вызываем ф-ю decryption  и передаем вариант ключа стр 128
                    mtx2.lock();
                    break;
                }
            }
        }
    }

public:
    void run(int maxThreads = thread::hardware_concurrency() - 2) // определяем макс колво потоков( от колва ядер)
    {
        for (int i = 0; i < maxThreads; i++)
        {
            threads.push_back(thread(&threadpool::forThread, this));// массив threads(потоки) в него добавляем поток с функцией forThread
            threads.back().detach();          // не дожидаемся завершения                                                                       // ( поток выполняет эту ф-ю)
        }
    }
};
void start()                // 1 начало.
{
    setlocale(0, "");
    string pathtotext;
    cout << "Укажите путь к файлу с текстом: ";
    getline(cin, pathtotext);  // типа тот же син

    if (check(pathtotext) == 0) // эта проверка происходит в строчке 20, если функция вернула 0, значит файла не существует и мы завершаем
        return;

    int i = 0;
    string temp;
    bool triger2 = 0;
    while (pathtotext[i] != '\0')   // проверяет чтобы файл был именно .txt
    {
        if (pathtotext[i] == '.' && pathtotext[i + 1] == 't')  // идет по пути, пока не встретит точку и след символ t
        {
            triger2 = 1;
        }
        if (triger2 == 1) // как только переменная = 1, начинаем записывать символы из пути в дргую временную переменную temp
        {
            temp += pathtotext[i];  //  здесь записываем
        }
        i++;
    }
    if (temp != ".txt")
    {
        cout << ">> Ошибка:: файл c текстом должен быть формата .txt\n";
        return;
    }
    ifstream text(pathtotext);   // открываем файл формата по тому пути, который указал пользователь с ЗАШИФР текстом
    string temptowrite;
    while (getline(text, temp))  // в этом цикле просто переписывается содержание файла в переменную temtowrite
    {
        encrypttext += temp;
    }
    for (int i = encrypttext.size(); i > 0; i--)     // здесь пребираем всевозможные варианты ключа и записываем в переменную keyvar
    {
        if (i == 1)
        {
        }
        else if (encrypttext.size() % i == 0)
        {
            keyvar.push_back(i);
        }
    }
    text.close();
    statuses.resize(keyvar.size());        // статус для того чтобы фиксировать перебран не перебран ключ, чтобы поток не брали перебранные
    for (int i = 0; i < statuses.size(); i++)                                                       // размер status  равен keyvar
    {
        statuses[i] = 0;    // заполняем нулями, 0 это значит что мы его еще не перебрали
    }
    cout << "Укажите путь к файлу со словорём: ";
    getline(cin, pathtotext);

    if (check(pathtotext) == 0)
        return;

    i = 0;
    temp.clear();
    triger2 = 0;
    while (pathtotext[i] != '\0')      // снова проверка на txt
    {
        if (pathtotext[i] == '.' && pathtotext[i + 1] == 't')
        {
            triger2 = 1;
        }
        if (triger2 == 1)
        {
            temp += pathtotext[i];
        }
        i++;
    }
    if (temp != ".txt")
    {
        cout << ">> Ошибка:: файл со словорём должен быть формата .txt\n";
        return;
    }

    ifstream dictionary_to_read("alph.txt");    // одна строчка - одно слово, цикл считыввает все строчкии и записывает в dictionary, это массив 
    string tempd;                                                    // перезаписали алфавит в массив
    while (getline(dictionary_to_read, tempd))
    {
        dictionary.push_back(tempd);
    }
    dictionary_to_read.close();

    threadpool x;        // создаем класс
    x.run();             // run 

    unique_lock<mutex>mtx(blocker);
    triger.wait(mtx);  // ждем сигнал что текст расшифрован

    cout << "Укажите путь куда сохранить файл(.txt): ";
    getline(cin, pathtotext);

    string encrtowrite;
    string keytowrite;
    for (int i = 0; i < decryptword.size(); i++)
    {
        encrtowrite += decryptword[i];
    }
    for (int i = 0; i < resultkey.size(); i++) // вктор чар переписывается в стринг
    {
        keytowrite += to_string(resultkey[i]);  // тоже просто перезапишет формат переменной
    }
    ofstream text_to_write(pathtotext);  // создаст файл
    text_to_write << "Ключ: ";  // запишет сообшение ключ
    text_to_write << keytowrite << endl;  // запишет сам ключ
    text_to_write << encrtowrite;  // запишет текст
    text_to_write.close(); // закторе файл
    check2(pathtotext);  // проверит закрылся ли стр 37
}

int main()
{
    start();
}