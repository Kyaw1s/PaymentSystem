#include <iostream>
#include <fstream>
#include <windows.h>
#include <cstdio>
#include <filesystem>
#include <string>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

class Cards
{
private:

    static string PaymentSystems[10];

public:
    long long list[100] = {};
    int count = 0;
    long long number = 0;

    static string PaymentSystem(long long number)
    {
        return PaymentSystems[number % 10];
    }
};

string Cards::PaymentSystems[10] = { "Государственный сектор", "Diners Club enRoute", "МИР" , "American Express",
   "VISA" , "MasterCard" , "Maestro" , "УЭК" , "Телекоммуникация" , "Государственный сектор" };

Cards cards;

class PrintMeneger
{
public:
    static void MessageWithColor(string str, int color)
    {
        SetConsoleTextAttribute(h, color);
        cout << str << endl;
        SetConsoleTextAttribute(h, 7);
    }

    static void Greting()
    {
        SetConsoleTextAttribute(h, 2);
        cout << "#######################################" << endl;
        cout << " \"BANK-KROT v1.0\" - MSHP inc. (c) 2020" << endl;
        cout << "#######################################" << endl << endl;
        cout << "Welcome to the \"Bank-krot\"!!!" << endl << "It is bank management system made with love." << endl << endl;
        cout << "---------------------------------------" << endl;
        cout << "Some useful commands:" << endl << endl;
        cout << "\t[1] add <card_number>\tadding a new card to the database" << endl;
        cout << "\t[2] balance <card_number>\tchecking card balance" << endl;
        cout << "\t[3] update <card_number> <new_value>\tupdating card balance" << endl;
        cout << "\t[4] remittance <card_number> <other_card_number> <new_value>\ttransfer from one card to another" << endl;
        cout << "\t[5] delete <card_number>\tdeletes the client by card number" << endl;
        cout << "\t[6] display\tshows all clients" << endl;
        cout << "\t[7] settings" << endl;
        cout << "---------------------------------------" << endl << endl << endl;
        SetConsoleTextAttribute(h, 7);
    }

    static void InfoRemittance(long long sender, long long recipient, int cofficient)
    {
        cout << cards.PaymentSystem(sender) + " ----> " + cards.PaymentSystem(recipient) << endl;
        string str = "Comission: " + to_string(cofficient) + "%";
        if (cofficient == 0) MessageWithColor(str, 2);
        else MessageWithColor(str, 4);
    }

    static void DisplayAllCard(Cards& cards)
    {
        cout << endl;
        for (int i = 0; i < cards.count; i++)
            cout << "\t" << i + 1 << ". " << cards.list[i] << "\t" << cards.PaymentSystem(cards.list[i]) << endl;
        cout << endl;
    }

    static void DisplaySettings()
    {
        cout << endl << "[1] Отображать всё в рублях(руб.)" << endl;
        cout << "[2] Отображать всё в долларах($)" << endl;
        cout << "[3] Отображать всё в Ерво(EUR)" << endl << endl;
    }
};

class CardCorrectMeneger
{
public:
    static bool IsNumberCorrect(long long n)
    {
        return IsLuhnCorrect(n) && GetDigitCount(n) == 16;
    }

    static bool IsCorrectCmd(string cmd)
    {
        for (int i = 0; i < 10; i++) if (isCorrectCmd[i] == cmd) return true;
        return false;
    }

    static void CycleEnterNumber(long long& number, bool flag)
    {
        while (true)
        {
            cout << "Card number: ";
            cin >> number;
            if (number == cards.number && flag) PrintMeneger::MessageWithColor("You can't translate to yourself", 4);
            else if (CardCorrectMeneger::IsNumberCorrect(cards.number)) break;
            else PrintMeneger::MessageWithColor("Incorrect card Number", 4);
        }
    }
    static void CycleEnterNumber(int& number, string[])
    {
        while (true)
        {
            cin >> number;
            if (number <= 3 && number >= 1) break;
            else PrintMeneger::MessageWithColor("Incorrect number", 4);
        }
    }
private:
    static string isCorrectCmd[10];

    static int GetDigitCount(long long n)
    {
        int counter = 0;
        while (n > 0)
        {
            n /= 10;
            counter++;
        }
        return counter;
    }

    static bool IsLuhnCorrect(long long n)
    {
        int sum = 0;
        for (int i = 0; i < 16; i++)
        {
            if (i % 2 == 1)
            {
                if (n % 10 * 2 > 9) sum += (n % 10) * 2 % 10 + (n % 10) * 2 / 10;
                else sum += n % 10 * 2;
            }
            else sum += n % 10;
            n /= 10;
        }
        return sum % 10 == 0;
    }
};

string CardCorrectMeneger::isCorrectCmd[10] = {"1", "2", "3", "4", "5",
    "add", "balance", "update", "remittance", "delete"};

class CardFileMeneger
{
public:

    static void CreateFileWithBalance(Cards& cards)
    {
        for (int i = 0; i < cards.count; i++)
        {
            ofstream fout(CardFileMeneger::GetFileName(cards.list[i]));
            fout << rand() * 10;
            fout.close();
        }
    }

    static void SaveList(Cards& cards)
    {
        ofstream fout("db.txt");
        fout << cards.count << endl;
        for (int i = 0; i < cards.count; i++) fout << cards.list[i] << endl;
        fout.close();
    }

    static void LoadList(Cards& cards)
    {
        ifstream fin("db.txt");
        fin >> cards.count;
        for (int i = 0; i < cards.count; i++) fin >> cards.list[i];
        fin.close();
    }

    static string GetFileName(long long number)
    {
        string number_s = "";
        while (number != 0)
        {
            char digit = '0' + number % 10;
            number_s = digit + number_s;
            number /= 10;
        }
        return "Cards\\" + number_s + ".txt";
    }

    static void DeleteCardFile(Cards& cards);
};

class CardOperations
{
private:

    static int GetCurrency()
    {
        return cofficient[flag - 1];
    }

public:

    static int flag;
    static int cofficient[3];
    static string symbol[3];

    static double GetBalance(long long number)
    {
        ifstream fin(CardFileMeneger::GetFileName(number));
        int balance;
        fin >> balance;
        fin.close();
        return balance / GetCurrency();
    }

    static void BalanceUpdate(long long number, int newValue)
    {
        int balance = (int)GetBalance(number);
        ofstream fout(CardFileMeneger::GetFileName(number));
        fout << (balance + newValue) * GetCurrency();
        fout.close();
    }

    static int Find(Cards& cards)
    {
        for (int i = 0; i < cards.count; i++) if (cards.list[i] == cards.number) return i;
        return -1;
    }

    static void Add(Cards& cards)
    {
        cards.list[cards.count] = cards.number;
        cards.count++;
        ofstream fout(CardFileMeneger::GetFileName(cards.number));
        fout << 0;
        fout.close();
        CardFileMeneger::SaveList(cards);
    }

    static void Remittance(long long sender, long long recipient, int amount)
    {
        int Balance = (int)GetBalance(sender);
        ofstream fout(CardFileMeneger::GetFileName(sender));
        fout << (Balance - amount) * GetCurrency();
        fout.close();
        Balance = (int)GetBalance(recipient) * GetCurrency();
        fout.open(CardFileMeneger::GetFileName(recipient));
        fout << (Balance + (amount - amount / 100 * GetCommision(sender, recipient))) * GetCurrency();
        fout.close();
    }

    static int GetCommision(long long sender, long long recipient)
    {
        return (int)abs(sender % 10 - recipient % 10);
    }

};

int CardOperations::cofficient[3] = { 1, 72, 82 };
int CardOperations::flag = 1;
string CardOperations::symbol[3] = { "руб.", "$", "EUR" };

CardOperations Currency;

void CardFileMeneger::DeleteCardFile(Cards& cards)
{
    remove(CardFileMeneger::GetFileName(cards.number).c_str());
    int index = CardOperations::Find(cards);
    for (int i = index; i < cards.count - 1; i++) cards.list[i] = cards.list[i + 1];
    cards.count--;
    CardFileMeneger::SaveList(cards);
}

int main()
{
    setlocale(LC_ALL, "ru");
    srand(time(NULL));

    PrintMeneger::Greting();

    CardFileMeneger::LoadList(cards);
    CardFileMeneger::CreateFileWithBalance(cards);

    cout << "Enter a command: ";
    string cmd;
    cin >> cmd;

    while (cmd != "exit")
    {
        if (CardCorrectMeneger::IsCorrectCmd(cmd))CardCorrectMeneger::CycleEnterNumber(cards.number, false);

        if (cmd == "add" || cmd == "1")
        {
            if (CardOperations::Find(cards) == -1)
            {
                CardOperations::Add(cards);
                CardOperations::BalanceUpdate(cards.number, 0);
                PrintMeneger::MessageWithColor("Done!", 2);
            }
            else PrintMeneger::MessageWithColor("This bank card already exists", 4);
        }

        else if (cmd == "balance" || cmd == "2")
        {
            if (CardOperations::Find(cards) != -1)
                cout << Cards::PaymentSystem(cards.number) << endl << CardOperations::GetBalance(cards.number) << " " << Currency.symbol[Currency.flag-1] << endl;
            else
                PrintMeneger::MessageWithColor("Card does not exist", 4);
        }

        else if (cmd == "update" || cmd == "3")
        {
            int newValue;
            PrintMeneger::MessageWithColor("Enter the amount: ", 2);
            cin >> newValue;
            if (CardOperations::Find(cards) != -1 && newValue > 0)
            {
                CardOperations::BalanceUpdate(cards.number, newValue);
                PrintMeneger::MessageWithColor("Done!", 2);
            }
            else PrintMeneger::MessageWithColor("Incorrect number", 4);
        }

        else if (cmd == "remittance" || cmd == "4")
        {
            long long newCardNumber;
            int amount;
            CardCorrectMeneger::CycleEnterNumber(newCardNumber, true);
            PrintMeneger::InfoRemittance(cards.number, newCardNumber, CardOperations::GetCommision(cards.number, newCardNumber));
            cout << "Transfer amount: " << endl;
            cin >> amount;
            if (amount > CardOperations::GetBalance(cards.number))
                PrintMeneger::MessageWithColor("Insufficient funds", 4);
            else if (amount <= 0)
                PrintMeneger::MessageWithColor("Incorrect number", 4);
            else
            {
                CardOperations::Remittance(cards.number, newCardNumber, amount);
                PrintMeneger::MessageWithColor("Done!", 2);
            }

        }

        else if (cmd == "delete" || cmd == "5")
        {
            if (CardOperations::Find(cards) != -1)
            {
                CardFileMeneger::DeleteCardFile(cards);
                PrintMeneger::MessageWithColor("Done!", 2);
            }
            else
                PrintMeneger::MessageWithColor("Card does not exist", 4);
        }

        else if (cmd == "display" || cmd == "6")
            PrintMeneger::DisplayAllCard(cards);

        else if (cmd == "settings" || cmd == "7")
        {
            PrintMeneger::DisplaySettings();
            CardCorrectMeneger::CycleEnterNumber(Currency.flag, Currency.symbol);
        }

        else
            PrintMeneger::MessageWithColor("Incorrect comand", 4);

        cout << "Enter a command: ";
        cin >> cmd;
    }

    CardFileMeneger::SaveList(cards);
    cout << "Bye!";

    return 0;
}
