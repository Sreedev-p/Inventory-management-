# LIBRARY MANAGEMENT SYSTEM

this project models a real university library where students and teachers can borrow Books,Movies and Journals<br>
students can only borrow upto 3 items while faculties can borrow upto 5<br>
Each item has only  one copy<br>
only one user can access the program at a time <br>
each user needs to be loged in via a userId and password<br>
An admin can add new items remove existing items add new user or remove an existing user<br>

## data structures used
for quick search of books and users I used a hashmap which loads permenently stored data on file on starting of the program<br>
after each modification the user file is modified for persistance <br>

## User database
contains a csv of ID,Name,password,UserType,active/inactive,borrow Count<br>
this is followed by count number of strings representing the ids of the borrowed media<br>
ID:unique userId its the unique key for the hashMap<br>
Name:firstName_lasstName <br>
password: password ->used for login verification<br>
userType:STUDENT,FACULTY,ADMIN -->an enum<br>
active/inactive: 1->active 0->inactive: admin sets it to 0 to remove a user<br>
count: number of boos borrowed<br>


## media database
csv of ID,name,authour and MediaType<br>
MediaType: enum of MOVIE,JOURNAL,BOOK<br>
media class contains an bool var available which tells if the media has aleady been borrowed or not<br>

## Hashmap initialisation
there are two hashMap one for user and other for media.<br>
on init it first loads user db to map<br>
then loads media db to map<br>
then checks on all members of user map if he has borrowed any book to mark thet book in media map as borrowed<br>
