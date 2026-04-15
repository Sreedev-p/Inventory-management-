# LIBRARY MANAGEMENT SYSTEM

this project models a real university library where students and teachers can borrow Books,Movies and Journals
students can only borrow upto 3 items while faculties can borrow upto 5
Each item has only  one copy
only one user can access the program at a time 
each user needs to be loged in via a userId and password
An admin can add new items remove existing items add new user or remove an existing user

## data structures used
for quick search of books and users I used a hashmap which loads permenently stored data on file on starting of the program
after each modification the user file is modified for persistance 

## User database
contains a csv of ID,Name,password,UserType,active/inactive,borrow Count
this is followed by count number of strings representing the ids of the borrowed media
ID:unique userId its the unique key for the hashMap
Name:firstName_lasstName 
password: password ->used for login verification
userType:STUDENT,FACULTY,ADMIN -->an enum
active/inactive: 1->active 0->inactive: admin sets it to 0 to remove a user
count: number of boos borrowed


## media database
csv of ID,name,authour and MediaType
MediaType: enum of MOVIE,JOURNAL,BOOK
media class contains an bool var available which tells if the media has aleady been borrowed or not

## Hashmap initialisation
there are two hashMap one for user and other for media.
on init it first loads user db to map
then loads media db to map
then checks on all members of user map if he has borrowed any book to mark thet book in media map as borrowed
