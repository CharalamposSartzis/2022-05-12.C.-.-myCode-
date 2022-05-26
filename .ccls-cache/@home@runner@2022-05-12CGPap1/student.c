
#include "student.h"    // stdio (for NULL) is included in student.h

int numStudents = 0;    //** CHECK <--



void printMenu()
{
    //** CHECK/FIX 3 and 5 -> together
    //** (BEAUTIFY).
    printf("\nMenu choices:\n1. Create new student.\n2. Delete a student.\n3. Find a student.\n4. Update a student's details.\n5. Print a student's details.\n6. Print all students' details.\n\nPlease make a choice: ");
}



void print(Student st)
{
    printf("Student with id: %d | name: %s \n", st.id, st.name);
}

void printstudents(list l)
{
    node traversal_p = l->head;          // Pointer to traverse through the list.
    printf("All students' details: ");

    while(traversal_p != NULL)    //**
    {
        // printf("Student with id: %d | name: %s \n", traversal_p->data.id, traversal_p->data.name);    //** OK
        print(traversal_p->data);            // Print the details of the current student each time.
        traversal_p = traversal_p->next;
    }
    printf("\n\n");
}



//list load(char* filename);  // retrieve the students' list from file.
//void save(char* filename, list l);  // save the students' list at file.



node createStNode(Student st)    // create a new node with the student's data.
{
    node stNode = (node)malloc(sizeof(struct nodeR));    // Allocate heap memory with the size of a node with the    
                                                         // student's data (struct nodeR).
    stNode->data = st;
    stNode->next = NULL;
    stNode->previous = NULL;

    return stNode;    //** <--
}

void insert(node stNode, list l)        // Insert a new student node at the end (tail) of the list.
{
    if((l->head == NULL) && (l->tail == NULL))      //**(CHECK)  If list is empty: set the new node as the head and the tail of the list.
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



int addStudent(Student st, list l)  // add new student to the list (insert at the end/tail of the list).
{
    // Set the student's data: name and id.  //
    
    printf("\nEnter the student's name: ");
    //** ++ CHECK the length of the input (++).
    scanf("%s", st.name);      // Save the user's input as the name of the new student.

    ++numStudents;    // Every call of the function adds a new student and increases the number 
                      // of students at the list.
    st.id=numStudents;         // 1st student has id=1, 2nd student has id=2 and so on.

  
    // Create the new student node with the data of the new student.
    node stNode = createStNode(st);      //** MUST BE called AFTER its definition!! 

    //Insert the new student node (stNode) at the end of the list.
    insert(stNode, l);

    return st.id;    // [??] return the id of the student who has just been added to the list.
}

Student* findStudent(int id, list l)  //** [return type ??] search for the student at the list, based on id.
{
    // // If student does not exist.
    // if(strlen(st.name)==0)    //** CHECK for the length at user input.
    // {
    //     printf("Student does not exist...");
    //     return 0;
    // }

    // Student* st_p = NULL;    //** CHECK.
    
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
    while(traversal_p != NULL)    //** (...&& traversal_p->data.id != id)
    {
        if(traversal_p->data.id == id)
        {
            return &traversal_p->data;    // return the address of the student.    //** CHECK.
            // st_p = &traversal_p->data;
            // break;    
        }
        traversal_p = traversal_p->next;    // O(n) [ O(n/2) ].
    }

    return NULL;    //** [??]    // If student not found.
    // return st_p;
}

int deleteStudent(Student st, list l)  // delete the student from the list, based on st.id.
{
    //** CALL: deleteStudent(*findStudent, myList); 
    
    // If student exists: delete the node from the list.

    Student* st_p = &st;    //** CHECK ++.
    if(st_p == NULL)        //** CHECK.
    {
        return 0;
    }
    
    // if st==head (prev==null): deleteFromBeg();        // O(1)
    // if st==tail (next==null): deleteFromBeg();        // O(1)
    // if st==head==tail (prev==null && next==null): free(address[pointer]);      // O(1)
    // else: deleteFromPos(st.id);    // Use findStudent();     // O(n/2)
  
    // printf("\nEnter the id of the student which you want to delete: ");    // OK: put as a menu choice.
    //** ++ CHECK the the input (++).
    //** ++ FUNCTION for the CHECKS !!
    // scanf("%d", st.id);      // Save the user's input as the name of the new student.
    
    return 0;    // [??] return the id of the student who has just been deleted.
}

int updateStudent(Student st, list l)  // update the student's details at the list, based on st.id.
{
    //** CALL: updateStudent(*findStudent, myList);
    
    // If student exists: update the node of the list.
    
    Student* st_p = &st;    //** CHECK ++.
    if(st_p == NULL)        //** CHECK.
    {
        return 0;
    }
    
    //++ setName, CANNOT change id (given by the application).

    return 0;    // [??] return the id of the student who has just been deleted.
}