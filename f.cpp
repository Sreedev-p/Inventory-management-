#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<algorithm>

#define FACULTY_LIMIT 5
#define STUDENT_LIMIT 3

using namespace std;

enum mediaType { BOOK, JOURNAL, MOVIE };
enum userType { STUDENT, FACULTY, ADMIN };

// Helper to convert Enums to strings for file saving
string mTypeToStr(mediaType t) {
    if (t == BOOK) return "BOOK";
    if (t == MOVIE) return "MOVIE";
    return "JOURNAL";
}

string uTypeToStr(userType t) {
    if (t == STUDENT) return "STUDENT";
    if (t == FACULTY) return "FACULTY";
    return "ADMIN";
}

class media {
public:
    string ID, name, author;
    mediaType type;
    bool available;

    media(string ID, string name, string author, string typeStr) {
        this->ID = ID;
        this->name = name;
        this->author = author;
        if (typeStr == "BOOK") type = BOOK;
        else if (typeStr == "MOVIE") type = MOVIE;
        else type = JOURNAL;
        available = true;
    }
    media() { available = true; }
};

// Forward declarations for Sync functions
class UserDetails;
void syncUsers(unordered_map<string, UserDetails*> &map);
void syncMedia(unordered_map<string, media> &map);

class UserDetails {
public:
    string name, password, ID;
    userType type;
    int borrowNo;
    vector<string> borrowedIDs;
    bool isActive;

    UserDetails(string ID, string name, string password, userType type) {
        this->ID = ID;
        this->name = name;
        this->password = password;
        this->type = type;
        this->borrowNo = 0;
        this->isActive = true;
    }

    virtual ~UserDetails() {}

    virtual int borrowMedia(media &obj) = 0;

    bool returnMedia(media &obj) {
        auto it = find(borrowedIDs.begin(), borrowedIDs.end(), obj.ID);
        if (it != borrowedIDs.end()) {
            obj.available = true;
            borrowedIDs.erase(it);
            borrowNo--;
            return true;
        }
        return false;
    }

    virtual void showDashboard() = 0;
};

// --- USER SUBCLASSES ---

class student : public UserDetails {
public:
    student(string ID, string name, string pass) : UserDetails(ID, name, pass, STUDENT) {}
    int borrowMedia(media &obj) override {
        if (!obj.available) return -1;
        if (borrowNo >= STUDENT_LIMIT) return -2;
        obj.available = false;
        borrowedIDs.push_back(obj.ID);
        borrowNo++;
        return 0;
    }
    void showDashboard() override {
        cout << "\n[STUDENT] " << name << " | Borrowed: " << borrowNo << "/" << STUDENT_LIMIT << endl;
    }
};

class faculty : public UserDetails {
public:
    faculty(string ID, string name, string pass) : UserDetails(ID, name, pass, FACULTY) {}
    int borrowMedia(media &obj) override {
        if (!obj.available) return -1;
        if (borrowNo >= FACULTY_LIMIT) return -2;
        obj.available = false;
        borrowedIDs.push_back(obj.ID);
        borrowNo++;
        return 0;
    }
    void showDashboard() override {
        cout << "\n[FACULTY] " << name << " | Borrowed: " << borrowNo << "/" << FACULTY_LIMIT << endl;
    }
};

class admin : public UserDetails {
public:
    admin(string ID, string name, string pass) : UserDetails(ID, name, pass, ADMIN) {}
    int borrowMedia(media &obj) override { return -3; }
    void showDashboard() override { cout << "\n[ADMIN] " << name << endl; }
};

// --- FILE SYNC LOGIC ---

void syncMedia(unordered_map<string, media> &map) {
    ofstream file("media.txt", ios::trunc);
    for (auto const& [id, m] : map) {
        file << m.ID << "," << m.name << "," << m.author << "," << mTypeToStr(m.type) << endl;
    }
    file.close();
}

void syncUsers(unordered_map<string, UserDetails*> &map) {
    ofstream file("user.txt", ios::trunc);
    for (auto const& [id, u] : map) {
        // ID, Name, Pass, Type, Active, BorrowCount, BorrowedIDs...
        file << u->ID << "," << u->name << "," << u->password << "," << uTypeToStr(u->type) << "," 
             << (u->isActive ? "1" : "0") << "," << u->borrowNo;
        for (const string& bID : u->borrowedIDs) {
            file << "," << bID;
        }
        file << endl;
    }
    file.close();
}

// --- DATA LOADERS ---

void loadMedia(unordered_map<string, media> &map) {
    ifstream file("media.txt");
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, nm, au, ty;
        getline(ss, id, ','); getline(ss, nm, ','); getline(ss, au, ','); getline(ss, ty, ',');
        map[id] = media(id, nm, au, ty);
    }
    file.close();
}

void loadUsers(unordered_map<string, UserDetails*> &map) {
    ifstream file("user.txt");
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, nm, ps, ty, active, bCount;
        getline(ss, id, ','); getline(ss, nm, ','); getline(ss, ps, ','); 
        getline(ss, ty, ','); getline(ss, active, ','); getline(ss, bCount, ',');
        
        UserDetails* u;
        if (ty == "STUDENT") u = new student(id, nm, ps);
        else if (ty == "FACULTY") u = new faculty(id, nm, ps);
        else u = new admin(id, nm, ps);
        
        u->isActive = (active == "1");
        int count = stoi(bCount);
        for (int i = 0; i < count; i++) {
            string bID;
            getline(ss, bID, ',');
            u->borrowedIDs.push_back(bID);
            u->borrowNo++;
        }
        map[id] = u;
    }
    file.close();
}

// --- MAIN ---

int main() {
    unordered_map<string, media> inventory;
    unordered_map<string, UserDetails*> users;
    loadMedia(inventory);
    loadUsers(users);

    while (true) {
        string id, pass;
        cout << "\nID: "; cin >> id;
        cout << "Pass: "; cin >> pass;

        if (users.count(id) && users[id]->password == pass) {
            UserDetails* u = users[id];
            if (!u->isActive) { cout << "Account Revoked!\n"; continue; }

            bool session = true;
            while (session) {
                u->showDashboard();
                if (u->type == ADMIN) {
                    cout << "1. Add 2. Remove 3. Revoke 4. List 5. Logout: ";
                    int c; cin >> c;
                    if (c == 1) {
                        string mid, mn, ma, mt;
                        cout << "ID: "; cin >> mid; cout << "Title: "; cin.ignore(); getline(cin, mn);
                        cout << "Author: "; getline(cin, ma); cout << "Type: "; cin >> mt;
                        inventory[mid] = media(mid, mn, ma, mt);
                        syncMedia(inventory);
                    } 
                    else if (c == 2) {
                        string mid; cout << "ID to remove: "; cin >> mid;
                        inventory.erase(mid); syncMedia(inventory);
                    }
                    else if (c == 3) {
                        string uid; cout << "User ID to revoke: "; cin >> uid;
                        if (users.count(uid)) { users[uid]->isActive = false; syncUsers(users); }
                    }
                    else if (c == 5) session = false;
                } 
                else {
                    cout << "1. Borrow 2. Return 3. View 4. Logout: ";
                    int c; cin >> c;
                    if (c == 1) {
                        string mid; cout << "Media ID: "; cin >> mid;
                        if (inventory.count(mid)) {
                            if (u->borrowMedia(inventory[mid]) == 0) { 
                                syncUsers(users); syncMedia(inventory);
                                cout << "Borrowed!\n";
                            }
                        }
                    }
                    else if (c == 2) {
                        string mid; cout << "Media ID: "; cin >> mid;
                        if (inventory.count(mid)) {
                            if (u->returnMedia(inventory[mid])) {
                                syncUsers(users); syncMedia(inventory);
                                cout << "Returned!\n";
                            }
                        }
                    }
                    else if (c == 3) {
                        for (string b : u->borrowedIDs) cout << " - " << b << " (" << inventory[b].name << ")\n";
                    }
                    else if (c == 4) session = false;
                }
            }
        } else { cout << "Invalid Login!\n"; }
    }
    return 0;
}