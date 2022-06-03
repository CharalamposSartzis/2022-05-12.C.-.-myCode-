
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXSTRING 75            // Maximum string length. 
#define MINSTRING 2             // Minimum string length. 





typedef struct    // Student struct with the attributes of a student.
{
    char name[MAXSTRING + 1];      // (+1) is for the string's '\0' NULL-terminated character.
    int id;
}Student;

typedef struct nodeR* node;     // a type of pointer that holds the address of a node of the list (struct nodeR).
typedef struct listR* list;     // a type of pointer that holds the address of the list of students (struct listR).

struct nodeR          // Node struct that contains the student's data/details (struct Student) and pointers to next and previous nodes.
{
    Student data;        // the student's data/details (struct Student).
    node next;          // a pointer that holds the address of the next node of a list's node.
    node previous;      // a pointer that holds the address of the previous node of a list's node.
};

struct listR       // List struct that represents a doubly-linked list of students.
{
    node head;    // a pointer that holds the address of the first node of the list.
    node tail;    // a pointer that holds the address of the last node of the list.
};


int readInput(char* action);                // reads the user's input and returns the id given by the user for search/update/delete.
bool empty_stdin();                        // clears the rest of the user's input, after reading an integer.
int getNumStudents();                      // returns the number of students increased by one each time, in order to assign a unique id to each student.
bool checkLen(char* string);               // checks the length of the strings.

void printMenu();                            // prints the main menu with the choices.
void print(Student st);                      // prints a specific student.
void printstudents(list l);                  // prints all the students of the list.
list load(char* filename);                  // retrieve the students' list from file.
void save(char* filename, list l);          // save the students' list at file.
int addStudent(Student st, list l);         // add new student to the list.
Student* findStudent(int id, list l);       // search for the student at the list, based on id.
int deleteStudent(Student st, list l);      // delete the student from the list, based on st.id.
int updateStudent(Student st, list l);      // update the student's details at the list, based on st.id.

list createList();                        // creates the doubly-linked list.
bool listIsEmpty(list l);                 // checks if the list is empty.
node createStNode(/*Student st*/);        // create a new node with the student's data.
void insert(node stNode, list l);         // Insert a new student node at the end (tail) of the list.
