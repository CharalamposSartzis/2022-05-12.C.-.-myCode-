
//#include <stdio.h>      // stdio (for NULL) is included in student.h
#include "student.h"
//#include <string.h>

int main(int argc, char **argv)        //** CHANGE args.
{
    // if (argc >= 2)
    //      fp = fopen(argv[1], "w");
    // else fp = fopen("file.txt", "w");
    char* filename;
    
    if(argc >= 2)
    {
        filename = argv[1];    //** should be: "students.txt"
    }
    else//
    {
        //**FIX: EOF/don't start program if filename NOT given.
        filename = "students_2.txt";
    }
    
    //** INSIDE load().
    // list studentList = createList();          //** LOOK at head and tail values: if NULL.
    list studentList = load(filename);
    
    // char* newStName;           //**
    Student* newStudent;       //**
    int addSt = 0;        //**
    int searchID = 0;     //**
    
    /*// if(choice)   ==  if(choice NOT zero)
    // {
    //     printf("true");
    // }
    // else
    // {
    //     printf("false");
    // }*/

    int choice = 0,
        scanReturn = 0;    /* variable to save scanf return */
    bool clearInput;

    bool scan_EOF;
    bool validInput;
    bool break_loop_input;
    
    while(1 /*true*/)        //**
    {  
        printMenu();
    
        printf("\nPlease make a choice: ");
        scanReturn = scanf("%d", &choice);    /* save return */
        clearInput = empty_stdin();        //** CHECK
        
        if(scanReturn == EOF) /* user generates manual EOF */
        {       
            printf("\n\nUser canceled input...\n");
            //** ++ Do you want to exit the program? ...
            return 1;    //**
            // exit(0);     //**
        }
        else if(scanReturn == 0)    //** Reads a character.
        {   
            printf("\nInvalid input (must be an integer)! Please try again.\n");
        }
        else if(!clearInput /*!empty_stdin()*/)     //** Reads an integer but the rest input is NOT clear (contains at least 1 character/letter).
        {            
            printf("\nInvalid input! You must enter only 1 integers and NO characters! \nPlease try again.\n");    //**
        }
        else        //** Reads an integer and the rest input is clear: doesn't contain any character/letter.
        {
            printf("\n** [Valid choice.] **\n\n");        //** ONLY for CHECK.
            //empty_stdin();
            
            switch(choice)
            {
                case 1:        // Create student.
                {
                    newStudent = createStudent();
                    printf("--name: %s\n", newStudent->name);    //** CHECK the name.
                    printf("Please give the name of the new student you want to create: ");    //**
                    scanf("%s", newStudent->name);            //** CHECK over-write.
                    printf("--name: %s\n", newStudent->name);    //** CHECK the saved name.
                        
                    addSt = addStudent(*newStudent, studentList);        //** The student gets an ID when added to the list. 
                    if(addSt>0)        //** if(addSt)
                    {
                        printf("The student has been added successfully! Given id: [%d]. ", addSt);
                        free(newStudent);        //**
                    }
                    else
                    {
                        printf("Student has NOT been added to the list...");
                        // free(newStudent);        //** ++ do-while
                        //** ++
                    }
                    
                    break;
                }
                case 2:        // Find student.
                {
                    searchID = readInput("search for");

                    if(searchID == 0)        //** CHECK at run-time.
                    {
                        // printf("User canceled student search...");
                        break;        //** Breaks switch-case.
                    }
                        
                    newStudent=findStudent(searchID, studentList);        //** CHANGE name of st* variable.
                    if(newStudent != NULL)
                    {
                        printf(" \nStudent's details: ");
                        print(*newStudent);        //++

                        // free(newStudent);        //** HERE is the PROBLEM: at [case 3] prints the id BUT NOT the name [name: GARBAGE].
                    }
                    else
                    {
                        printf("\nStudent with id: [%d] doesn't exist in the list!", searchID);    //**
                    }
                        
                    break;
                }
                case 3:        // Print list of students.
                {
                    printstudents(studentList);
                    break;
                }
                case 4:        // Update student.
                {
                    searchID = readInput("update");

                    if(searchID == 0)        //** CHECK at run-time.
                    {
                        // printf("User canceled student's update...");
                        break;        //** Breaks switch-case.
                    }
                    
                    newStudent = findStudent(searchID, studentList);
                    if(newStudent != NULL)
                    {
                        printf(" Student's details: ");
                        print(*newStudent);        //++
                        
                        addSt = updateStudent(*newStudent, studentList);
                        if(addSt>0)        //** if(addSt)
                        {
                            printf("\n The student with id: [%d] has been updated successfully! \n New details: ", addSt);
                            print(*newStudent);
                        }
                        else
                        {
                            printf("\n Student with id: [%d] has NOT been updated...", newStudent->id);
                            // free(newStudent);        //** ++ do-while
                            //** ++
                        }
                    }
                    else
                    {
                        printf("\nStudent with id: [%d] doesn't exist in the list!", searchID);    //**
                    }
                    
                    break;
                }
                case 5:        // Delete student.
                {
                    // //student {st.id, name=""};
                        
                    searchID = readInput("delete");

                    if(searchID == 0)        //** CHECK at run-time.
                    {
                        // printf("User canceled student's delete...");
                        break;        //** Breaks switch-case.
                    }
                    
                    newStudent = findStudent(searchID, studentList);
                    if(newStudent != NULL)
                    {
                        printf(" Student's details: ");
                        print(*newStudent);        //++

                        //++ Do you want to delete him ??
                        
                        addSt = deleteStudent(*newStudent, studentList);
                        if(addSt>0)        //** if(addSt)
                        {
                            printf("\n The student with id: [%d] has been deleted successfully!", addSt);
                        }
                        else
                        {
                            printf("\n Student with id: [%d] has NOT been deleted...", newStudent->id);
                            // free(newStudent);        //** ++ do-while
                            //** ++
                        }
                    }
                    else
                    {
                        printf("\nStudent with id: [%d] doesn't exist in the list!", searchID);    //**
                    }
                        
                    break;
                }
                case 0:        // Exit the program.
                {
                    printf("\nYou are going to exit the program...\n");
                        
                    // return 1;        //** [??]
                    exit(0);            //**
                    // return 1;        //** [??]
                }
                default:
                {
                    printf("Integer input is out of range [0-5]! Please try again.\n");
                }
            } //switch 
        } //else: clear input.
    } //while
    
    return 0;        //** [??]
}