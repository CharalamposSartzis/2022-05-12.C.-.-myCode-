
//#include <stdio.h>      // stdio (for NULL) is included in student.h
#include "student.h"
//#include <string.h>



int main(int argc, char** argv)        //** CHANGE args.
{
    if(argc < 2)
    {
        fprintf(stderr, "\nYou must enter a file name as a command line argument! Try to compile again.\n\n");
        exit(1);
    }
    int argLen = strlen(argv[1]);
    if(argLen < 2)        //** ++ CHECK for MAXSTRING/MAXLEN.
    {
        fprintf(stderr, "\nYou must enter a file name with a minimum length of 2! Try to compile again.\n\n");
        exit(1);
    }
    char* filename = argv[1];
    
    // createList();          //** LOOK at head and tail values: if NULL.
    list studentList = load(filename);
    
    //  char newStName[MAXSTRING + 1];        //** FIX
    Student newSt;        // Student struct variable representing the new student that is added to the list each time.
    Student* st_p;        // The pointer that holds the address of the student we want to find each time (in order to: print details, update or delete).
    int addSt = 0;        // variable to save the return result of addStudent().
    int searchID = 0;     // variable to save the return result of readInput().
    
    /*// if(choice)   ==  if(choice NOT zero)
    // {
    //     printf("true");
    // }
    // else
    // {
    //     printf("false");
    // }*/

    int choice;            // variable to save the user's selection from the menu. 
    int scanReturn;        // variable to save scanf() return.
    bool clearInput;       // variable to save empty_stdin() return.
    
    while(1 /*true*/)        //**
    {  
        printMenu();
    
        printf("\nPlease make a choice: ");
        scanReturn = scanf("%d", &choice);    /* save return */
        clearInput = empty_stdin();        //** CHECK
        
        if(scanReturn == EOF) /* user generates manual EOF */
        {       
            printf("\n\nUser canceled input... \nYou are going to exit the program...\n");
            //** ++ Do you want to exit the program? ...

            save(filename, studentList);
            
            return 1;    //** ERROR exit.
            // exit(1);     //**
        }
        if(scanReturn == 0)    //** Reads a character.
        {   
            printf("\nInvalid input (must be an integer)! Please try again.\n");
        }
        else if(!clearInput /*!empty_stdin()*/)     //** Reads an integer but the rest input is NOT clear (contains at least 1 character/letter).
        {            
            printf("\nInvalid input! You must enter only 1 integers and NO characters! \nPlease try again.\n");    //**
        }
        else        //** Reads an integer and the rest input is clear: doesn't contain any character/letter.
        {    
            switch(choice)
            {
                case 1:        // Create student.
                {
                    newSt.id = getNumStudents();    //** FIX with ifdef - nodef [??].
                    printf("Please give the name of the new student you want to create: ");    //** ++ CHECK for strlen.
                    scanf("%s", newSt.name);        //** CHECK over-write.

                    //** Also in load().
                    addSt = addStudent(newSt, studentList);        //** The student gets an ID when added to the list. 
                    if(addSt>0)        //** if(addSt)
                    {
                        printf("\nThe student has been added successfully! Given id: [%d]. \n", addSt);
                        // free(newSt_p);        //** ++ MORE CHECKS.
                    }
                    else
                    {
                        printf("\nStudent has NOT been added to the list! Please make a choice and try again. \n");
                        //** free(newSt_p);        //** ++ do-while
                        //** ++
                    }
                    
                    // free(newSt_p);        //** NOT NEEDED: NO stud malloc creation.
                    
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
                        
                    st_p = findStudent(searchID, studentList);        //** CHANGE name of st* variable.
                    if(st_p != NULL)
                    {
                        printf(" \nStudent's details: ");
                        print(*st_p);        //++

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
                    
                    st_p = findStudent(searchID, studentList);
                    if(st_p != NULL)
                    {
                        printf(" Student's details: ");
                        print(*st_p);        //++
                        
                        addSt = updateStudent(*st_p, studentList);
                        if(addSt>0)        //** if(addSt)
                        {
                            printf("\n The student with id: [%d] has been updated successfully! \n New details: ", addSt);
                            print(*st_p);
                        }
                        else
                        {
                            printf("\n Student with id: [%d] has NOT been updated...", st_p->id);
                            // free(newSt_p);        //** ++ do-while
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
                    
                    st_p = findStudent(searchID, studentList);
                    if(st_p != NULL)
                    {
                        printf(" Student's details: ");
                        print(*st_p);        //++

                        //++ Do you want to delete him ??
                        
                        addSt = deleteStudent(*st_p, studentList);
                        if(addSt>0)        //** if(addSt)
                        {
                            printf("\n The student with id: [%d] has been deleted successfully!", addSt);
                            //++ MORE CHECKS.
                        }
                        else
                        {
                            printf("\n Student with id: [%d] has NOT been deleted...", st_p->id);
                            // free(newSt_p);        //** ++ do-while
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

                    save(filename, studentList);        //**
                    
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
