
#include "student.h"    // stdio (for NULL) is included in student.h

int numStudents = 0;                    //**
size_t st_size = sizeof(Student);        //**





int getNumStudents()        // returns the number of students increased by one each time, in order to assign a unique id to each student.
{
    return ++numStudents;
}

void printMenu()            // prints the main menu with the choices.
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

int readInput(char* action)        // reads the user's input and returns the id given by the user for search/update/delete.
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

/* FROM: stackoverflow.com: How to clear user input and check if enters characters instead of an integer. */
bool empty_stdin()    // Function to empty stdin and check for the rest of the user input. Clears the rest of the user's input, after reading an integer.
{
    int cnt = 0;
    int inputChar;

    bool notSPACE; 
    bool notTAB; 
    bool notENTER;
    bool notEOF; 
    
    do
    {
        inputChar = getchar();      
        
        notSPACE = (inputChar!=' ');        
        notTAB = (inputChar!='\t');
        notENTER = (inputChar!='\n');
        notEOF = (inputChar!=EOF);
            
        if(notSPACE && notTAB && notENTER && notEOF)   // Count all the characters that are NOT: SPACE, TAB, ENTER, EOF.
        {
            ++cnt;
        }
        
    }while(notENTER && notEOF);        // Continue until the user presses ENTER or EOF.
    
    return (cnt==0);        // Correct/valid input: doesn't have any other characters after the integer that has been read.
}

bool checkLen(char* string)        // checks the length of the strings: must be between MAXSTRING and MINSTRING.
{
    int s_length = strlen(string);
    return (s_length >= MINSTRING) && (s_length <= MAXSTRING);
}



void print(Student st)
{
    printf("\n id: %-5d | name: %-13s \n", st.id, st.name);          // prints a student's details using a specified format.
}

void printstudents(list l)
{
    node traversal_p = l->head;          // Pointer to traverse through the list.
    printf("\n ---\tAll students' details: \t--- \n");

    while(traversal_p != NULL)            // prints all the students of the list.
    {
        print(traversal_p->data);            // Print the details of the current student each time.
        traversal_p = traversal_p->next;
    }
    printf("\n\n");
}



list load(char* filename)  // Retrieve the students' list from the file.
{
    list newList = createList();           // Create/initialize an empty list that will be filled with the loaded data from the file.
    
    FILE* fd = fopen(filename,"r");        // Open file for reading.
    
    if(fd == NULL)
    {
        fprintf(stderr, "File '%s' does NOT exist or could NOT be opened in load! \nΕmpty list is created and returned.\n", filename);
        return newList;
    }
    
    Student newSt;        // Student struct variable representing the new student that is loaded and added to the list each time.
    int addSt = 0;        // variable to hold the result returned by the addStudent().

    int i = 0;            // index for iteration of the binary file.
    size_t f_read;        // variable to hold the result of fread().

    fseek(fd, 0, SEEK_END);                  // move the file pointer to the end of the file.
    size_t last_position = ftell(fd);        // last_position = total size of file's saved data = (last byte's position + 1).
            
    while( (i*st_size) < last_position )     // Repeat until reaching the last byte's position.
    {      
        fseek(fd, i*st_size, SEEK_SET);            // move the file pointer at [i * st_size(size of Student)] bytes from the beginning of the file.
        f_read = fread(&newSt, st_size, 1, fd);       // read the data from the file pointer and store them into student newSt.
            
        if(f_read != 1)        // Fread must read 1 element. Otherwise: problem while loading.
        {
            fprintf(stderr, "\nProblem while loading from the file '%s'... \n", filename);
            exit(1);        // ERROR exit.
        }
        if(newSt.id < 1)    // Student(s) with id<1 (invalid id) would NOT be added to the list!
        {
            fprintf(stderr, "\nIncorrect/invalid student data loaded from the file '%s': id [%d] of student #%d ! \n", filename, (i+1), newSt.id);
            exit(1);        // ERROR exit.
        }
            
        addSt = addStudent(newSt, newList);        // add new student (with the loaded data) in the list.
        if(addSt < 1)        // If it returns invalid id.
        {
            fprintf(stderr, "Problem while adding student with id [%d] to the list...", newSt.id);
            exit(1);        //** ERROR exit.
        }
    
        ++i;    // Go to the next student struct inside the file.
        
    } //while-end
    
    // if nothing wrong: set the last available id as the id of the last element loaded from the file (last id). The next student that's
    // going to be added (this time by the user) will have id = last_id + 1.
    numStudents=addSt;    //**

    printf("List loaded from the file '%s' successfully! \n", filename);
    fclose(fd);        // close file.

    if(listIsEmpty(newList))
    {
        printf("\nThe file '%s' has NO data in it! The students' list is empty.\n", filename);
    }
    
    return newList;    // Return the list filled with the file data.
}

void save(char* filename, list l)  // save the students' list at file.
{
    // We use load() at the beginning of the program. Therefore, the list l already exists.
    node temp = l->head;    // node pointer to traverse through the list.
    Student st;            // Struct student that holds the temporary student data that are going to be written to the file.

    int i = 0;
    size_t f_write;        //  variable to hold the result of fwrite().
    
    FILE* fd = fopen(filename, "w");        // Open file for writing. If NOT exists -> creates new empty file.
    
    if(fd == NULL)
    {
        fprintf(stderr, "\nCouldn't open/create the file '%s'...\n", filename);
        return;    
    }

    if(listIsEmpty(l))
    {
        fclose(fd);
        printf("\nThe saved list is empty! The file '%s' has NO data in it.\n", filename);
        return;    
    }
     
    // writing all the nodes of the list into the file.
    while(temp!=NULL)
    {
        st = temp->data;            // hold the temp's Student data.
  
        fseek(fd, i*st_size, SEEK_SET);             // move the file pointer at [i * st_size(size of Student)] bytes from the beginning of the file. 
        f_write = fwrite(&st, st_size, 1, fd);      // write a student's data (each time) from the address of st to the point of file pointer. 

        if(f_write != 1)     // Fwrite must write 1 element. Otherwise: problem while writing.
        {
            fprintf(stderr, "\nError while writing to the file '%s'...\n", filename);
            exit(1);        //** ERROR exit.
        }
        
        temp = temp->next;    // Go to the next node of the list.
        ++i;        // Go to the next point (size of student bytes) inside the file.
    }

    // If nothing wrong.
    printf("List saved in the file '%s' successfully! \n", filename);
    fclose(fd);        // close file.
}



list createList()        // creates/initializes the doubly-linked list.
{
    list newList = (list)malloc(sizeof(struct listR));

    newList->head = NULL;
    newList->tail = NULL;

    return newList;
}

bool listIsEmpty(list l)        // checks if the list is empty.
{
    return ((l->head == NULL) && (l->tail == NULL));    // list empty: If head == tail == NULL.
}



node createStNode(Student* st_p)     // create a new node with the student's data.
{
    node stNode = (node)malloc(sizeof(struct nodeR));    // Allocate heap memory with the size of a node with the    
                                                         // student's data (struct nodeR).
    // COPIES the content of st_p -> to the data of the stNode.
    stNode->data = *st_p;    
    stNode->next = NULL;
    stNode->previous = NULL;

    return stNode;    
}

void insert(node stNode, list l)        // Insert a new student node at the end (tail) of the list.
{
    if(listIsEmpty(l))      // If list is empty: set the new node as the head and the tail of the list.
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



int addStudent(Student st, list l)      // Add new student to the list (insert at the end/tail of the list).
{    
    // Create the new student node with the data of the new student.
    node stNode = createStNode(&st);       

    //Insert the new student node (stNode) at the end of the list.
    insert(stNode, l);
    
    return st.id;    // return the id of the student who has just been added to the list.
}

Student* findStudent(int id, list l)      // search for the student at the list, based on id.
{
    // If list is empty return NULL pointer (no students in the list). 
    if(listIsEmpty(l))
    {
        printf("\nThe list is empty!\n");
        return NULL;    
    }
    
    // check if student is at head
    if(l->head->data.id == id)
    {
        return &(l->head->data);        // return the address of the student (at head).
    }

    // check if student is at tail.
    if(l->tail->data.id == id)
    {
        return &(l->tail->data);      // return the address of the student (at tail).
    }
    
    node traversal_p = l->head;          // Pointer to traverse through the list.
    while(traversal_p != NULL)    
    {
        if(traversal_p->data.id == id)
        {
            return &(traversal_p->data);    // return the address of the student (at specified position).   
        }
        traversal_p = traversal_p->next;   
    }

    return NULL;      // If student not found.
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
        if((delNode_p->next == NULL) && (delNode_p->previous == NULL))  //**[OR l->head == l->tail (==temp)] If it's the only node of the list.
        {
            l->head = NULL;        //**
            l->tail = NULL;
        
            // checkDelete=1;        //** CHECK-FIX.
        }
        else
        {
            l->head = l->head->next;        //** Delete from the beginning of the list.
            l->head->previous = NULL;

            // checkDelete=1;
        }
    }
    else if(l->tail->data.id == st_id)    // Delete from the end of the list.
    {
        delNode_p = l->tail;
        
        l->tail = l->tail->previous;
        l->tail->next = NULL;
        
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
    int st_id = st.id;     
    node updNode_p;                 // Pointer that holds the address of the node that its data (student) is going to be updated.
    char newName[MAXSTRING + 1];    // char array to hold the new name and set it as the new name.

    do
    {
        // CANNOT change student's unique id (given by the application).
        printf("Write the new name of the student with id = %d: ", st_id);
        scanf("%s", newName);            // Read the new name from the user's input.        

        if(!checkLen(newName))
        {
            printf("\nYou must enter a student name with length between 2 and 75 characters! Please try again.\n\n"); //**
        }
    }while(!checkLen(newName));        // Repeat until the given name has the correct number of characters.
    
    if(l->head->data.id == st_id)    // Set the student's Update node the at the beginning of the list.
    {
        updNode_p = l->head;
    }
    else if(l->tail->data.id == st_id)    // Set the student's Update node the at the end of the list.
    {
        updNode_p = l->tail;
    }
    else    // Set the student's Update node the at a specific position in the list.
    {
        updNode_p = l->head;          // Pointer to traverse through the list.
        while(updNode_p->data.id != st_id)       // Continue until the id of the data is the same with the st.id.
        { 
            updNode_p = updNode_p->next;
        }
    }

    strcpy(updNode_p->data.name, newName);      // Copy and set the new name as the name of the student inside the Update node.
       
    return st_id;      
}