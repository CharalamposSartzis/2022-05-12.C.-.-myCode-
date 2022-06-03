
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXSTRING 75         //Maximum 
#define MINSTRING 2





typedef struct    //** Student
{
    char name[MAXSTRING + 1];      //**CHECK for input length. [+1]->'\0' end. name value: the address of the char array (of the 1st element).
    int id;
}Student;

typedef struct nodeR* node;     // a type of pointer that holds the address of a node of the list (struct nodeR).
typedef struct listR* list;     // a type of pointer that holds the address of the list of students (struct listR).

//** CHECK values at the creation (if NULL).
struct nodeR          //** TEST typedef.
{
    Student data;
    node next;          // a pointer that holds the address of the next node of a list's node.
    node previous;      // a pointer that holds the address of the previous node of a list's node.
};

//** CHECK values at the creation (if NULL).
struct listR
{
    node head;    // a pointer that holds the address of the first node of the list.
    node tail;    // a pointer that holds the address of the last node of the list.
};


int readInput(char* action);            //**
bool empty_stdin();
int getNumStudents();
bool checkLen(char* string);

void printMenu();
void print(Student st);
void printstudents(list l);
list load(char* filename);                  // retrieve the students' list from file.
void save(char* filename, list l);          // save the students' list at file.
int addStudent(Student st, list l);         // add new student to the list.
Student* findStudent(int id, list l);       //** [return type ??] search for the student at the list, based on id.
int deleteStudent(Student st, list l);      // delete the student from the list, based on st.id.
int updateStudent(Student st, list l);      // update the student's details at the list, based on st.id.

list createList();                        //**
bool listIsEmpty(list l);                 //**
node createStNode(/*Student st*/);        // create a new node with the student's data.
void insert(node stNode, list l);         // Insert a new student node at the end (tail) of the list.
