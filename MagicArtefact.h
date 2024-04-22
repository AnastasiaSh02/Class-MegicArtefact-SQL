#ifndef MagicArtefactH
#define MagicArtefactH

enum class NameArtefact: int
{
    MagicSword ,
    MagicAmulet,
    PhoenixFeather,
    BookOfSpells,
    Unknown = -1
};

enum class SizeArtefact: int
{
    MegaArt = 0,
    AveragArt = 1,
    MiniArt = 2,
    Unknown = -1
};

// Базовый класс для всех магических артефактов
class MagicArtifact
{
protected:
    NameArtefact name;
    SizeArtefact sizeA;
    bool WorkingCondition; // в рабочем состоянии или сломан?
    bool SuperStrength; //супер сила есть или нет?

public:
    MagicArtifact()
    {
        name = NameArtefact::Unknown;
        sizeA = SizeArtefact::Unknown;
        WorkingCondition = false;
        SuperStrength = false;
    }
    virtual ~MagicArtifact() {}
    virtual void Activate() const = 0; //активация артефакта
    virtual void Deactivate() const = 0;//деактивации артефакта
    virtual NameArtefact GetName() const { return name; }
    virtual SizeArtefact GetSize() const { return sizeA; }
    bool IsWork() { return WorkingCondition; }
    bool IsSuperStr() { return SuperStrength; }
};

class MagicSword : public MagicArtifact
{
public:
    MagicSword() : MagicArtifact() { sizeA = SizeArtefact::MegaArt; }
    NameArtefact GetName() const { return NameArtefact::MagicSword; }
    bool IsSuperStr() const { return true; }
    void Activate() const { cout<< "The sword is activated"<< endl; }
    void Deactivate() const { cout<< "The sword is deactivated"<< endl; }
};

class MagicAmulet : public MagicArtifact
{
public:
    MagicAmulet() : MagicArtifact() { sizeA = SizeArtefact::MiniArt; }
    NameArtefact GetName() const { return NameArtefact::MagicAmulet; }
    bool IsSuperStr() const { return true; }
    void Activate() const { wcout<< L"The amulet is activated"<< endl; }
    void Deactivate() const { wcout<< L"The amulet is deactivated"<< endl; }
};

class PhoenixFeather : public MagicArtifact
{
public:
    PhoenixFeather() : MagicArtifact() { sizeA = SizeArtefact::AveragArt; }
    NameArtefact GetName() const { return NameArtefact::PhoenixFeather; }
    bool IsSuperStr() const { return false; }
    void Activate() const { wcout<< L"The feather is activated"<< endl; }
    void Deactivate() const { wcout<< L"The feather is deactivated"<< endl; }
};

class BookOfSpells : public MagicArtifact
{
public:
    BookOfSpells() : MagicArtifact() { sizeA = SizeArtefact::MegaArt; }
    NameArtefact GetName() const { return NameArtefact::BookOfSpells; }
    bool IsSuperStr() const { return false; }
    void Activate() const { wcout<< L"The book is activated"<< endl; }
    void Deactivate() const { wcout<< L"The book is deactivated"<< endl; }
};

string sizeToString(SizeArtefact size)
{
    switch(size)
    {
        case SizeArtefact::MegaArt:
            return "Mega";
        case SizeArtefact::AveragArt:
            return "Average";
        case SizeArtefact::MiniArt:
            return "Mini";
        default:
            return "Unknown";
    }
}

string nameToString(NameArtefact name)
{
    switch(name)
    {
        case NameArtefact::BookOfSpells:
            return "Book Of Spells";
        case NameArtefact::MagicAmulet:
            return "Magic Amulet";
        case NameArtefact::MagicSword:
            return "Magic Sword";
        case NameArtefact::PhoenixFeather:
            return "Phoenix Feather";
        default:
            return "Unknown";
    }
}

// Задание 4.
template <typename T>

class Iterator
{
public:
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual T GetCurrent() const = 0;
    virtual ~Iterator() {}
};


typedef MagicArtifact * MagicArtifactPtr;

class ArtifContIter : public Iterator<MagicArtifactPtr>
{
private:
    MagicArtifactPtr *Artifacts; //указатель на массив указателей на артефакты
    int Pos; //текущая позиция итератора
    int Count; //общее количество артефактов

public:
    ArtifContIter(MagicArtifactPtr *artifacts, int count)
    {
        Artifacts = artifacts;
        Count = count;
        Pos = 0;
    }

    void First() { Pos = 0; }//установка итератора на 1 элемент
    void Next() { Pos++; } //переход к следующему элементу
    bool IsDone() const { return Pos >= Count; }//проверкa завершения итерации
    MagicArtifactPtr GetCurrent() const //получение текущего элемента
    {
        if (Pos < Count)
            return Artifacts[Pos];
        else //текущая позиция выходит за границы контейнерa
            return nullptr;
    }
    int GetCurrentPosition() const { return Pos; } //получение текущей позиции
    int RemainingCount() const { return Count - Pos; } //получение кол-ва оставшихся элементов
};

class ArtifactCont
{
private:
    MagicArtifactPtr *Artifacts;
    int ArtifactCount;
    int MaxSize;

public:
    ArtifactCont(int maxSize);
    ~ArtifactCont();
    void AddArtifact(MagicArtifactPtr newArt);
    int GetCount() const { return ArtifactCount; }
    MagicArtifactPtr GetByIndex(int index) const;
    ArtifContIter GetIterator() { return ArtifContIter(Artifacts, ArtifactCount); }
};

class MegaArtifContIter : public Iterator<MagicArtifactPtr>
{
private:
    const vector<MagicArtifactPtr> *Artifacts;
    typename vector<MagicArtifactPtr>::const_iterator it; //итератор для перебора вектора артефактов

public:
    MegaArtifContIter(const vector<MagicArtifactPtr> *artifacts) //конструктор
    {
        Artifacts = artifacts;
        it = Artifacts->begin(); //итератор на начало вектора
    }
    void First() { it = Artifacts->begin(); } // установка итератора на 1 элемент
    void Next() { it++; } //переход к следующему элементу
    bool IsDone() const { return it == Artifacts->end(); } //проверка завершения итерации
    MagicArtifactPtr GetCurrent() const { return *it; }//получение текущего элемента
};

class MegaArtifactCont
{
private:
    vector<MagicArtifactPtr> Artifacts;

public:
    ~MegaArtifactCont();
    void AddArtifact(MagicArtifactPtr newArt) { Artifacts.push_back(newArt); }
    int GetCount() const { return Artifacts.size(); }
    MagicArtifactPtr GetByIndex(int index) const;
    MegaArtifContIter GetIterator() const { return MegaArtifContIter(&Artifacts); }
};


//
// Задание 6. Декортаоры для итератора
//

// ДЕКОРАТОР для фильтрации артефактов по РАЗМЕРУ
class SizeFiltItDecorator : public Iterator<MagicArtifactPtr>
{
private:
    Iterator<MagicArtifactPtr> *it;
    SizeArtefact targetSize; //размер, по которому будет происходить фильтрация

public:
    SizeFiltItDecorator(Iterator<MagicArtifactPtr> *iterator, SizeArtefact size)
    {
        it = iterator;
        targetSize = size;
    }

    void First()
    {
        it->First(); //итератор на начало
        while (!it->IsDone() && it->GetCurrent()->GetSize() != targetSize) //пока артефакт не будет заданного размера
            it->Next();
    }
    void Next()
    {
        it->Next(); //переход к следующему артефакту
        while (!it->IsDone() && it->GetCurrent()->GetSize() != targetSize)
            it->Next();
    }
    bool IsDone() const { return it->IsDone(); } //проверка завершения итерации
    MagicArtifactPtr GetCurrent() const { return it->GetCurrent(); }  //получение текущего элемента артефакта
};


//ДЕКОРАТООР для фильтрации артефактов по ИМЕНИ
class NameFiltItDecorator : public Iterator<MagicArtifactPtr>
{
private:
    Iterator<MagicArtifactPtr> *it;
    NameArtefact targetName;

public:
    NameFiltItDecorator(Iterator<MagicArtifactPtr> *iterator, NameArtefact name)
    {
        it = iterator;
        targetName = name;
    }

    void First()
    {
        it->First();
        while (!it->IsDone() && it->GetCurrent()->GetName() != targetName)
            it->Next();
    }
    void Next()
    {
        it->Next();
        while (!it->IsDone() && it->GetCurrent()->GetName() != targetName)
            it->Next();
    }
    bool IsDone() const { return it->IsDone(); } //override (если случатся неполадки с виртуальностью
    MagicArtifactPtr GetCurrent() const { return it->GetCurrent(); }
};

//ДЕКОРАТОР для вывода информации о ТЕКУЩЕМ ЭЛЕМЕНТЕ
class InfoPrintItDecorator : public Iterator<MagicArtifactPtr>
{
private:
    Iterator<MagicArtifactPtr> *it;

public:
    InfoPrintItDecorator(Iterator<MagicArtifactPtr> *iterator)
    {
        it = iterator;
    }

    void First() { it->First(); }
    void Next() { it->Next(); }
    bool IsDone() const { return it->IsDone(); }
    MagicArtifactPtr GetCurrent() const
    {
        MagicArtifactPtr current = it->GetCurrent();
        if (current)
        {
            cout << "Current Artifact: " << nameToString(current->GetName()) << endl;
            cout << "Size: " << sizeToString(current->GetSize()) << endl;
        }
        return current;
    }
};


//Задание 7. фабричный метод и наполнение контейнера случайным количеством объектов.
class MagicArtifactFactoryContainer
{
private:
    vector<MagicArtifactPtr> artifacts;

public:
    MagicArtifactFactoryContainer() { srand(time(0)); }

    void CreateAndAddArtifact()
    {
        // Случайно выбираем тип артефакта
        int artifactType = rand() % 4; // 0 - Sword, 1 - Amulet, 2 - Feather, 3 - Book
        MagicArtifactPtr newArtifact = nullptr;

        switch (artifactType)
        {
            case 0:
                newArtifact = new MagicSword(); break;
            case 1:
                newArtifact = new MagicAmulet(); break;
            case 2:
                newArtifact = new PhoenixFeather(); break;
            case 3:
                newArtifact = new BookOfSpells(); break;
        }
        if (newArtifact)
            artifacts.push_back(newArtifact);
    }

    int GetArtifactCount() const { return artifacts.size(); }

    MagicArtifactPtr GetArtifactByIndex(int index) const
    {
        if (index >= 0 && index < artifacts.size())
            return artifacts[index];
    }
};


#endif // MagicArtefactH








/*

//ДЕКОРАТОР для ПОДСЧЕТА использования каждого артефакта
// + нужно подключить  map для отслеживания статистики использования артефактов
class ArtifUsageCounterDecorator : public Iterator<MagicArtifactPtr>
{
private:
    Iterator<MagicArtifactPtr> *iterator;
    map<NameArtefact, int> usageCount; //карта для хранения кол-ва использований артефактов

public:
    ArtifUsageCounterDecorator(Iterator<MagicArtifactPtr> *Iterator)
    {
        iterator = Iterator;
    }

    void First() { iterator->First(); }

    void Next()
    {
        iterator->Next();
        MagicArtifactPtr currentArtifact = iterator->GetCurrent();
        if (currentArtifact)
        {
            usageCount[currentArtifact->GetName()]++; //увеличивает счетчик использования текущего артефакта
        }
    }
    bool IsDone() const { return iterator->IsDone(); }
    MagicArtifactPtr GetCurrent() const { return iterator->GetCurrent(); }
    int GetUsageCount(const MagicArtifactPtr& artifact) const
    {
        auto it = usageCount.find(artifact->GetName());
        if (it != usageCount.end())
        {
            return it->second;
        }
        return 0;
    }
};*/
