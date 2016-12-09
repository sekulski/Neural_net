#include "teacher.h"

Teacher::Teacher(vector<double> &netChar)
{
    NETCHAR = netChar;
    createRespAndTopo();
    networkToTeach = new Net(TOPOLOGY, NETCHAR);
    teachNetwork();

    for(unsigned x = 0; x < TOPOLOGY.back(); x++)
    {
        cout << "wyjscie " << x+1 << endl;
        networkToTeach->feedForward(SIGNALS[x]);
        networkToTeach->drawNetwork(false,true);
    }
}

void Teacher::createRespAndTopo()
{
    fstream toTeach;
    string takenLine;

    while(true)
    {
        system("cls");
        cout << "Wprowadz nazwe pliku z danymi wejsciowymi z folderu Signals " << endl;
        string fileName, fileDir = "Signals/";
        getline(cin, fileName);
        fileDir +=fileName + ".txt";

        toTeach.open(fileDir, ios::in);

        if(toTeach.good() == true)
        {
            break;
        }
        else
        {
            cout << "Zla sciezka / brak dostepu do pliku" << endl;
            getchar();
        }
    }

    getline(toTeach, takenLine); // Pomijamy wyraz TOPOLOGY

    getline(toTeach, takenLine);

    while(!takenLine.empty())
    {
        unsigned poz = takenLine.find_first_of(";");     // Znajdź pierwszy znak spec
        takenLine.erase(poz, poz-1);                     // Usuń go
        string subLine = takenLine.substr(0,poz);        // Zrób substring
        takenLine.erase(0, poz);                         // Wyrzuć substring z głównego stringu
        TOPOLOGY.push_back(stoul(subLine));
    }

    getline(toTeach, takenLine);

    unsigned lineCounter = 3;
    while(!toTeach.eof())
    {
        getline(toTeach, takenLine);

        SIGNALS.push_back(LETTER());                         // Robimy wektor dla litery
        while(!takenLine.empty())
        {
            unsigned poz = takenLine.find_first_of(";");      // Znajdź pierwszy znak spec
            takenLine.erase(poz, poz);                        // Usuń go
            string subLine = takenLine.substr(0,poz);         // Zrób substring
            takenLine.erase(0, poz);                          // Wyrzuć substring z głównego stringu
            SIGNALS.back().push_back(stod(subLine));          // Wrzucamy do litery jej dane
        }

        if(SIGNALS.back().size() != TOPOLOGY[0])
        {
            cout << "Struktura danych a ilosc neuronow wejsciowych - NIEZGODNE\nLINIA: " << lineCounter << endl;
            //return 0;
        }

        lineCounter++;
    }
    if(SIGNALS.size() != TOPOLOGY.back())
    {
        cout << "Ilosc sygnalow uczacych nie jest zgodna z liczba wyjsc sieci" << endl;
    }
    toTeach.close();


    // Generuję sygnały uczące
    unsigned counter = 0;
    for(unsigned letters = 0; letters < SIGNALS.size(); letters++)
    {
        RESPONSES.push_back(LETTER());
        for(unsigned bits = 0; bits < SIGNALS.back().size(); bits++)
        {
            if(counter == bits)
            {
                RESPONSES.back().push_back(1);
            }
            else
            {
                RESPONSES.back().push_back(0);
            }
            counter++;
        }
    }


}

void Teacher::teachNetwork()
{
    int epoch = 0;
    bool flag = true;

    while(flag)
    {
        epoch++;
        int randS = rand()%TOPOLOGY.back();
        //cout << randS << endl;
        networkToTeach->feedForward(SIGNALS[randS]);
        networkToTeach->backProp(RESPONSES[randS]);

        if(epoch == 4000)
        flag = false;
    }
    cout << "EPOCH " << epoch << endl;
}
