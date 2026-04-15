#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<algorithm>
#define MAX_F 5
#define MAX_S 3
using namespace std;
enum mType{BOOK,JOURNAL,MOVIE};
enum uType{STUDENT,FACULTY,ADMIN};
string mtypeStr(mType t){
    if(t==BOOK){
        return "BOOK";
    }
    if(t==MOVIE){
        return "MOVIE";
    }
    return "JOURNAL";
}
string uTypeStr(uType t){
    if(t==STUDENT){
        return "STUDENT";
    }
    if(t==FACULTY){
        return "FACULTY";
    }
    return "ADMIN";
}
class item{
public:
    string id,name,author;
    mType kind;
    bool available;
    item(string i,string n,string a,string t){
        this->id=i;
        this->name=n;
        this->author=a;
        if(t=="BOOK"){
            kind=BOOK;
        }else if(t=="MOVIE"){
            kind=MOVIE;
        }else{
            kind=JOURNAL;
        }
        available=true;
    }
    item(){
        available=true;
    }
};
class user{
public:
    string name,pass,id;
    uType job;
    int count;
    vector<string> myList;
    bool valid;
    user(string i,string n,string p,uType j){
        this->id=i;
        this->name=n;
        this->pass=p;
        this->job=j;
        this->count=0;
        this->valid=true;
    }
    virtual ~user(){}
    virtual int borrow(item &obj)=0;
    bool giveBack(item &obj){
        auto it=find(myList.begin(),myList.end(),obj.id);
        if(it!=myList.end()){
            obj.available=true;
            myList.erase(it);
            count--;
            return true;
        }
        return false;
    }
    virtual void showAll()=0;
};
class student:public user{
public:
    student(string i,string n,string p):user(i,n,p,STUDENT){}
    int borrow(item &obj) override{
        if(!obj.available){
            return -1;
        }
        if(count>=MAX_S){
            return -2;
        }
        obj.available=false;
        myList.push_back(obj.id);
        count++;
        return 0;
    }
    void showAll() override{
        cout<<"\n[STUDENT] "<<name<<" | Borrowed: "<<count<<"/"<<MAX_S<<endl;
    }
};
class teacher:public user{
public:
    teacher(string i,string n,string p):user(i,n,p,FACULTY){}
    int borrow(item &obj) override{
        if(!obj.available){
            return -1;
        }
        if(count>=MAX_F){
            return -2;
        }
        obj.available=false;
        myList.push_back(obj.id);
        count++;
        return 0;
    }
    void showAll() override{
        cout<<"\n[FACULTY] "<<name<<" | Borrowed: "<<count<<"/"<<MAX_F<<endl;
    }
};
class admin:public user{
public:
    admin(string i,string n,string p):user(i,n,p,ADMIN){}
    int borrow(item &obj) override{
        return -3;
    }
    void showAll() override{
        cout<<"\n[ADMIN] "<<name<<endl;
    }
};
void saveItem(unordered_map<string,item> &allItems){
    ofstream f("media.txt",ios::trunc);
    for(auto const& [key,val]:allItems){
        f<<val.id<<","<<val.name<<","<<val.author<<","<<mtypeStr(val.kind)<<endl;
    }
    f.close();
}
void saveUsers(unordered_map<string,user*> &allUsers){
    ofstream f("user.txt",ios::trunc);
    for(auto const& [key,val]:allUsers){
        f<<val->id<<","<<val->name<<","<<val->pass<<","<<uTypeStr(val->job)<<","<<(val->valid?"1":"0")<<","<<val->count;
        for(const string& thing:val->myList){
            f<<","<<thing;
        }
        f<<endl;
    }
    f.close();
}
void loadItems(unordered_map<string,item> &allItems){
    ifstream f("media.txt");
    string s;
    while(getline(f,s)){
        if(s.empty()){
            continue;
        }
        stringstream ss(s);
        string i,n,a,t;
        getline(ss,i,',');getline(ss,n,',');getline(ss,a,',');getline(ss,t,',');
        allItems[i]=item(i,n,a,t);
    }
    f.close();
}
void loadUsers(unordered_map<string,user*> &allUsers){
    ifstream f("user.txt");
    string s;
    while(getline(f,s)){
        if(s.empty()){
            continue;
        }
        stringstream ss(s);
        string i,n,p,t,stat,c;
        getline(ss,i,',');getline(ss,n,',');getline(ss,p,',');
        getline(ss,t,',');getline(ss,stat,',');getline(ss,c,',');
        user* u;
        if(t=="STUDENT"){
            u=new student(i,n,p);
        }else if(t=="FACULTY"){
            u=new teacher(i,n,p);
        }else{
            u=new admin(i,n,p);
        }
        u->valid=(stat=="1");
        int num=stoi(c);
        for(int j=0;j<num;j++){
            string borrowed;
            getline(ss,borrowed,',');
            u->myList.push_back(borrowed);
            u->count++;
        }
        allUsers[i]=u;
    }
    f.close();
}
int main(){
    unordered_map<string,item> media;
    unordered_map<string,user*> users;
    loadItems(media);
    loadUsers(users);
    for(auto const& [uid,u]:users){
        for(string& bid:u->myList){
            if(media.count(bid)){
                media[bid].available=false;
            }
        }
    }
    while(true){
        string myId,myPass;
        cout<<"\n-----------------MENU------------------";
        cout<<"\nID: ";cin>>myId;
        cout<<"Password: ";cin>>myPass;
        if(users.count(myId)&&users[myId]->pass==myPass){
            user* u=users[myId];
            if(!u->valid){
                cout<<"Account Revoked!\n";
                continue;
            }
            bool go=true;
            while(go){
                u->showAll();
                if(u->job==ADMIN){
                    cout<<"1.Add  2.Remove  3.Revoke  4.List  5.Logout 6.Exit: ";
                    int choice;
                    cin>>choice;
                    if(choice==1){
                        string ni,nn,na,nt;
                        cout<<"ID: ";
                        cin>>ni;
                        cout<<"Title: ";
                        cin.ignore();
                        getline(cin,nn);
                        cout<<"Author: ";
                        getline(cin,na);
                        cout<<"Type: ";cin>>nt;
                        media[ni]=item(ni,nn,na,nt);
                        saveItem(media);
                    }else if(choice==2){
                        string ni;cout<<"ID to remove: ";cin>>ni;
                        media.erase(ni);
                        saveItem(media);
                    }else if(choice==3){
                        string ui;cout<<"User ID to revoke: ";cin>>ui;
                        if(users.count(ui)){
                            users[ui]->valid=false;
                            saveUsers(users);
                        }
                    }else if(choice==4){
                        for(auto const& [k,v]:media){
                            cout<<k<<" | "<<v.name<<endl;
                        }
                    }else if(choice==5){
                        go=false;
                    }
                    else if(choice==6){
                        return 0;
                    }
                }else{
                    cout<<"1.Borrow  2.Return  3.View  4.Logout: ";
                    int choice;cin>>choice;
                    if(choice==1){
                        vector<item*> temp;
                        for(auto& pair:media){
                            temp.push_back(&pair.second);
                        }
                        int total=temp.size();
                        int current=0;
                        bool browse=true;
                        while(browse){
                            cout<<"\n--- Page "<<(current/10)+1<<" ---\n";
                            for(int i=current;i<current+10&&i<total;i++){
                                cout<<"["<<temp[i]->id<<"] "<<temp[i]->name<<(temp[i]->available?" (Available)":" (Borrowed)")<<endl;
                            }
                            cout<<"\n(N) Next 10 | (B) Borrow by ID | (Q) Back: ";
                            char opt;
                            cin>>opt;
                            if((opt=='n'||opt=='N')&&current+10<total){
                                current+=10;
                            }else if(opt=='b'||opt=='B'){
                                string mi;
                                cout<<"Enter ID: ";
                                cin>>mi;
                                if(media.count(mi)){
                                    int res=u->borrow(media[mi]);
                                    if(res==0){
                                        saveUsers(users);
                                        saveItem(media);
                                        cout<<"Borrowed Succesffully!\n";
                                    }else if(res== -1){
                                        cout<<"Cannot borrow (media Unavailable)!\n";
                                    }
                                    else if(res == -2){
                                        cout<<"Cannot borrow (Limit exeeded)"<<endl;
                                    }
                                }
                                browse=false;
                            }else{
                                browse=false;
                            }
                        }
                    }else if(choice==2){
                        string mi;
                        cout<<"Media ID: ";
                        cin>>mi;
                        if(media.count(mi)){
                            if(u->giveBack(media[mi])){
                                saveUsers(users);
                                saveItem(media);
                                cout<<"Returned Successfully!\n";
                            }
                        }
                    }else if(choice==3){
                        for(string b:u->myList){
                            cout<<" - "<<b<<" ("<<media[b].name<<")\n";
                        }
                    }else if(choice==4){
                        go=false;
                    }
                }
            }
        }else{
            cout<<"Invalid Credentials!\n";
        }
    }
    return 0;
}