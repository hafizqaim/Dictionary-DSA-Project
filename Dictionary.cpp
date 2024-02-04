#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include <Windows.h>
using namespace std;

void dictionary();
void animationLoading();


int hashSizeDefinition()
{
    ifstream file("Dictionary.txt");
    if (!file.is_open()) {
        cout << "File Not Open" << endl;
        return -1;
    }

    string word;
    int count = 0;
    while (file >> word)
        count++;

    int capacity = count;
    float loadFactor = 0;
    while (1) {
        loadFactor = (float)count / capacity;
        if (loadFactor <= 0.7)
            break;
        capacity++;
    }

    file.close();
    return capacity;
}

struct DictionaryNode
{
    string word;
    string meaning;
    DictionaryNode():word(""),meaning(""){}
};

class Dictionary
{
    DictionaryNode* Dict;
    int capacity;

public:
    Dictionary(int size)
    {
        capacity = size;
        Dict = new DictionaryNode[capacity];
    }

    ~Dictionary()
    {
        delete[] Dict;
    }

    void loadDictionary()
    {
        ifstream file("Dictionary.txt");
        if (!file.is_open())
        {
            cout << "Unable to open file" << endl;
            Beep(1686, 500);
            return;
        }
        Beep(1686, 500);

        string word, mean, line;
        animationLoading();
        while (getline(file, line))
        {
            istringstream iss(line);
            iss >> word;
            // Extract and ignore the spaces between words
            while (iss.peek() == ' ')
            {
                iss.ignore();
            }
            // Extract the second word
            iss >> mean;
            addWordUniversal(word, mean);
        }
        file.close();
    }

    int hashFunc(const string& word)
    {
        int hash = 0;
        for (int i = 0; i < word.length(); i++) //the ascii of earch character of the string is added to a variable and then hash key is generated
            hash += word[i];

        return hash % capacity;
    }

    void addWordUniversal(const string& word, const string& meaning)
    {
        int hash = hashFunc(word);
        int limit = hash;
        bool isFull = true;

        for (int i = 0; i < capacity; ++i) {
            hash = (hash + i * i) % capacity; // Quadratic probing
            if (Dict[hash].word.empty()) {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            cout << "\t\t\t\t\033[31mDictionary is full. Cannot add more words." << endl;
            return;
        }

        Dict[hash].word = word;
        Dict[hash].meaning = meaning;
        
    }

    bool wordExists(const string& word)
    {
        int hash = hashFunc(word);
        int limit = hash;

        bool isFull = true;

        for (int i = 0; i < capacity; ++i) {
            hash = (hash + i * i) % capacity; // Quadratic probing
            if (Dict[hash].word.empty()) {
                isFull = false;
                break;
            }
            if (Dict[hash].word == word) {
                return true;
            }
        }

        return false;
    }

    string getMeaning(const string& word)
    {
        int hash = hashFunc(word);
        int limit = hash;

        for (int i = 0; i < capacity; ++i) {
            hash = (hash + i * i) % capacity; // Quadratic probing
            if (Dict[hash].word == word) {
                return Dict[hash].meaning;
            }
        }

        return "";
    }

    void wordUpdate()
    {
        string word, mean;
        cout << "\t\t\t\t\033[31mEnter the Word you want to Update: ";
        cin >> word;
        Beep(1686, 500);

        for (int i = 0; i < word.length(); i++)
            word[i] = tolower(word[i]);

        if (wordExists(word))
        {
            cout << "\t\t\t\t\033[36mEnter the New Meaning of " << word << " : ";
            cin.ignore(); getline(cin, mean);
            Beep(1686, 500);
            int hash = hashFunc(word);
            int limit = hash;

            for (int i = 0; i < capacity; ++i) {
                hash = (hash + i * i) % capacity; // Quadratic probing
                if (Dict[hash].word == word) {
                    char temp;
                    cout << "\t\t\t\t\033[36mDo you want to update the meaning of " << word << " from " << Dict[hash].meaning << " to " << mean << " ? (Y/N) : ";
                    cin >> temp;

                    while (temp != 'Y' && temp != 'y' && temp != 'N' && temp != 'n')
                    {
                        cout << "\t\t\t\t\033[31mWrong Input! Enter Again: "; cin >> temp;
                    }

                    if (temp == 'y' || temp == 'Y')
                    {
                        Dict[hash].meaning = mean;
                        cout << "\t\t\t\t\033[36mMeaning of " << word << " has successfully been updated!" << endl;
                        return;
                    }
                    else
                    {
                        cout << "\t\t\t\t\033[36m" << word << " hasn't been updated!" << endl;
                        return;
                    }
                }
            }

            cout << "\t\t\t\t\033[31mWord couldn't be found in the dictionary :)" << endl;
        }
        else
            cout << "\t\t\t\t\033[36m" << word << " couldn't be Found in the Dictionary" << endl;
    }

    void wordDelete()
    {
        string word;
        cout << "\t\t\t\t\033[36mEnter the Word you want to Delete: ";
        getline(cin, word);

        for (int i = 0; i < word.length(); i++)
            word[i] = tolower(word[i]);

        if (wordExists(word))
        {
            int hash = hashFunc(word);
            int limit = hash;

            for (int i = 0; i < capacity; ++i) {
                hash = (hash + i * i) % capacity; // Quadratic probing
                if (Dict[hash].word == word) {
                    char temp;
                    cout << "\t\t\t\t\033[36mDo you want to delete " << word << " ? (Y/N): "; cin >> temp;
                    while (temp != 'Y' && temp != 'y' && temp != 'N' && temp != 'n')
                    {
                        cout << "\t\t\t\t\033[31mWrong Input! Enter Again: "; cin >> temp;
                    }

                    if (temp == 'Y' || temp == 'y')
                    {
                        Dict[hash].word = "";
                        Dict[hash].meaning = "";
                        cout << "\t\t\t\t\033[36m" << word << " has successfully been deleted!" << endl;
                        return;
                    }
                    else
                    {
                        cout << "\t\t\t\t\033[36m" << word << " hasn't been deleted!" << endl;
                        return;
                    }
                }
            }

            cout << "\t\t\t\t\033[31mWord couldn't be found in the dictionary :)" << endl;
        }
        else
            cout << "\t\t\t\t\033[36m" << word << " couldn't be Found in the Dictionary!" << endl;
    }
    
    void displayDictionary()
    {
        for (int i = 0; i < capacity; ++i)
            if (Dict[i].word != "")
                cout << "\t\t\t\t\033[36mWord: " << Dict[i].word << ", Meaning: " << Dict[i].meaning << endl;
    }

    void addWordManually()
    {
        string word = "";
        cout << "\t\t\t\t\033[36mEnter the Word to Add: ";
        getline(cin, word);
        Beep(1686, 500);
        for (int i = 0; i < word.length(); i++)
            word[i] = tolower(word[i]);

        if (wordExists(word))
        {
            cout << "\t\t\t\t\033[36mWord Already Exists in the Dictionary!" << endl;
            return;
        }

        string meaning = "";
        cout << "\t\t\t\t\033[36mEnter the Meaning of the Word: ";
        getline(cin, meaning);
        Beep(1686, 500);

        addWordUniversal(word, meaning);
        cout << "\t\t\t\t\033[36m" << word << " has been successfully been added to Dictionary!" << endl;
    }

    void wordSearch(string&word)
    {
        
        Beep(1686, 500);
        for (int i = 0; i < word.length(); i++)
            word[i] = tolower(word[i]);

        string meaning = getMeaning(word);
        if (!meaning.empty())
            cout << "\t\t\t\t\033[36mWord: " << word << ", Meaning: " << meaning << endl;
        else 
        {
            cout << "\t\t\t\t\033[36mWord not found in the dictionary." << endl;
            Beep(1686, 500);
        }
        Sleep(550);
        system("CLS");
    }

    void findWordSuggestions(const string& word)
    {
        string suggest[10];
        int count = 0;

        for (int i = 0; i < capacity && count < 10; ++i) {
            if (Dict[i].word.substr(0, word.length()) == word) { // Check if the word starts with the input word
                suggest[count++] = Dict[i].word;
            }
        }

        if (count > 0) {
            cout << "\n\n\t\t\t\t\033[36m-----------Suggestions---------" << endl;
            for (int i = 0; i < count; ++i) {
                cout << "\t\t\t\t\033[36m" << suggest[i] << endl;
            }
        }
        else {
            cout << "\n\n\t\t\t\t\033[36mNo Suggestions could be found!" << endl;
        }
    }
};

int main()
{
    dictionary();
    int size, defaultSize = hashSizeDefinition();
    char option;
    cout << "\n\n\t\033[31m Do you want to initialize the hash table size manually or automatically? (M/A): ";
    cin >> option;
    cin.ignore();
    Beep(1686, 500);
    cout << "\033[32m";

    if (option == 'M' || option == 'm') 
    {
        cout << "\t\t\t\t\033[31m Enter the size of the hash table: ";
        cin >> size;
        while (size)
            cin.ignore();
        Beep(1686, 500);
    }
    else if (option == 'A' || option == 'a')
        size = defaultSize;
    else 
    {
        size = defaultSize + 500;
        cout << "\t\t\t\t\033[31m Invalid option. Default size of " << size << " will be used." << endl;
    }

    Dictionary Dict(size);
    Dict.loadDictionary();

    char choice;
    while (true)
    {
        dictionary();
        cout << "\n";
        cout << "\t\t\t\t\033[31m==============================================" << endl;
        cout << "\t\t\t\t                    MENU                  " << endl;
        cout << "\t\t\t\t==============================================" << endl << endl;
        cout << "\t\t\t\t  \033[31m1. \033[32mSearch for a Word\n";
        cout << "\t\t\t\t  \033[31m2. \033[32mAdd a Word Manually\n";
        cout << "\t\t\t\t  \033[31m3. \033[32mDisplay Dictionary\n";
        cout << "\t\t\t\t  \033[31m4. \033[32mUpdate Word\n";
        cout << "\t\t\t\t  \033[31m5. \033[32mDelete Word\n";
        cout << "\t\t\t\t  \033[31m6. \033[32mExit\n";
        cout << "\t\t\t\t  Enter your choice: \033[31m";
        cin >> choice;
        cin.ignore();
        Beep(1686, 500);
        cout << "\033[32m";

        switch (choice)
        {
        case '1':
        {
            cout << "\n\t\t\t\t  Enter Word: ";
            string word;
            char ch;
            while ((ch = _getche()) != '\r')//the loop continues unless 'Enter Key' is pressed
            {
                system("cls");
                cout << "\t\t\t\t Enter Word : " << word;
                if (ch == '\b')//makes it user friendly and checks whether the entered key is a backspace or not 
                {
                    if (!word.empty())//and if the word is not empty then..
                    {
                        word.pop_back();//it removes the last character here
                        Dict.findWordSuggestions(word);
                    }
                }
                else
                {
                    cout << ch;
                    word = word + ch;//else the entered character is displayed on the console and added in the actual word as well

                    for (int i = 0; i < word.length(); i++)
                        word[i] = tolower(word[i]);

                    Dict.findWordSuggestions(word);
                }
            }
            cout << endl;
            string meaning = Dict.getMeaning(word);
            if (!meaning.empty())
                cout << "\t\t\t\t Word: " << word << " --> Meaning: " << meaning << endl << endl;
            else
                cout << "\t\t\t\t Word not found in the dictionary." << endl;

            cout << "\t\t\t\t "; system("pause");
            system("CLS");
            break;
        }
        case '2':
            Dict.addWordManually();
            Sleep(2000);
            system("CLS");
            break;
        case '3':
            Dict.displayDictionary();
            Sleep(2000);
            break;
        case '4':
            Dict.wordUpdate();
            Sleep(2000);
            system("CLS");
            break;
        case '5':
            Dict.wordDelete();
            Sleep(2000);
            system("CLS");
            break;
        case '6':
            cout << "\n\t\t\t\t  Exiting Application . . . . . . " << endl;
            Sleep(2000);
            system("CLS");
            return 0;

        default:
            cout << "\n\t\t\t\t  Invalid choice. Try again!" << endl;
            Sleep(2000);
            system("CLS");
        }
    }
}

void animationLoading() 
{
    system("cls");
    int c = 0;
    const int numFrames = 6;
    const string colors[] = { "\033[1;31m", "\033[1;32m", "\033[1;33m", "\033[1;34m", "\033[1;35m", "\033[1;36m" };

    while (c < 1) {
        for (int i = 0; i < numFrames; ++i) {
            cout << string(14, '\n') << string(50, ' ') << colors[i] << "Loading" << string(5, '.') << "\033[0m";
            Sleep(550);
            system("CLS");
        }
        c++;
    }
}

void dictionary()
{
    cout << "\t\033[31m ________   __     ______  ___________  __      ______    _____  ___        __        _______   ___  ___\n";
    cout << "\t\033[32m|\"      \"\\ |\" \\   /\" _  \"\\(\"     _   \")|\" \\    /    \" \\  (\\\"   \\|\"  \\      /\"\"\\      /\"      \\ |\"  \\\"/  |\n";
    cout << "\t\033[33m(.  ___  :)||  | (: ( \\___))__/  \\\\__/ ||  |  // ____  \\ |.\\\\   \\    |    /    \\    |:        | \\   \\  /\n";
    cout << "\t\033[34m|: \\   ) |||:  |  \\/ \\        \\\\_ /    |:  | /  /    ) :)|: \\.   \\\\  |   /' /\\  \\   |_____/   )  \\\\  \\/\n";
    cout << "\t\033[35m(| (___\\ |||.  |  //  \\ _     |.  |    |.  |(: (____/ // |.  \\    \\. |  //  __'  \\   //      /   /   /\n";
    cout << "\t\033[36m|:       :)/\\  |\\(:   _) \\    \\:  |    /\\  |\\\\        /  |    \\    \\ | /   /  \\\\  \\ |:  __   \\  /   /\n";
    cout << "\t\033[37m(________/(__\\_|_)\\_______)    \\__|   (__\\_|_)\\\"_____/    \\___|\\____\\)(___/    \\___)|__|  \\___)|___/\n";
    cout << "\t\033[0m";
}

    //Some repetitive functions that are used
    //Sleep is used to hold the current output for the given time e.g Sleep(2000) means 2 sec
    //Beep is used to make the beep sound with the given frequency and time period e.g Beep(1686, 500) means a sound of 1686 Hz for 0.5 sec

