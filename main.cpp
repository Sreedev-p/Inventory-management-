#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<unordered_map>
#define FACULTY_LIMIT 5
#define STUDENT_LIMIT 3
using namespace std;
int accNo=200;
enum mediaType{BOOK,JOURNAL,MOVIE};
enum userType{STUDENT,FACULTY,ADMIN};
class media{
    public:
    string ID,name,authour;
    mediaType type;
    bool available;
    media(string ID,string name,string authour,string type){
        this->ID=ID;
        this->name=name;
        this->authour=authour;
        if(type =="BOOK")
            this->type =BOOK;
        if(type =="MOVIE")
            this->type =MOVIE;
        if(type =="JOURNAL")
            this->type =JOURNAL;
        available=true;
    }
    media(string ID,string name,string authour){
        this->ID=ID;
        this->name=name;
        this->authour=authour;
        available=true;
    }
    media(){}
};
class Book:public media{
    public:
    Book(string ID,string name,string authour):media( ID, name, authour, BOOK){      
    }
};
class UserDetails{
    protected:
    string name,password,ID;
    userType type;
    int max;//no of books borrowed init 0
    vector<string>borrowed;
    public:
    void assignID(){
        ID =++accNo;
    }
    UserDetails(string ID,string name,string password,string type){
        this->ID=ID;
        this->name=name;
        this->password=password;
        if(type =="STUDENT")
            this->type=STUDENT;
        if(type =="FACULTY")
            this->type=FACULTY;
        if(type =="ADMIN")
            this->type=ADMIN;
        max=0;
    }
    UserDetails(string name,string pass){
        this->name=name;
        this->password=pass;
        max=0;
    }
    UserDetails(string name){
        this->name=name;
        max=0;
    }
};
class student:public UserDetails{
    public:
    student(string name,string password):UserDetails(name,password){
        type=STUDENT;
    }
    student(string name):UserDetails(name){
        type=STUDENT;
    }
    int borrowMedia(media &obj) {
        if(!obj.available){
            return -1;
        }
        else if(max == STUDENT_LIMIT){
            return -2;
        }
        else{
            obj.available=false;
            borrowed.push_back(obj.ID);
            max++;
            return 0;
        }
    }
    bool returnMedia(media &obj) {
        int i=0;
        for(string &s: borrowed){
            if(s==obj.ID){
                obj.available=true;
                borrowed.erase(borrowed.begin()+i);
                max--;
                return true;
            }
            i++;//search  in borrowed for the book
        }
        return false;
    }
    void createAccount() {
        cout<<"Enter Name: ";
        cin.ignore();
        getline(cin,name);
        assignID();//left
        cout<<"Congradulations You have succesfully created a new account"<<endl;
        cout<<"Your new ID is: "<<ID<<endl;
        cout<<"Set a password: ";
        cin.ignore();
        getline(cin,password);
        cout<<"Thank you, you can now login to our system"<<endl;
        type=STUDENT;
        max =0;
    }
    bool login() {
        string pass;
        cout<<"Enter your Password";
        cin.ignore();
        getline(cin,pass);
        if(pass == password)
            return true;
        return false;
    }
    void showMedia(){
        for(string s: borrowed){
            cout<<"->"<<s<<endl;
        }
    }
};
class faculty:public UserDetails{
    public:
    faculty(string name,string password):UserDetails(name,password){
        type=FACULTY;
    }
    faculty(string name):UserDetails(name){
        type=FACULTY;
    }
    void showMedia(){
        for(string s: borrowed){
            cout<<"->"<<s<<endl;
        }
    }
    int borrowMedia(media &obj) {
        if(!obj.available){
            return -1;
        }
        else if(max == FACULTY_LIMIT){
            return -2;
        }
        else{
            obj.available=false;
            borrowed.push_back(obj.ID);
            max++;
            return 0;
        }
    }
    bool returnMedia(media &obj) {
        int i=0;
        for(string &s: borrowed){
            if(s==obj.ID){
                obj.available=true;
                borrowed.erase(borrowed.begin()+i);
                max--;
                return true;
            }
            i++;
        }
        return false;
    }
    void createAccount() {
        cout<<"Enter Name: ";
        cin.ignore();
        getline(cin,name);
        assignID();//left
        cout<<"Congradulations You have succesfully created a new account"<<endl;
        cout<<"Your new ID is: "<<ID<<endl;
        cout<<"Set a password: ";
        cin.ignore();
        getline(cin,password);
        cout<<"Thank you, you can now login to our system"<<endl;
        type=FACULTY;
        max =0;
    }
    bool login() {
        string pass;
        cout<<"Enter your Password";
        cin.ignore();
        getline(cin,pass);
        if(pass == password)
            return true;
        return false;
    }
};
class admin:public UserDetails{
    public:
    admin(string name,string password):UserDetails(name,password){
        type=FACULTY;
    }
    admin(string name):UserDetails(name){
        type=FACULTY;
    }
    void addMedia(string ID,string name,string authour,mediaType T){

    }
};
int main(){
    unordered_map <string,media> media;
    unordered_map <string,UserDetails> users;
}
void init(){}
void loadUsers(unordered_map<string ,UserDetails> &map){
    ifstream file("user.txt");
    string line;
    while(getline(file,line)){
        if(line.empty())
            continue;
        string IDStr,nameStr,pass,type;
        stringstream stream(line);//convert the got line into a stream so that we can take it like a cin input
        getline(stream,IDStr,',');//reads all csv to each variable
        getline(stream,nameStr,',');
        getline(stream,pass,',');
        getline(stream,type,',');
        UserDetails temp(IDStr,nameStr,pass,type);//create a temp media object to feed the hashMap
        map[IDStr]=temp;
    }
    file.close();
}
void loadMedia(unordered_map<string ,media> &map){
    ifstream file("media.txt");
    string line;
    while(getline(file,line)){
        if(line.empty())
            continue;
        string IDStr,nameStr,authourStr,type;
        stringstream stream(line);//convert the got line into a stream so that we can take it like a cin input
        getline(stream,IDStr,',');//reads all csv to each variable
        getline(stream,nameStr,',');
        getline(stream,authourStr,',');
        getline(stream,type,',');
        media temp(IDStr,nameStr,authourStr,type);//create a temp media object to feed the hashMap
        map[IDStr]=temp;
    }
    file.close();
}