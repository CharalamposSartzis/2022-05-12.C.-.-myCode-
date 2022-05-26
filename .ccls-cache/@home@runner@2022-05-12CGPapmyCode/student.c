
#include "student.h"    // stdio (for NULL) is included in student.h

int numStudents = 0;    //** CHECK <-- pragma once [DECLARE at student.h].



void printMenu()
{
    //** CHECK/FIX 3 and 5 -> together
    //** (BEAUTIFY).
    printf("\n\n-------------------------------------------------------\n"
            "\n\tMenu choices:\n"
            "\t1. Create new student.\n"
            "\t2. Find a student.\n"
            "\t3. Print all students' details.\n"
            "\t4. Update a student's details.\n"
            "\t5. Delete a student.\n"
            "\t0. Exit!\n");
}

int readInput(char* action)
{
    // action: search for, update, delete.
    
    int searchID = 0;
    
    int scanReturn;
    bool clearInput;
    
    bool scan_EOF;
    bool validInput;
    bool break_loop_input;
    
    do
    {
        printf("\nPlease give the [id] of the student you want to %s: ", action);    //**
        scanReturn = scanf("%d", &searchID);      //**
        clearInput = empty_stdin();            //**

        scan_EOF = (scanReturn == EOF);
        validInput = (scanReturn == 1) && (clearInput);        //**
        break_loop_input = (scan_EOF || validInput);           //** BREAK loop.

        if(scan_EOF)                // If the user cancels the input.
        {
            printf("\nUser canceled student search...");
            // break;
        }
        else if(!validInput /*&& !scan_EOF*/)    //** CHECK-CHANGE. If the user doesn't cancel the input AND he enters invalid id.
        {
            printf("\nInvalid id input! You must enter only 1 integer and NO characters! \nPlease try again.\n");    //**
        }
                          
    }while(!break_loop_input);    //** CHECK-CHANGE.

    return searchID;            // The function returns a value when the do-while loop ends. This means that the user entered
}

//** CHECK bool in LECTURES.
bool empty_stdin(/*int* choice*/)    // Function to empty stdin and check for the rest of the user input.
{
    int cnt = 0;
    int inputChar;

    bool notSPACE; 
    bool notTAB; 
    bool notENTER;
    bool notEOF; 
    
    do
    {
        inputChar = getchar();      // included ENTER (*) and SPACE.
        
        notSPACE = (inputChar!=' ');        //**
        notTAB = (inputChar!='\t');
        notENTER = (inputChar!='\n');
        notEOF = (inputChar!=EOF);
        
        printf("-- BEFORE count. --\n");
        //printf("| char: %c |\n", inputChar);
        
        if(notSPACE && notTAB && notENTER && notEOF)   // (*) exclude SPACE and ENTER here [OK].
        {
            ++cnt;
            printf("| char: %c |\n", inputChar);
        }
        
    }while(notENTER && notEOF);        //**
    
    printf("\ncount: %d", cnt);
    return (cnt==0);        //**
}

void print(Student st)
{
    // printf("\n id: [%d] | name: %s .\n", st.id, st.name);
    printf("%-5d | %-13s\n", st.id, st.name);    //** CHECK format.
}

void printstudents(list l)
{
    node traversal_p = l->head;          // Pointer to traverse through the list.
    printf("--All students' details: \n");
    // printf("Student with id: %d | name: %s \n", traversal_p->data.id, l->head->data.name);    //** OK

    printf("%-5s | %-13s\n", "ID", "NAME");    //** CHECK format.
    
    while(traversal_p != NULL)    //**
    {
        // Student st = traversal_p->data;
        print(traversal_p->data);            // Print the details of the current student each time.
        // printf("\n [[ NAME: %s ]]", traversal_p->data.name);
        traversal_p = traversal_p->next;
    }
    printf("\n\n");
}



list load(char* filename)  // retrieve the students' list from file.
{
    FILE* fd = fopen(filename,"r");        // Open file for reading.
    
    if(fd == NULL)
    {
        fprintf(stderr, "File %s could not be opened in load.\n", filename);
        // perror("fopen");
        return NULL;
    }
     
    list newList = createList();
    Student* newStudent;
    node newStNode;
    
    int stID;
    char stName[MAXSTRING + 1];

    fscanf(fd, "%d%s", &stID, stName);    //** Read from fd -> data of type/format int|char* -> save them at local variables ID and name.
    
    while(!feof(fd))    // OR 1 == fscanf(fd, "%d", &age)).
    { 
        //** Create new student node with the data saved at local variables ID and name. Then insert it to the list.
        
        newStudent = createStudent();  
        newStudent->id = stID;
        strcpy(newStudent->name, stName);
        
        newStNode = createStNode(newStudent);
        insert(newStNode, newList);

        //** Read from fd -> data of type/format int|char* -> save them at local variables ID and name.
        fscanf(fd, "%d%s", &stID, stName);    
    }
    
    fclose(fd);
    return newList;    //** Return the list filled with the file data.
}

//void save(char* filename, list l);  // save the students' list at file.

//** WILL BE replaced by LOAD.
list createList()
{
    list newList = (list)malloc(sizeof(struct listR));

    newList->head = NULL;
    newList->tail = NULL;

    return newList;
}

//**[CHECK lectures] Do it with int (true=1+ OR (<= -1), false=0).
bool listIsEmpty(list l)
{
    //**
    return ((l->head == NULL) && (l->tail == NULL));
}

node createStNode(Student* st_p) //**    // create a new node with the student's data.
{
    node stNode = (node)malloc(sizeof(struct nodeR));    // Allocate heap memory with the size of a node with the    
                                                         // student's data (struct nodeR).
    //** COPIES the content of st_p -> to the data of the stNode.
    stNode->data = *st_p;    //** CHECK.
    // stNode->data.id = st_p->id;    //** CHECK.
    // strcpy(stNode->data.name, st_p->name);    //** CHECK.
    stNode->next = NULL;
    stNode->previous = NULL;

    return stNode;    //** <--
}

void insert(node stNode, list l)        // Insert a new student node at the end (tail) of the list.
{
    if(listIsEmpty(l))      //**(CHECK)  If list is empty: set the new node as the head and the tail of the list.
    {
        l->head = stNode;
        l->tail = stNode;

        return;
    }
    
    // Insertion at the end (tail) of the list.
    l->tail->next = stNode;
    stNode->previous = l->tail;
    l->tail = stNode;
}

// Sets the data of the student at a node.
Student* createStudent()            //** ADD parameters.
{
    // // Create the new student node with the data of the new student.
    // node stNode = createStNode();      //** MUST BE called AFTER its definition!!
        
    // return &(stNode->data);

    Student* newSt = (Student*)malloc(sizeof(Student));

    //newSt->id=0;
    //strcpy(newSt->name, "");    //**

    newSt->id = 0;        //**
    strcpy(newSt->name, "DEFAULT_NAME");    //** CHECK for Name with len < DEFAULT_NAME len [AND for length=0];

    //** FREE name after the use of the function.
    
    return newSt;
}



int addStudent(Student st, list l)      //** CHECK return type [LECTURES]. Add new student to the list (insert at the end/tail of the list).
{
    //** st: pass by value -> creates copy of the passed argument 
    // [** ?? lives in stack UNTIL the END of FUNCTION].
    
    // Set the student's data: name and id.  //

    //**
    // printf("\nEnter the student's name: ");
    // // //** ++ CHECK the length of the input (++).
    // scanf("%s", /*stNode->data*/st.name);      // Save the user's input as the name of the new student.

    ++numStudents;  // Every call of the function adds a new student and increases the number of students at the list.
    /*stNode->data*/st.id = numStudents;         // 1st student has id=1, 2nd student has id=2 and so on.

    // st = createStudent();
    
    // Create the new student node with the data of the new student.
    node stNode = createStNode(&st);      //** [??] MUST BE called AFTER its definition!! 

    //Insert the new student node (stNode) at the end of the list.
    insert(stNode, l);

    //** FREE the student argument, AFTER the function call.
    
    return st.id;    //** [++] [??] return the id of the student who has just been added to the list.
}

Student* findStudent(int id, list l)  //** search for the student at the list, based on id.
{
    // // If student does not exist.
    // if(strlen(st.name)==0)    //** CHECK for the length at user input.
    // {
    //     printf("Student does not exist...");
    //     return 0;
    // }

    // Student* st_p = NULL;    //** CHECK.

    //**
    if(listIsEmpty(l))
    {
        printf("\nThe list is empty!\n");
        return NULL;    //** NULL;
    }
    
    // check if is at head
    if(l->head->data.id == id)
    {
        // st_p = &(l->head->data);    //** BEWARE !!
        return &(l->head->data);        // O(1).
    }

    // check if is at tail.
    if(l->tail->data.id == id)
    {
        // st_p = &(l->tail->data);    //** BEWARE !!
        return &(l->tail->data);        // O(1).
    }
    
    node traversal_p = l->head;          // Pointer to traverse through the list.
    while(traversal_p != NULL)    //**CHECK-CHANGE (...&& traversal_p->data.id != id)
    {
        if(traversal_p->data.id == id)
        {
            return &(traversal_p->data);    // return the address of the student.    //** CHECK.
            // st_p = &traversal_p->data;
            // break; 
            // stNode = traversal_p;
        }
        traversal_p = traversal_p->next;    // O(n) [ O(n/2) ].
    }

    return NULL;    //** [??]    // If student not found.
    // return st_p;
}

int deleteStudent(Student st, list l)  // delete the student from the list, based on st.id.
{
    //** findStudent(st.id, l); --> CHECK if list is EMPTY.
    
    //** CALL: 1st call the findStudent -> place the result as the argument st.
    //** If student exists (at FINDSTUDENT): delete the node from the list.

    // stNode 
    
    // int st_id = st.id;        //** [SAFER, MORE CORRECT ??]    //**CHECK-FIX.
    
    int checkDelete=0;
    node temp = l->head;

    if((temp->next == NULL) && (temp->previous == NULL))    //** [OR l->head == l->tail (==temp)] Delete when it's the last element of the list.
    {
        l->head = NULL;        //**
        l->tail = NULL;
        
        free(temp);
        
        checkDelete=1;        //** CHECK-FIX.
    }
    
    else if(l->head->data.id == st.id)    // Delete from the beginning of the list.
    {
        // temp = l->head;
        
        l->head = l->head->next;        //** CHECK the other way.
        l->head->previous = NULL;
        free(temp);
        
        checkDelete=1;
    }
    
    else if(l->tail->data.id == st.id)    // Delete from the end of the list.
    {
        temp = l->tail;
        
        l->tail = l->tail->previous;
        l->tail->next = NULL;
        free(temp);
        
        checkDelete=1;
    }

    else    // Delete from a specific position inside the list.
    {
        // temp = l->head;          // Pointer to traverse through the list.
        while(temp->data.id != st.id)    //** NOT NULL: inside findStudent.
        { 
            temp = temp->next;
        }

        temp->previous->next = temp->next;
        temp->next->previous = temp->previous;
        free(temp);
        
        checkDelete=1;
    }

    //++ free(temp);
    //++ checkDelete=1;
    
    // ++ at menu.
    // printf("The student with id = %d has been deleted!", st.id);
    // return checkDelete;
    // return st_id;     //** [SAFER, MORE CORRECT ??]    <--

    return st.id;        //** [OK ??]    //**FIX


    // if st==tail (next==null): deleteFromBeg();        // O(1)
    // if st==head==tail (prev==null && next==null): free(address[pointer]);      // O(1)
    // else: deleteFromPos(st.id);    // Use findStudent();     // O(n/2)
  
    // printf("\nEnter the id of the student which you want to delete: ");    // OK: put as a menu choice.
    //** ++ CHECK the the input (++).
    //** ++ FUNCTION for the CHECKS !!
    // scanf("%d", st.id);      // Save the user's input as the name of the new student.
    
    // return 0;    // [??] return the id of the student who has just been deleted.
}

int updateStudent(Student st, list l)  // update the student's details at the list, based on st.id.
{
    //** CALL: 1st call the findStudent -> place the result as the argument st.
    //** If student exists (at FINDSTUDENT): update the node of the list.

    // int st_id = st.id;     //** FIX-CHANGE
    int checkUpdate=0;        //** CHECK-DELETE IT.
    node temp;
    char newName[MAXSTRING];    // newName string is deleted/freed at the end of the function.

    // CANNOT change student's unique id (given by the application).
    printf("Write the new name of the student with id = %d: ", st.id);
    scanf("%s", newName);
    
    if(l->head->data.id == st.id)    // Update the student at the beginning of the list.
    {
        strcpy(l->head->data.name, newName);
        checkUpdate=1;
    }
    
    else if(l->tail->data.id == st.id)    // Update the student at the end of the list.
    {
        strcpy(l->tail->data.name, newName);
        checkUpdate=1;
    }

    else    // Update the student at a specific position inside the list.
    {
        temp = l->head;          // Pointer to traverse through the list.
        while(temp->data.id != st.id)    //** NOT NULL: inside findStudent.
        { 
            temp = temp->next;
        }

        strcpy(temp->data.name, newName);
        checkUpdate=1;
    }

    // ++ at menu.
    // printf("The student with id = %d has been updated!", st.id);

    return st.id;        //** FIX-CHANGE
    // return checkUpdate;
   
    // return 0;    // [??] return the id of the student who has just been deleted.
}