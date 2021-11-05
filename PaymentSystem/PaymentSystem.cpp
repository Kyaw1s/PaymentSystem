#include <iostream>
#include <fstream>
#include <windows.h>
#include <cstdio>
#include <filesystem>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

class Cards
{
public:
    long long cardsList[100] = {};
    int cardsCount = 0;
    long long cardNumber = 0;

    static string PaymentSystem(long long number)
    {
        short i = number % 10;
        if (i == 1) return "Diners Club enRoute";
        else if (i == 2) return "МИР";
        else if (i == 3) return "American Express";
        else if (i == 4) return "VISA";
        else if (i == 5) return "MasterCard";
        else if (i == 6) return "Maestro";
        else if (i == 7) return "УЭК";
        else if (i == 8) return "Телекоммуникация";
        else return "Государственный сектор";
    }

};

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

    static void InfoRemittance(long long sender, long long recipient)
    {
        cout << cards.PaymentSystem(sender) + " ----> " + cards.PaymentSystem(recipient) << endl;
        if (sender % 10 == recipient % 10) MessageWithColor("Commission: 0%", 2);
        else MessageWithColor("Commission: 5%", 4);
    }

    static void DisplayAllCard(long long arr[], int size)
    {
        cout << endl;
        for (int i = 0; i < size; i++)
            cout << "\t" << i + 1 << ". " << arr[i] << "\t" << cards.PaymentSystem(arr[i]) << endl;
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
        return cmd == "add" || cmd == "1" || cmd == "balance" || cmd == "2" || cmd == "update" || cmd == "3"
            || cmd == "remittance" || cmd == "4" || cmd == "delete" || cmd == "5";
    }

    static void CycleEnterNumber(long long& number, bool flag)
    {
        while (true)
        {
            cout << "Card number: ";
            cin >> number;
            if (number == cards.cardNumber && flag) PrintMeneger::MessageWithColor("You can't translate to yourself", 4);
            else if (CardCorrectMeneger::IsNumberCorrect(cards.cardNumber)) break;
            else PrintMeneger::MessageWithColor("Incorrect card Number", 4);
        }
    }
    static void CycleEnterNumber(int& number, string& symbol)
    {
        while (true)
        {
            cin >> number;
            if (number <= 3 && number >= 1) break;
            else PrintMeneger::MessageWithColor("Incorrect number", 4);
        }
        if (number == 1) symbol = "руб.";
        else if (number == 2) symbol = "$";
        else symbol = "EUR";
    }
private:
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

class CardFileMeneger
{
public:

    static void CreateFileWithBalance(int size, long long arr[])
    {
        for (int i = 0; i < size; i++)
        {
            ofstream fout(CardFileMeneger::GetFileName(arr[i]));
            fout << rand() * 10;
            fout.close();
        }
    }

    static void SaveList(int n, long long arr[])
    {
        ofstream fout("db.txt");
        fout << n << endl;
        for (int i = 0; i < n; i++) fout << arr[i] << endl;
        fout.close();
    }

    static void LoadList(int& n, long long arr[])
    {
        ifstream fin("db.txt");
        fin >> n;
        for (int i = 0; i < n; i++) fin >> arr[i];
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

    static void DeleteCardFile(long long, long long[], int&);
};

class CardOperations
{
private:

    static int GetCommision(long long sender, long long recipient)
    {
        if (sender % 10 == recipient % 10) return 0;
        return 5;
    }

    static int GetCurrencyCofficient()
    {
        if (currency == 2) return 72;
        else if (currency == 3) return 82;
        return 1;
    }

public:

    static int currency;
    static string currencySymbol;

    static double GetBalance(long long number)
    {
        ifstream fin(CardFileMeneger::GetFileName(number));
        int balance;
        fin >> balance;
        fin.close();
        return balance / GetCurrencyCofficient();
    }

    static void BalanceUpdate(long long number, int newValue)
    {
        int balance = GetBalance(number);
        ofstream fout(CardFileMeneger::GetFileName(number));
        fout << (balance + newValue) * GetCurrencyCofficient();
        fout.close();
    }

    static int Find(long long number, long long arr[], int size)
    {
        for (int i = 0; i < size; i++) if (arr[i] == number) return i;
        return -1;
    }

    static void Add(long long number, long long arr[], int& size)
    {
        for (int i = 0; i < size; i++) if (arr[i] == number) return;
        arr[size] = number;
        size++;
        ofstream fout(CardFileMeneger::GetFileName(number));
        fout << 0;
        fout.close();
        CardFileMeneger::SaveList(size, arr);
    }

    static void Remittance(long long sender, long long recipient, int amount)
    {
        int Balance = GetBalance(sender);
        ofstream fout(CardFileMeneger::GetFileName(sender));
        fout << (Balance - amount) * GetCurrencyCofficient();
        fout.close();
        Balance = GetBalance(recipient) * GetCurrencyCofficient();
        fout.open(CardFileMeneger::GetFileName(recipient));
        fout << (Balance + (amount - amount / 100 * GetCommision(sender, recipient))) * GetCurrencyCofficient();
        fout.close();
    }
};

int CardOperations::currency = 1;
string CardOperations::currencySymbol = "руб.";

CardOperations Currency;

void CardFileMeneger::DeleteCardFile(long long number, long long arr[], int& size)
{
    remove(CardFileMeneger::GetFileName(number).c_str());
    int index = CardOperations::Find(number, arr, size);
    for (int i = index; i < size - 1; i++) arr[i] = arr[i + 1];
    size--;
    CardFileMeneger::SaveList(size, arr);
}

int main()
{
    setlocale(LC_ALL, "ru");
    srand(time(NULL));

    PrintMeneger::Greting();

    CardFileMeneger::LoadList(cards.cardsCount, cards.cardsList);
    CardFileMeneger::CreateFileWithBalance(cards.cardsCount, cards.cardsList);

    cout << "Enter a command: ";
    string cmd;
    cin >> cmd;

    while (cmd != "exit")
    {
        if (CardCorrectMeneger::IsCorrectCmd(cmd))CardCorrectMeneger::CycleEnterNumber(cards.cardNumber, false);

        if (cmd == "add" || cmd == "1")
        {
            if (CardOperations::Find(cards.cardNumber, cards.cardsList, cards.cardsCount) == -1)
            {
                CardOperations::Add(cards.cardNumber, cards.cardsList, cards.cardsCount);
                CardOperations::BalanceUpdate(cards.cardNumber, 0);
                PrintMeneger::MessageWithColor("Done!", 2);
            }
            else PrintMeneger::MessageWithColor("This bank card already exists", 4);
        }

        else if (cmd == "balance" || cmd == "2")
        {
            if (CardOperations::Find(cards.cardNumber, cards.cardsList, cards.cardsCount) != -1)
                cout << Cards::PaymentSystem(cards.cardNumber) << endl << CardOperations::GetBalance(cards.cardNumber) << " " << Currency.currencySymbol << endl;
            else
                PrintMeneger::MessageWithColor("Card does not exist", 4);
        }

        else if (cmd == "update" || cmd == "3")
        {
            int newValue;
            PrintMeneger::MessageWithColor("Enter the amount: ", 2);
            cin >> newValue;
            if (CardOperations::Find(cards.cardNumber, cards.cardsList, cards.cardsCount) != -1 && newValue > 0)
            {
                CardOperations::BalanceUpdate(cards.cardNumber, newValue);
                PrintMeneger::MessageWithColor("Done!", 2);
            }
            else PrintMeneger::MessageWithColor("Incorrect number", 4);
        }

        else if (cmd == "remittance" || cmd == "4")
        {
            long long newCardNumber;
            int amount;
            CardCorrectMeneger::CycleEnterNumber(newCardNumber, true);
            PrintMeneger::InfoRemittance(cards.cardNumber, newCardNumber);
            cout << "Transfer amount: " << endl;
            cin >> amount;
            if (amount > CardOperations::GetBalance(cards.cardNumber))
                PrintMeneger::MessageWithColor("Insufficient funds", 4);
            else if (amount <= 0)
                PrintMeneger::MessageWithColor("Incorrect number", 4);
            else
            {
                CardOperations::Remittance(cards.cardNumber, newCardNumber, amount);
                PrintMeneger::MessageWithColor("Done!", 2);
            }

        }

        else if (cmd == "delete" || cmd == "5")
        {
            if (CardOperations::Find(cards.cardNumber, cards.cardsList, cards.cardsCount) != -1)
            {
                CardFileMeneger::DeleteCardFile(cards.cardNumber, cards.cardsList, cards.cardsCount);
                PrintMeneger::MessageWithColor("Done!", 2);
            }
            else
                PrintMeneger::MessageWithColor("Card does not exist", 4);
        }

        else if (cmd == "display" || cmd == "6")
            PrintMeneger::DisplayAllCard(cards.cardsList, cards.cardsCount);

        else if (cmd == "settings" || cmd == "7")
        {
            PrintMeneger::DisplaySettings();
            CardCorrectMeneger::CycleEnterNumber(Currency.currency, Currency.currencySymbol);
        }

        else
            PrintMeneger::MessageWithColor("Incorrect comand", 4);

        cout << "Enter a command: ";
        cin >> cmd;
    }

    CardFileMeneger::SaveList(cards.cardsCount, cards.cardsList);
    cout << "Bye!";

    return 0;
}
