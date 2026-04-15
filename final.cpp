#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<algorithm>

#define FACULTY_LIMIT 5
#define STUDENT_LIMIT 3

using namespace std;

int accNo = 200;

enum mediaType { BOOK, JOURNAL, MOVIE };
enum userType { STUDENT, FACULTY, ADMIN };

// --- MEDIA CLASS ---
class media {
public:
    string ID, name, author;
    mediaType type;
    bool available;

    media(string ID, string name, string author, string typeStr) {
        this->ID = ID; this->name = name; this->author = author;
        if (typeStr == "BOOK") type = BOOK;
        else if (typeStr == "MOVIE") type = MOVIE;
        else type = JOURNAL;
        available = true;
    }
    media() : available(true) {}
};

// --- BASE USER CLASS ---
class UserDetails {
public:
    string name, password, ID;
    userType type;
    int currentBorrowed; // Tracks current count
    vector<string> borrowedIDs;

    UserDetails(string ID, string name, string password, userType type) 
        : ID(ID), name(name), password(password), type(type), currentBorrowed(0) {}

    virtual ~UserDetails() {} // Essential for polymorphic pointers

    virtual int borrowMedia(media &obj) = 0;
    
    bool returnMedia(media &obj) {
        auto it = find(borrowedIDs.begin(), borrowedIDs.end(), obj.ID);
        if (it != borrowedIDs.end()) {
            obj.available = true;
            borrowedIDs.erase(it);
            currentBorrowed--;
            return true;
        }
        return false;
    }

    virtual void showDashboard() = 0;
};

// --- DERIVED USER CLASSES ---
class student : public UserDetails {
public:
    student(string ID, string name, string pass) : UserDetails(ID, name, pass, STUDENT) {}
    
    int borrowMedia(media &obj) override {
        if (!obj.available) return -1;
        if (currentBorrowed >= STUDENT_LIMIT) return -2;
        obj.available = false;
        borrowedIDs.push_back(obj.ID);
        currentBorrowed++;
        return 0;
    }

    void showDashboard() override {
        cout << "\n--- Student Dashboard [" << name << "] ---\n";
        cout << "Items Borrowed: " << currentBorrowed << "/" << STUDENT_LIMIT << endl;
    }
};

class faculty : public UserDetails {
public:
    faculty(string ID, string name, string pass) : UserDetails(ID, name, pass, FACULTY) {}
    
    int borrowMedia(media &obj) override {
        if (!obj.available) return -1;
        if (currentBorrowed >= FACULTY_LIMIT) return -2;
        obj.available = false;
        borrowedIDs.push_back(obj.ID);
        currentBorrowed++;
        return 0;
    }

    void showDashboard() override {
        cout << "\n--- Faculty Dashboard [" << name << "] ---\n";
        cout << "Items Borrowed: " << currentBorrowed << "/" << FACULTY_LIMIT << endl;
    }
};

class admin : public UserDetails {
public:
    admin(string ID, string name, string pass) : UserDetails(ID, name, pass, ADMIN) {}
    int borrowMedia(media &obj) override { return -3; } // Admin's don't borrow
    void showDashboard() override {
        cout << "\n--- Admin System Control [" << name << "] ---\n";
    }
};

// --- SYSTEM ENGINE FUNCTIONS ---

void loadMedia(unordered_map<string, media> &mMap) {
    ifstream file("media.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, nm, au, ty;
        getline(ss, id, ','); getline(ss, nm, ','); getline(ss, au, ','); getline(ss, ty, ',');
        mMap[id] = media(id, nm, au, ty);
    }
}

void loadUsers(unordered_map<string, UserDetails*> &uMap) {
    ifstream file("user.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, nm, ps, ty;
        getline(ss, id, ','); getline(ss, nm, ','); getline(ss, ps, ','); getline(ss, ty, ',');
        if (ty == "STUDENT") uMap[id] = new student(id, nm, ps);
        else if (ty == "FACULTY") uMap[id] = new faculty(id, nm, ps);
        else uMap[id] = new admin(id, nm, ps);
    }
}

// --- MAIN EXECUTION ---
int main() {
    unordered_map<string, media> inventory;
    unordered_map<string, UserDetails*> users;

    loadMedia(inventory);
    loadUsers(users);

    string loginID, loginPass;
    cout << "===== WELCOME TO THE UNIVERSITY LIBRARY =====\n";
    cout << "Enter User ID: "; cin >> loginID;
    cout << "Enter Password: "; cin >> loginPass;

    if (users.find(loginID) != users.end() && users[loginID]->password == loginPass) {
        UserDetails* currentUser = users[loginID];
        bool running = true;

        while (running) {
            currentUser->showDashboard();
            cout << "1. Search Media\n2. Borrow Item\n3. Return Item\n4. Exit\nChoice: ";
            int choice; cin >> choice;

            if (choice == 1) {
                for (auto const& [id, item] : inventory) 
                    cout << id << " | " << item.name << " [" << (item.available ? "Available" : "Out") << "]\n";
            } 
            else if (choice == 2 && currentUser->type != ADMIN) {
                cout << "Enter Media ID to borrow: "; string mid; cin >> mid;
                if (inventory.count(mid)) {
                    int res = currentUser->borrowMedia(inventory[mid]);
                    if (res == 0) cout << "Successfully borrowed!\n";
                    else if (res == -1) cout << "Item already out!\n";
                    else cout << "Limit reached!\n";
                }
            }
            else if (choice == 4) running = false;
        }
    } else {
        cout << "Invalid Credentials.\n";
    }

    // Cleanup memory
    for (auto& pair : users) delete pair.second;
    return 0;
}