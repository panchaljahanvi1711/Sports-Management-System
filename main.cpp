#include <iostream>

#include <string>

#include <vector>

#include <memory>

#include <fstream>

#include <sstream>

#include <iomanip>

#include <limits>

#include <algorithm>

#include <ctime>



using namespace std;




class Person {

protected:

    int id;

    string name;

public:

    Person(int personId, const string& personName) : id(personId), name(personName) {}

    virtual ~Person() = default;

    int getId() const { return id; }

    const string& getName() const { return name; }

    virtual string role() const = 0;

    virtual void displayDetails() const {

        cout << role() << " ID: " << id << " | Name: " << name << '\n';

    }

};



class Coach : public Person {

    string specialty;

public:

    Coach(int coachId, const string& coachName, const string& coachSpecialty)

        : Person(coachId, coachName), specialty(coachSpecialty) {}

    string role() const override { return "Coach"; }

    void displayDetails() const override {

        cout << "Coach ID: " << id << " | Name: " << name

             << " | Specialty: " << specialty << '\n';

    }

};



class Player : public Person {

    int age;

public:

    Player(int playerId, const string& playerName, int playerAge)

        : Person(playerId, playerName), age(playerAge) {}

    string role() const override { return "Player"; }

    int getAge() const { return age; }

    void displayDetails() const override {

        cout << "Player ID: " << id << " | Name: " << name

             << " | Age: " << age << '\n';

    }

};



class Sport {

    int sportId;

    string sportName;

    string coachName;

    int playerCount;

    string venue;

public:

    Sport(int id, const string& name, const string& coach, int players, const string& place)

        : sportId(id), sportName(name), coachName(coach), playerCount(players), venue(place) {

        if (playerCount < 0) playerCount = 0;

    }

    ~Sport() = default; 

    int getId() const { return sportId; }

    const string& getName() const { return sportName; }

    const string& getCoach() const { return coachName; }

    int getPlayerCount() const { return playerCount; }

    const string& getVenue() const { return venue; }

    void setName(const string& value) { sportName = value; }

    void setCoach(const string& value) { coachName = value; }

    void setVenue(const string& value) { venue = value; }

    bool addPlayers(int n) {

        if (n <= 0) return false;

        playerCount += n;

        return true;

    }

    bool removePlayers(int n) {

        if (n <= 0 || n > playerCount) return false;

        playerCount -= n;

        return true;

    }

    void display() const {

        cout << "ID: " << sportId << " | Sport: " << sportName

             << " | Coach: " << coachName << " | Players: " << playerCount

             << " | Venue: " << venue << '\n';

    }

};



class Team {

    int teamId;

    string teamName;

    string sportName;

public:

    Team(int id, const string& name, const string& sport)

        : teamId(id), teamName(name), sportName(sport) {}

    ~Team() = default;

    void display() const {

        cout << "Team ID: " << teamId << " | Team: " << teamName

             << " | Sport: " << sportName << '\n';

    }

};



class RegistrationTransaction {

    int transactionId;

    int sportId;

    int playersAdded;

    string timestamp;

public:

    RegistrationTransaction(int txId, int sId, int count)

        : transactionId(txId), sportId(sId), playersAdded(count) {

        time_t now = time(nullptr);

        tm localTime{};

#ifdef _WIN32

        localtime_s(&localTime, &now);

#else

        localtime_r(&now, &localTime);

#endif

        ostringstream out;

        out << put_time(&localTime, "%Y-%m-%d %H:%M:%S");

        timestamp = out.str();

    }

    void display() const {

        cout << "Transaction #" << transactionId << " | Sport ID: " << sportId

             << " | Players registered: " << playersAdded

             << " | Time: " << timestamp << '\n';

    }

    void save(ofstream& file) const {

        file << transactionId << '|' << sportId << '|' << playersAdded << '|' << timestamp << '\n';

    }

};




static bool readQuoted(istream& in, string& value) {
    in >> ws;
    value.clear();
    if (in.peek() != '"') {
        return static_cast<bool>(in >> value);
    }
    in.get();
    char ch;
    while (in.get(ch)) {
        if (ch == '"') return true;
        if (ch == '\\' && in.get(ch)) {
            value += ch;
        } else {
            value += ch;
        }
    }
    return false;
}

static void writeQuoted(ostream& out, const string& value) {
    out << '"';
    for (char ch : value) {
        if (ch == '"' || ch == '\\') out << '\\';
        out << ch;
    }
    out << '"';
}

class SportsManager {

    

    vector<unique_ptr<Sport>> sports;

    vector<RegistrationTransaction> transactions;

    int nextTransactionId = 1;

    const string dataFile = "sports_data.txt";

    const string transactionFile = "transactions.txt";



    Sport* findSport(int id) {

        for (auto& s : sports) if (s->getId() == id) return s.get();

        return nullptr;

    }

    static string readLine(const string& prompt) {

        cout << prompt;

        string value;

        getline(cin, value);

        return value;

    }

    static int readInt(const string& prompt) {

        while (true) {

            cout << prompt;

            int value;

            if (cin >> value) {

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                return value;

            }

            cout << "Please enter a valid integer.\n";

            cin.clear();

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }

    }

public:

    SportsManager() = default;

    ~SportsManager() { saveData(); } 



    void seedIfEmpty() {

        if (!sports.empty()) return;

        sports.push_back(unique_ptr<Sport>(new Sport(101, "Cricket", "Rahul Sharma", 15, "Cricket Ground")));

        sports.push_back(unique_ptr<Sport>(new Sport(102, "Football", "Amit Patel", 11, "Football Ground")));

        sports.push_back(unique_ptr<Sport>(new Sport(103, "Basketball", "Raj Mehta", 10, "Basketball Court")));

    }



    void loadData() {

        ifstream file(dataFile);

        if (!file) { seedIfEmpty(); return; }

        string line;

        while (getline(file, line)) {

            istringstream in(line);

            int id, count;

            string name, coach, venue;

            if ((in >> id) && readQuoted(in, name) && readQuoted(in, coach)
                && (in >> count) && readQuoted(in, venue)) {

                if (!findSport(id)) sports.push_back(unique_ptr<Sport>(new Sport(id, name, coach, count, venue)));

            }

        }

        if (sports.empty()) seedIfEmpty();

    }



    void saveData() const {

        ofstream file(dataFile);

        if (!file) { cerr << "Warning: could not save " << dataFile << '\n'; return; }

        for (const auto& s : sports) {
            file << s->getId() << ' ';
            writeQuoted(file, s->getName());
            file << ' ';
            writeQuoted(file, s->getCoach());
            file << ' ' << s->getPlayerCount() << ' ';
            writeQuoted(file, s->getVenue());
            file << '\n';
        }
    }



    void displayAll() const {

        if (sports.empty()) { cout << "No sports found.\n"; return; }

        cout << "\n--- ALL SPORTS ---\n";

        for (const auto& s : sports) s->display();

    }



    void addSport() {

        int id = readInt("Enter unique sport ID: ");

        if (findSport(id)) { cout << "That ID already exists.\n"; return; }

        string name = readLine("Enter sport name: ");

        string coach = readLine("Enter coach name: ");

        int players = readInt("Enter current player count (0 or more): ");

        if (players < 0) { cout << "Player count cannot be negative.\n"; return; }

        string venue = readLine("Enter venue: ");

        sports.push_back(unique_ptr<Sport>(new Sport(id, name, coach, players, venue)));

        saveData();

        cout << "Sport added successfully.\n";

    }



    void searchSport() {

        int id = readInt("Enter sport ID to search: ");

        Sport* s = findSport(id);

        if (s) s->display(); else cout << "Sport not found.\n";

    }



    void updateSport() {

        int id = readInt("Enter sport ID to update: ");

        Sport* s = findSport(id);

        if (!s) { cout << "Sport not found.\n"; return; }

        cout << "1. Update name\n2. Update coach\n3. Update venue\n4. Update player count\n";

        int choice = readInt("Choose field: ");

        if (choice == 1) s->setName(readLine("New sport name: "));

        else if (choice == 2) s->setCoach(readLine("New coach name: "));

        else if (choice == 3) s->setVenue(readLine("New venue: "));

        else if (choice == 4) {

            int n = readInt("New player count (0 or more): ");

            if (n < 0) { cout << "Player count cannot be negative.\n"; return; }

            // Replace the record while retaining its ID and other details.

            s->removePlayers(s->getPlayerCount());

            s->addPlayers(n);

            if (n == 0) {

                

            }

        } else { cout << "Invalid field choice.\n"; return; }

        saveData();

        cout << "Sport updated successfully.\n";

    }



    void deleteSport() {

        int id = readInt("Enter sport ID to delete: ");

        auto it = find_if(sports.begin(), sports.end(), [id](const unique_ptr<Sport>& s) { return s->getId() == id; });

        if (it == sports.end()) { cout << "Sport not found.\n"; return; }

        sports.erase(it); 
        saveData();

        cout << "Sport deleted successfully.\n";

    }



    void addPlayersTransaction() {

        int id = readInt("Enter sport ID: ");

        Sport* s = findSport(id);

        if (!s) { cout << "Sport not found.\n"; return; }

        int count = readInt("Number of players to register: ");

        if (!s->addPlayers(count)) { cout << "Count must be greater than zero.\n"; return; }

        transactions.emplace_back(nextTransactionId++, id, count);

        ofstream file(transactionFile, ios::app);

        if (file) transactions.back().save(file);

        saveData();

        cout << "Registration transaction completed. Updated player count: " << s->getPlayerCount() << '\n';

    }



    void removePlayers() {

        int id = readInt("Enter sport ID: ");

        Sport* s = findSport(id);

        if (!s) { cout << "Sport not found.\n"; return; }

        int count = readInt("Number of players to remove: ");

        if (!s->removePlayers(count)) { cout << "Invalid count or not enough players.\n"; return; }

        saveData();

        cout << "Players removed. Remaining count: " << s->getPlayerCount() << '\n';

    }



    void changeVenue() {

        int id = readInt("Enter sport ID: ");

        Sport* s = findSport(id);

        if (!s) { cout << "Sport not found.\n"; return; }

        s->setVenue(readLine("Enter new venue: "));

        saveData();

        cout << "Venue updated.\n";

    }



    void showPolymorphismDemo() const {

        Coach coach(1, "Sample Coach", "General Sports");

        Player player(2, "Sample Player", 20);

      
        vector<unique_ptr<Person>> people;

        people.push_back(unique_ptr<Coach>(new Coach(coach)));

        people.push_back(unique_ptr<Player>(new Player(player)));

        cout << "\n--- POLYMORPHISM DEMO ---\n";

        for (const auto& person : people) person->displayDetails();

    }



    void showTransactions() const {

        ifstream file(transactionFile);

        if (!file) { cout << "No transaction history found yet.\n"; return; }

        cout << "\n--- SAVED TRANSACTIONS ---\n";

        string line;

        while (getline(file, line)) cout << line << '\n';

    }



    void run() {

        loadData();

        int choice;

        do {

            cout << "\n========== SPORTS MANAGEMENT SYSTEM ==========\n"

                 << "1. Display all sports\n2. Add sport\n3. Search sport by ID\n"

                 << "4. Update sport\n5. Delete sport\n6. Register/add players (transaction)\n"

                 << "7. Remove players\n8. Change venue\n9. Show transaction history\n"

                 << "10. Demonstrate inheritance/polymorphism\n11. Save data\n0. Exit\n";

            choice = readInt("Enter choice: ");

            switch (choice) {

                case 1: displayAll(); break;

                case 2: addSport(); break;

                case 3: searchSport(); break;

                case 4: updateSport(); break;

                case 5: deleteSport(); break;

                case 6: addPlayersTransaction(); break;

                case 7: removePlayers(); break;

                case 8: changeVenue(); break;

                case 9: showTransactions(); break;

                case 10: showPolymorphismDemo(); break;

                case 11: saveData(); cout << "Data saved.\n"; break;

                case 0: saveData(); cout << "Data saved. Goodbye!\n"; break;

                default: cout << "Invalid choice. Try again.\n";

            }

        } while (choice != 0);

    }

};



int main() {

    SportsManager manager;

    manager.run();

    return 0;

}
