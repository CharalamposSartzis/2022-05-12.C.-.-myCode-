
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
            
        /*printf("\n\n** FREAD() TEST st details: \t");
        print(*newSt_p);
        printf("\n****\n");*/

        if(f_read != 1)        //** CHECK    //** If fread==0 -> EOF.
        {
            clearList(newList);
            fprintf(stderr, "Problem while loading from the file %s... \nReturn an empty/cleared list.", filename);
            
            return newList;
        }
        if(newSt_p->id < 1)    //++ len.
        {
            clearList(newList);
            fprintf(stderr, "The #%d student's data was NOT read correctly from the file %s... \nReturn an empty/cleared list.", (i+1), filename);
            
            return newList;
        }
            
        /*printf("\n\n** TEST new st details: \t");
        print(*newSt_p);
        printf("\n****\n");*/

        addSt = addStudent(*newSt_p, newList);        //** FIX addStudent().
        if(addSt<=0)        //**FIX         //** if(addSt)
        {
            fprintf(stderr, "Problem while adding student with id [%d] to the list...", newSt_p->id);
            break;
        }
        else
        {
            printf("\nStudent with id: [%d] has been loaded and added to the list successfully! \n", addSt);    //** FOR TEST [--]
            // lastID = addSt;
            //** ++
        }
     
        ++i;    //**
        
    } //while_end
    
    //++ if nothing wrong
    numStudents=addSt;    //**

    printf("List loaded from the file %s successfully! \n", filename);
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
        /*printf("\n ** TEST st details: **\n");    // OK
        print(st);
        printf("\n **** \n\n");*/
  
        fseek(fd, (st.id - 1)*sizeof(Student), SEEK_SET);                 //** 
        f_write = fwrite(&st, sizeof(Student), 1, fd);          //** [!!] &st.  

        if(f_write != 1) 
        {
            fprintf(stderr, "Error while writing in the file %s...\n", filename);
            return;
        }
        
        temp = temp->next;
        // ++i;
    }
        
    printf("List saved in the file %s successfully! \n", filename);
    fclose(fd);
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

void clearList(list l)    //**
{
    if(!listIsEmpty(l))        //** We have at least one (1) node in the list.
    {
        node clearNode_p;
        
        while(l->head != l->tail)       //** Each time: delete from the beginning/head of the list.
        {
            clearNode_p = l->head;
            
            l->head = l->head->next;        //** Delete from the beginning of the list.
            l->head->previous = NULL;
        
            free(clearNode_p);
        }
        
        // if(clearNode_p == l->tail)     //**[If: l->head == l->tail] If it's the only node of the list.
        l->head = NULL;     //** If we only do [l->head = NULL]: OK [l->head == l->tail], if we make 1 null -> the other will be null too.
        l->tail = NULL;     //**     SAME: l->head = l->tail = NULL;
        
        free(clearNode_p);      //** CHECK for totally FREE !!
        // checkDelete=1;       //** CHECK-FIX.
        
        printf("\nΤhe list has been cleared... \n");        //** FOR TEST.
    }
    else
    {
        printf("\nΤhe list is already empty! \n");
    }
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
    //** 1st call the findStudent(st.id, l) [CHECKs if list is EMPTY ++] -> place the result as the argument st.
    //** If student exists (at FINDSTUDENT): delete the node from the list.
    
    int st_id = st.id;        //** [SAFER, MORE CORRECT ??]    //**CHECK-FIX.
    
    // int checkDelete=0;    //**FIX
    
    node delNode_p = l->head;

    if(delNode_p->data.id == st_id)    //** 
    {
        if((delNode_p->next == NULL) && (delNode_p->previous == NULL))     //**[OR l->head == l->tail (==temp)] If it's the only node of the list.
        {
            l->head = NULL;        //**
            l->tail = NULL;
        
            // free(delNode_p);      //** CHECK for totally FREE !!
            // checkDelete=1;        //** CHECK-FIX.
        }
        
        l->head = l->head->next;        //** Delete from the beginning of the list.
        l->head->previous = NULL;
        
        // free(delNode_p);
        // checkDelete=1;
    }
    else if(l->tail->data.id == st_id)    // Delete from the end of the list.
    {
        delNode_p = l->tail;
        
        l->tail = l->tail->previous;
        l->tail->next = NULL;
        
        // free(delNode_p);
        // checkDelete=1;
    }
    else    // Delete from a specific position inside the list.
    {
        //** delNode_p = l->head;          // Pointer to traverse through the list.
        while(delNode_p->data.id != st_id)    //** NOT NULL: inside findStudent.
        { 
            delNode_p = delNode_p->next;
        }

        delNode_p->previous->next = delNode_p->next;
        delNode_p->next->previous = delNode_p->previous;
        
        // free(delNode_p);
        // checkDelete=1;
    }

    free(delNode_p);        //**
    
    // printf("The student with id = %d has been deleted!", st.id);     // ++ at menu.
    
    return st_id;     //** [SAFER, MORE CORRECT ??]    <--
    
    // return checkDelete;
    // return 0;    // [??] return the id of the student who has just been deleted.
}

int updateStudent(Student st, list l)  // update the student's details at the list, based on st.id.
{
    //** CALL: 1st call the findStudent -> place the result as the argument st.
    //** If student exists (at FINDSTUDENT): update the node of the list.

    int st_id = st.id;     
    // int checkUpdate=0;        //** CHECK-DELETE IT.
    node updNode_p;                 // Pointer that holds the address of the node that its data (student) is going to be updated.
    char newName[MAXSTRING + 1];    //**

    // CANNOT change student's unique id (given by the application).
    printf("Write the new name of the student with id = %d: ", st_id);
    scanf("%s", newName);    //** ++ LEN CHECK.
    
    if(l->head->data.id == st_id)    // Update the student at the beginning of the list.
    {
        updNode_p = l->head;
        // strcpy(l->head->data.name, newName);
        // checkUpdate=1;
    }
    else if(l->tail->data.id == st_id)    // Update the student at the end of the list.
    {
        updNode_p = l->tail;
        // strcpy(l->tail->data.name, newName);
        // checkUpdate=1;
    }
    else    // Update the student at a specific position inside the list.
    {
        updNode_p = l->head;          // Pointer to traverse through the list.
        while(updNode_p->data.id != st_id)    //** NOT NULL: inside findStudent.
        { 
            updNode_p = updNode_p->next;
        }

        // strcpy(updNode_p->data.name, newName);
        // checkUpdate=1;
    }

    strcpy(updNode_p->data.name, newName);        //**
    
    // printf("The student with id = %d has been updated!", st_id);        // ++ at menu.
    
    return st_id;        //** FIX

    // return checkUpdate;
    // return 0;    // [??] return the id of the student who has just been deleted.
}