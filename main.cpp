#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "sqlite3.h"
using namespace std;

#include "MagicArtefact.h"

/////////////////Реализация функций контейнера ArtifactCont и MegaArtifactCont
ArtifactCont :: ArtifactCont(int maxSize)
{
    MaxSize = maxSize;
    ArtifactCount = 0;
    Artifacts = new MagicArtifactPtr[maxSize];
    if (Artifacts == nullptr)
    {
        wcout << L"Failed to allocate memory for magic artifacts!" << endl;
        return;
    }
}

ArtifactCont :: ~ArtifactCont()
{
    delete[] Artifacts;
}
MegaArtifactCont :: ~MegaArtifactCont()
{
    for (auto artifact : Artifacts)
        delete artifact;
}

void ArtifactCont :: AddArtifact(MagicArtifactPtr newArt)
{
    if (ArtifactCount < MaxSize)
        Artifacts[ArtifactCount++] = newArt;
    else
        wcout << L"Magic artifact container is full!" << endl;
}

MagicArtifactPtr ArtifactCont :: GetByIndex(int index) const
{
    if (index >= 0 && index < ArtifactCount)
        return Artifacts[index];
    else
        wcout << L"Index out of bounds!" << endl;
        return nullptr;
}

MagicArtifactPtr MegaArtifactCont :: GetByIndex(int index) const
{
    if (index >= 0 && index < Artifacts.size())
        return Artifacts[index];
}
////////////////

//ЗАДАНИЕ 8
//функция создает контейнер магических артефактов, наполняет его случайным количеством артефактов,
//использует итератор и декораторы для фильтрации артефактов по размеру и имени и выводит отфильтрованные результаты

void ArtifactFiltering()
{
    MagicArtifactFactoryContainer artifactContainer;
    int numArtifacts = rand() % 40;

    for (int i = 0; i < numArtifacts; ++i)
    {
        artifactContainer.CreateAndAddArtifact();
    }

    cout << "Number of artifacts created: " << artifactContainer.GetArtifactCount() << endl << endl << endl;

    int numArtifactsInContainer = artifactContainer.GetArtifactCount();//кол-во артефактов из контейнера

    //итератор для доступа к артефактам контейнера и создаем декораторы для фильтрации
    vector<MagicArtifactPtr> artifacts;

    for (int i = 0; i < numArtifactsInContainer; ++i)
    {
        MagicArtifactPtr artifact = artifactContainer.GetArtifactByIndex(i);
        artifacts.push_back(artifact);
    }

    ArtifContIter artifactIterator(&artifacts[0], numArtifactsInContainer);
    SizeFiltItDecorator sizeFilter(&artifactIterator, SizeArtefact::MegaArt);
    NameFiltItDecorator nameFilter(&artifactIterator, NameArtefact::MagicAmulet);

    //декораторы для фильтрации итератора
    cout << "Artifacts filtered by size:" << endl << endl;

    int artifactCount = 1;
    for (sizeFilter.First(); !sizeFilter.IsDone(); sizeFilter.Next(), ++artifactCount)
    {
        MagicArtifactPtr artifact = sizeFilter.GetCurrent();
        if (artifact)
        {
            cout << "Artifact " << artifactCount << ": " << nameToString(artifact->GetName())
            << ", Size: " << sizeToString(artifact->GetSize()) << endl;
        }
    }
    cout << endl << endl << endl;

    cout << "Artifacts filtered by name:" << endl << endl;
    artifactCount = 1;
    for (nameFilter.First(); !nameFilter.IsDone(); nameFilter.Next(), ++artifactCount)
    {
        MagicArtifactPtr artifact = nameFilter.GetCurrent();
        if (artifact)
        {
            cout << "Artifact " << artifactCount << ": " << nameToString(artifact->GetName())
            << ", Size: " << sizeToString(artifact->GetSize()) << endl;
        }
    }
}


//База данных

class DataBaseIt
{
private:
    sqlite3* Database;
    int index;
public:
    sqlite3_stmt *SQLPtr;

    DataBaseIt(sqlite3 *database)
    {
        Database = database;
        int execResult = sqlite3_prepare_v2(
                Database,
                "SELECT name, sizeA, WorkingCondition, SuperStrength FROM MagicArtifact",
                 -1,
                 &SQLPtr,
                 NULL);
    };

    ~DataBaseIt() { sqlite3_finalize(SQLPtr); }
    void First() { index=0; }
    void Next() { index++; }
    bool IsDone() const {return index >= sqlite3_column_count(SQLPtr);}
    string GetCurrent() const
    {
        const char* text = reinterpret_cast<const char*>(sqlite3_column_text(SQLPtr, index));
        if (text != nullptr) {
            return string(text);
        } else {
            return ""; // или другое значение по умолчанию
        }
    };
};

class DataBase
{
private:
    sqlite3* Database;
    sqlite3_stmt *ContPtr;

public:
    DataBase()
    {
        int OpenResult = sqlite3_open16( "C://Users//я//Desktop//с++//Задание на классы//BDArtif.db", &Database);
        if(OpenResult != SQLITE_OK)
        {
            wcout<<L"Ошибка открытия базы данных"<< endl;
        }
        wcout << L"Базада открыта"<<endl;

        const char* createTableSQL = "CREATE TABLE IF NOT EXISTS MagicArtifact ("
                                         "name TEXT,"
                                         "sizeA TEXT,"
                                         "WorkingCondition INTEGER,"
                                         "SuperStrength INTEGER"
                                         ");";

        char *errmsg;
        int execResult = sqlite3_exec(
                Database,
                createTableSQL,
                NULL,
                NULL,
                &errmsg);

        if (execResult != SQLITE_OK)
        {
                cout << errmsg << endl;
                wcout << L"Ошибка создания таблицы MagicArtifact" << endl;
        }
    };

    ~DataBase()
    {
        char *errmsg;
        int execResult = sqlite3_exec(
            Database,
            "DELETE FROM MagicArtifact",
            NULL,
            NULL,
            &errmsg);
        if (execResult != SQLITE_OK)
        {
            cout << errmsg << endl;
            wcout << L"Ошибка при очистке БД"<< endl;
        }
        sqlite3_close(Database);
        wcout << L"БД очищена и закрыта" << endl;
    };

    void AddArtif()
    {
        NameArtefact NAME = static_cast<NameArtefact>(rand()%4);
        string name;
        switch(NAME)
        {
            case NameArtefact::BookOfSpells :
                name = "BookOfSpells";
                break;
            case NameArtefact::MagicAmulet :
                name = "MagicAmulet";
                break;
            case NameArtefact::MagicSword :
                name = "MagicSword";
                break;
            case NameArtefact::PhoenixFeather :
                name = "PhoenixFeather";
                break;
            default :
                name = "Unknown";
        }

        SizeArtefact sizeA = static_cast<SizeArtefact>(rand()%3);
        string size;
        switch(sizeA)
        {
            case SizeArtefact::AveragArt :
                size = "AveragArt";
                break;
            case SizeArtefact::MegaArt :
                size = "MegaArt";
                break;
            case SizeArtefact::MiniArt :
                size = "MiniArt";
                break;
            default :
                size = "Unknown";
        }

        bool WorkingCondition = bool(rand()%2);
        string working = WorkingCondition ? "Артефакт в рабочем состоянии" : "Артефакт не работает";

        bool SuperStrength = bool(rand()%2);
        string super = SuperStrength ? "Супер сила есть" : "Супер силы нет";

        string ins =
            "INSERT INTO MagicArtifact (name, sizeA, WorkingCondition, SuperStrength) VALUES('"+name+"','" +size+"',"+(WorkingCondition ? "1" : "0")+",'"+(SuperStrength ? "1" : "0")+"');";

        char *errmsg;
        int execResult = sqlite3_exec(
            Database,
            ins.c_str(),
            NULL,
            NULL,
            &errmsg);

        if (execResult  != SQLITE_OK)
        {
            cout << errmsg << endl;
            wcout << L"Ошибка выполнения запроса" << endl;
        }
    }

    int GetCount()
    {
        int execResult2  = sqlite3_prepare_v2(
            Database,
            "SELECT count(*) FROM MagicArtifact",
            -1,
            &ContPtr,
            NULL);

        if(execResult2  != SQLITE_OK)
        {
            const char *errmsg2 = sqlite3_errmsg(Database);
            cout<<errmsg2<<endl;
            wcout << L"Ошибка подготовки запроса" << endl;
        }

        int execResult3  = sqlite3_step(ContPtr);
        if(execResult3  == SQLITE_ROW)
        {
            int cou = sqlite3_column_int(ContPtr, 0);
            wcout << L"В таблице артефактов: " << cou << L" магических артефактов" <<endl;
        }
    };
    DataBaseIt *GetIterator() { return new DataBaseIt(Database); }
};

int main()
{
    /*// Создаем объекты магических артефактов
    MagicSword sword;
    MagicAmulet amulet;
    PhoenixFeather feather;
    BookOfSpells book;

    // Вызываем методы для каждого артефакта
    cout << "MagicSword: " << endl;

    sword.Activate();
    cout << "The sword size: " << sizeToString(sword.GetSize()) << endl;
    cout << "The sword is " << (sword.IsSuperStr() ? "super strong" : "not super strong") << endl;
    sword.Deactivate();

    cout << endl;
    cout << "MagicAmulet: " << endl;

    amulet.Activate();
    cout << "The amulet size: " << sizeToString(amulet.GetSize()) << endl;
    cout << "The amulet is " << (amulet.IsSuperStr() ? "super strong" : "not super strong") << endl;
    amulet.Deactivate();

    cout << endl;
    cout << "PhoenixFeather: " << endl;

    feather.Activate();
    cout << "The feather size: " << sizeToString(feather.GetSize()) << endl;
    cout << "The feather is " << (feather.IsSuperStr() ? "super strong" : "not super strong") << endl;
    feather.Deactivate();

    cout << endl;
    cout << "BookOfSpells: " << endl;

    book.Activate();
    cout << "The book size: " << sizeToString(book.GetSize()) << std::endl;
    cout << "The book is " << (book.IsSuperStr() ? "super strong" : "not super strong") << endl;
    book.Deactivate();*/

    /*MegaArtifactCont artifactContainer;

    // Добавляем артефакты в контейнер
    artifactContainer.AddArtifact(new MagicSword());
    artifactContainer.AddArtifact(new MagicAmulet());
    artifactContainer.AddArtifact(new PhoenixFeather());
    artifactContainer.AddArtifact(new BookOfSpells());

    MegaArtifContIter megaIterator = artifactContainer.GetIterator();

    SizeFiltItDecorator sizeFilter(&megaIterator, SizeArtefact::MegaArt);
    NameFiltItDecorator nameFilter(&megaIterator, NameArtefact::MagicAmulet);
    InfoPrintItDecorator infoPrinter(&megaIterator);

    cout << "Artifacts filtered by size:" << endl;
    for (sizeFilter.First(); !sizeFilter.IsDone(); sizeFilter.Next())
    {
        MagicArtifactPtr artifact = sizeFilter.GetCurrent();
        if (artifact)
            cout << "Artifact: " << nameToString(artifact->GetName()) << ", Size: " << sizeToString(artifact->GetSize()) << endl;
    }
    cout << endl << endl;

    cout << "Artifacts filtered by name:" << endl;
    for (nameFilter.First(); !nameFilter.IsDone(); nameFilter.Next())
    {
        MagicArtifactPtr artifact = nameFilter.GetCurrent();
        if (artifact)
            cout << "Artifact: " << nameToString(artifact->GetName()) << ", Size: " << sizeToString(artifact->GetSize()) << endl;
    }
    cout << endl << endl;

    cout << "Information about current artifact:" << endl;
    for (infoPrinter.First(); !infoPrinter.IsDone(); infoPrinter.Next())
    {
        infoPrinter.GetCurrent();
    }*/

    /*MagicArtifactFactoryContainer artifactContainer;
    int numArtifacts = rand() % 100;

    for (int i = 0; i < numArtifacts; ++i)
    {
        artifactContainer.CreateAndAddArtifact();
    }
    cout << "Number of artifacts created: " << artifactContainer.GetArtifactCount() << endl;

    for (int i = 0; i < artifactContainer.GetArtifactCount(); ++i)
    {
        MagicArtifactPtr artifact = artifactContainer.GetArtifactByIndex(i);
        if (artifact)
        {
            cout << "Artifact " << i+1 << " ------- " << nameToString(artifact->GetName()) << " -------- " << sizeToString(artifact->GetSize()) << endl;
        }
    }*/
    setlocale(LC_ALL,"Russian");

    srand(time(0)); // Инициализация генератора случайных чисел
    //ArtifactFiltering();

    DataBase db;

    for(int i = 0 ; i < rand()%100; i++)
    {
        db.AddArtif();
    }

    db.GetCount();
    cout<< "Name \t\t" << "Size \t\t" << "W/C \t" << "S/S \t" << endl;

    DataBaseIt *iterator = db.GetIterator();
    while(true)
    {
        int StepResult = sqlite3_step(iterator->SQLPtr);
        if(StepResult != SQLITE_ROW) { break; }
        for(iterator->First(); !iterator->IsDone(); iterator->Next())
        {
            cout << iterator->GetCurrent() << "\t ";
        }
        cout<<endl;
	}

    return 0;
}
