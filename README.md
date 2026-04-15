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
