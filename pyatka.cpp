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

bool check(string pathtofile)  
{
    bool enable = 0;
    ifstream file(pathtofile); 
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
    ifstream file(pathtofile);  
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
void swap(vector<int>& a, int i, int j)  
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
    return true;  
}
void decrypt(vector<int>a)  
{
    int k = 0;   
    int n = 0;   
    vector<char>tempdecryptword(encrypttext.size()); 
    for (int i = 0; i < encrypttext.size(); i++)  
    {   
        if (n > a.size() - 1)   
        {                             
            n = 0;
            k += a.size();
        }
        tempdecryptword[(a[n] - 1) + k] = encrypttext[i]; 
        n++;                                                                                                    
    }                         
    vector<string> words;  
    string tempword;
    for (int i = 0; i < tempdecryptword.size(); i++) 
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
    for (int k = 0; k < words.size(); k++)   
    {
        for (int i = 0; i < dictionary.size(); i++)
        {
            if (words[k] == dictionary[i]) 
            {
                count++;
                break;
            }
        }
        if (count >= (words.size() / 1.5))  
        {
            decryptword = tempdecryptword;
            resultkey = a;
            flag = 0;  
            cout << "Текст расшифрован" << endl;
            triger.notify_one();
            return;
        }
    }
}
void decryption(int size)
{
    vector<int> a(size); 
    for (int i = 0; i < size; i++)
        a[i] = i + 1;
    while (NextSet(a, size))  
    {
        decrypt(a);
    }
}
class threadpool
{
private:
    vector<thread> threads;
    void forThread() 
    {
        while (flag)  
        {

            for (int i = statuses.size() - 1; i >= 0; i--) 
            {
                unique_lock<mutex>mtx2(blocker);
                if (statuses[i] == 0)  
                {
                    statuses[i] = 1; 
                    mtx2.unlock();  
                    decryption(keyvar[i]); 
                    mtx2.lock();
                    break;
                }
            }
        }
    }

public:
    void run(int maxThreads = thread::hardware_concurrency() - 2) 
    {
        for (int i = 0; i < maxThreads; i++)
        {
            threads.push_back(thread(&threadpool::forThread, this));
            threads.back().detach();       
        }
    }
};
void start()                
{
    setlocale(0, "");
    string pathtotext;
    cout << "Укажите путь к файлу с текстом: ";
    getline(cin, pathtotext);  

    if (check(pathtotext) == 0) 
        return;

    int i = 0;
    string temp;
    bool triger2 = 0;
    while (pathtotext[i] != '\0')  
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
        cout << ">> Ошибка:: файл c текстом должен быть формата .txt\n";
        return;
    }
    ifstream text(pathtotext);  
    string temptowrite;
    while (getline(text, temp))  
    {
        encrypttext += temp;
    }
    for (int i = encrypttext.size(); i > 0; i--)    
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
    statuses.resize(keyvar.size());     
    for (int i = 0; i < statuses.size(); i++)                                                       
    {
        statuses[i] = 0;   
    }
    cout << "Укажите путь к файлу со словорём: ";
    getline(cin, pathtotext);

    if (check(pathtotext) == 0)
        return;

    i = 0;
    temp.clear();
    triger2 = 0;
    while (pathtotext[i] != '\0')      
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

    ifstream dictionary_to_read("alph.txt");    
    string tempd;                                                    
    while (getline(dictionary_to_read, tempd))
    {
        dictionary.push_back(tempd);
    }
    dictionary_to_read.close();

    threadpool x;        
    x.run();             

    unique_lock<mutex>mtx(blocker);
    triger.wait(mtx);  

    cout << "Укажите путь куда сохранить файл(.txt): ";
    getline(cin, pathtotext);

    string encrtowrite;
    string keytowrite;
    for (int i = 0; i < decryptword.size(); i++)
    {
        encrtowrite += decryptword[i];
    }
    for (int i = 0; i < resultkey.size(); i++) 
    {
        keytowrite += to_string(resultkey[i]); 
    }
    ofstream text_to_write(pathtotext);  
    text_to_write << "Ключ: ";  
    text_to_write << keytowrite << endl; 
    text_to_write << encrtowrite;  
    text_to_write.close(); 
    check2(pathtotext);  
}

int main()
{
    start();
}