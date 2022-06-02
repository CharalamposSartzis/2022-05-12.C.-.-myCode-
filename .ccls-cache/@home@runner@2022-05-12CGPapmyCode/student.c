
#include "student.h"    // stdio (for NULL) is included in student.h

int numStudents = 0;    //** CHECK <-- pragma once [DECLARE at student.h].



int getNumStudents()        //** FIX with ifdef - nodef [??].
{
    return ++numStudents;
}

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



list load(char* filename)  // Retrieve the students' list from the file.
{
    list newList = createList();           // The list that will be filled with the loaded data from the file.
    
    FILE* fd = fopen(filename,"r");        // Open file for reading.
    
    if(fd == NULL)
    {
        fprintf(stderr, "File %s does NOT exist or could NOT be opened in load! \nΕmpty list is created and returned.\n", filename);
        return newList;
    }
    
    Student newSt;                // Student struct variable representing the new student that is loaded and added to the list each time.
    Student* newSt_p = &newSt;    // The pointer that holds the address of the new student.
    node newStNode;
    int addSt = 0;        //**FIX

    // int lastID = 0;        //** Holds the id of the last student loaded from the file. +1: is the next available id for the next students added from the user

    int i = 0;
    size_t f_read;
    size_t st_size = sizeof(Student);
    //bool flag = false;    //**

    fseek(fd, 0, SEEK_END);                  //** BE CAREFUL !!
    size_t last_position = ftell(fd);        //** Last pos = (total size) = 
            
    while( (i*st_size) < last_position ) //** CHECK if last pos = ([OK: THIS IS]totalSize <- lastAddress + 1) OR (totalSize - 1) !!
    { 
        //** Create new student node with the data saved at local variables ID and name. Then insert it to the list.
            
        fseek(fd, i*st_size, SEEK_SET);        //** BE CAREFUL !!
        f_read = fread(newSt_p, st_size, 1, fd);        // newSt_p is a POINTER!! NOT a struct Student...
            
        printf("\n\n** FREAD() TEST st details: \t");
        print(*newSt_p);
        printf("\n****\n");

        if(f_read != 1)        //** CHECK    //** If fread==0 -> EOF.
        {
            fprintf(stderr, "Problem while loading from the file %s... \nΕmpty list is created and returned.", filename);
            // break;
            return newList;
        }
        if(newSt_p->id < 1)    //++ len.
        {
            fprintf(stderr, "The #%d student's data was NOT read correctly from the file %s... \nΕmpty list is created and returned.", (i+1), filename);
            // break;
            return newList;
        }
            
        printf("\n\n** TEST new st details: \t");
        print(*newSt_p);
        printf("\n****\n");

        //** FIX [addStudent()].
        addSt = addStudent(*newSt_p, newList);        //** The student gets an ID when added to the list. 
        if(addSt<=0)        //** if(addSt)
        {
            fprintf(stderr, "Problem while adding student with id [%d] to the list...", newSt_p->id);
            //** free(newSt_p);        //** ++ do-while
            break;
        }
        else
        {
            printf("\nStudent with id: [%d] has been loaded and added to the list successfully! \n", addSt);    //** FOR TEST [--]
            // lastID = addSt;
            //** ++
        }

        // free(newSt_p);    //** NOT NEEDED.
            
        ++i;    //**
            
    } //while_end

    // if(f_read == 1)        //** FIX    
    // {
    //     printf("List loaded from the file %s successfully! \n", filename);
    // }
    // else
    // {
    //     fprintf(stderr, "Problem loading list from file %s...\n", filename);
    // }
        
    //++ if nothing wrong
    numStudents=addSt;    //**
        
    fclose(fd);

    if(listIsEmpty(newList))
    {
        //** CHECK at run.
        printf("\nThe file %s has NO data in it! The students' list is empty.\n", filename);
    }
    
    return newList;    // Return the list filled with the file data. When there's an  it's empty
}

void save(char* filename, list l)  // save the students' list at file.
{
    // We use load() at the beginning of the program. Therefore, the list l already exists.
    node temp = l->head;
    Student st;        //**

    int i = 0;
    size_t f_write;        //**
    
    FILE* fd = fopen(filename, "w");
    
    if(fd == NULL)
    {
        fprintf(stderr, "\nCouldn't open/create the file %s...\n", filename);
        // exit (1);
        return;    //**
    }

    if(listIsEmpty(l))
    {
        printf("\nThe list to be saved is empty! The file %s has NO data in it.\n", filename);
            
        return;    //**
    }
     
    // writing all the nodes of the list to the file
    while(temp!=NULL)
    {
        st = temp->data;
        printf("\n ** TEST st details: **\n");    // OK
        print(st);
        printf("\n **** \n\n");
  
        fseek(fd, i*sizeof(Student), SEEK_SET);                 //** 
        f_write = fwrite(&st, sizeof(Student), 1, fd);          //** [!!] &st.  
               
        temp = temp->next;
        ++i;
    }
        
    if(f_write != 0)    //**
    {
        printf("List saved in the file %s successfully! \n", filename);
    }
    else
    {
        fprintf(stderr, "Error while writing in the file %s...\n", filename);
    }
        
    fclose(fd);
// } 
}



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



int addStudent(Student st, list l)      //** CHECK return type [LECTURES]. Add new student to the list (insert at the end/tail of the list).
{    
    // Set the student's data: name and id.  //
    
    // Create the new student node with the data of the new student.
    node stNode = createStNode(&st);      //** [??] MUST BE called AFTER its definition!! 

    //Insert the new student node (stNode) at the end of the list.
    insert(stNode, l);

    //** FREE the student argument, AFTER the function call.
    
    return st.id;    //** [++ HUA labs/lecs] [??] return the id of the student who has just been added to the list.
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

    if((temp->next == NULL) && (temp->previous == NULL))    //** [OR l->head == l->tail (==temp)] Delete when it's the ONLY element of the list.
    {
        l->head = NULL;        //**
        l->tail = NULL;
        
        free(temp);        //** CHECK for totally FREE !!
        
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

    int st_id = st.id;     
    // int checkUpdate=0;        //** CHECK-DELETE IT.
    node temp;
    char newName[MAXSTRING + 1];    // newName string is deleted/freed at the end of the function.

    // CANNOT change student's unique id (given by the application).
    printf("Write the new name of the student with id = %d: ", st_id);
    scanf("%s", newName);    //** ++ LEN CHECK.
    
    if(l->head->data.id == st_id)    // Update the student at the beginning of the list.
    {
        strcpy(l->head->data.name, newName);
        // checkUpdate=1;
    }
    else if(l->tail->data.id == st_id)    // Update the student at the end of the list.
    {
        strcpy(l->tail->data.name, newName);
        // checkUpdate=1;
    }
    else    // Update the student at a specific position inside the list.
    {
        temp = l->head;          // Pointer to traverse through the list.
        while(temp->data.id != st_id)    //** NOT NULL: inside findStudent.
        { 
            temp = temp->next;
        }

        strcpy(temp->data.name, newName);
        // checkUpdate=1;
    }

    // printf("The student with id = %d has been updated!", st_id);        // ++ at menu.

    // return checkUpdate;
    // return 0;    // [??] return the id of the student who has just been deleted.
    
    return st_id;        //** FIX-CHANGE
}