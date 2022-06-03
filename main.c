
#include "student.h"        // Header file with the declarations of all the functions.





int main(int argc, char** argv) 
{
    if(argc < 2)        // We must have at least 2 command line arguments to compile the program.
    {
        fprintf(stderr, "\nYou must enter a file name as a command line argument! Try to compile again.\n\n");
        exit(1);        // ERROR exit.
    }
    if(!checkLen(argv[1]))        // Check the length of the 2nd command line argument: must be between MINSTRING and MAXSTRING.
    {
        fprintf(stderr, "\nYou must enter a file name with a minimum length of 2 and a maximum length of 75 characters! Try to compile again.\n\n");
        exit(1);        // ERROR exit.
    }
    char* filename = argv[1];        // The 2nd command line argument is the name of the file that will be used to load/save the list of students.
    
    list studentList = load(filename);        // We load the list from the file.
    
    Student newSt;        // Student struct variable representing the new student that is added to the list each time.
    Student* st_p;        // The pointer that holds the address of the student we want to find each time (in order to: print details, update or delete).
    int addSt = 0;        // variable to save the return result of the functions: addStudent(), updateStudent() and deleteStudent().
    int searchID = 0;     // variable to save the return result of readInput().
    
    int choice;                // variable to save the user's selection from the menu. 
    int scanReturn;            // variable to save scanf() return.
    bool clearInput;           // variable to save empty_stdin() return.
    
    char upd_del_choice[10];    // A char array that holds the user's answer about editing/deleting a student. 
    int str_cmp_Y;              // Variables to store the results of comparing the answer with the strings "Yes" and "No".
    int str_cmp_N;
    bool isYes;
    bool isNo;
    
    while(1 /*true*/)        
    {  
        printMenu();
    
        printf("\nPlease make a choice: ");
        scanReturn = scanf("%d", &choice);     // save scanf() return: the integer that has been read.
        clearInput = empty_stdin();            // Clear the rest of the input and check for other invalid input data.
        
        if(scanReturn == EOF)     // The case where the user manually generates EOF (End Of File).
        {       
            printf("\n\nUser canceled input... \nYou are going to exit the program...\n");

            save(filename, studentList);        // Save the list into the file: the file is created if it doesn't exist.
            return 1;    // ERROR exit.
        }
        if(scanReturn == 0)    // Reads a character (no integer).
        {   
            printf("\nInvalid input (must be an integer)! Please try again.\n");
        }
        else if(!clearInput)     // Reads an integer but the rest input is NOT clear (contains at least 1 character/letter).
        {            
            printf("\nInvalid input! You must enter only 1 integers and NO characters! \nPlease try again.\n");    //**
        }
        else        // Reads an integer and the rest input is clear: doesn't contain any character/letter.
        {    
            switch(choice)
            {
                case 1:        // Create student.
                {
                    newSt.id = getNumStudents();    // Give id: last available id + 1. The current total number of students created and added 
                                                    //     from the beginning of the program is increased by 1.
                    do
                    {
                        printf("Please give the name of the new student you want to create: ");   
                        scanf("%s", newSt.name);        

                        if(!checkLen(newSt.name))
                        {
                            printf("\nYou must enter a student name with length between 2 and 75 characters! Please try again.\n\n"); //**
                        }
                    }while(!checkLen(newSt.name));        // Display user prompt until the input has the appropriate length.
                    
                    addSt = addStudent(newSt, studentList);        // Add student to the list. Return the student's id.
                    if(addSt>0)        // If id is valid (>=1).
                    {
                        printf("\nThe student has been added successfully! Given id: [%d]. \n", addSt);
                    }
                    else
                    {
                        printf("\nStudent has NOT been added to the list! Please make a choice and try again. \n");
                    }
                    
                    break;
                } //case1-end.
                case 2:        // Find student.
                {
                    if(listIsEmpty(studentList))
                    {
                        printf("\nThe students' list is empty! There isn't any student to search for...\n");
                        break;
                    }
                    
                    searchID = readInput("search for");        // Action: search for a student. Read user's input and return the id given by the user to search.
                    if(searchID < 1)        // Invalid id given.
                    {
                        printf("There is NO student with id <= 0...");
                        break;        // Breaks switch-case: go to main menu.
                    }
                        
                    st_p = findStudent(searchID, studentList);        // Search the list based on the given id.
                    if(st_p != NULL)
                    {
                        printf(" \nStudent's details: ");
                        print(*st_p);        
                    }
                    else    // Student pointer is NULL: the student with the given id NOT found in the list.
                    {
                        printf("\nStudent with id: [%d] doesn't exist in the list!", searchID);    //**
                    }
                        
                    break;
                } //case2-end.
                case 3:        // Print list of students.
                {
                    if(listIsEmpty(studentList))
                    {
                        printf("\nThe students' list is empty! There are NO students, so we can't display their details...\n");
                        break;
                    }
                    
                    printstudents(studentList);    // Print all students' details/information.
                    break;
                } //case3-end.
                case 4:        // Update student.
                {
                    if(listIsEmpty(studentList))
                    {
                        printf("\nThe students' list is empty! There isn't any student to update...\n");
                        break;
                    }
                    
                    searchID = readInput("update");     // Action: update student. Read user's input and return the id given by the user to search and update.
                    if(searchID < 1)        // Invalid id given by user.
                    {
                        printf("There is NO student with id <= 0...");
                        break;        // Breaks switch-case: go to main menu.
                    }
                    
                    st_p = findStudent(searchID, studentList);        // Search the list based on the given id.
                    if(st_p != NULL)
                    {
                        printf("\n Student's details: ");
                        print(*st_p);      

                        do
                        {
                            printf("\n Do you want to update the student with id [%d]? (Yes/No): ", st_p->id);
                            scanf("%s", upd_del_choice);

                            str_cmp_Y = strcmp(upd_del_choice, "Yes");
                            str_cmp_N = strcmp(upd_del_choice, "No");

                            isYes = (str_cmp_Y == 0);
                            isNo = (str_cmp_N == 0);
                            
                            if( !(isYes || isNo) )        
                            {
                                printf("\n Please give a valid answer. \n");
                            }
                        }while( !(isYes || isNo) );        // Ask input from the user until it's "Yes" or "No".

                        if(isNo)        // If the user doesn't want to update this student: go to main menu.
                        {
                            break;
                        }
                        
                        addSt = updateStudent(*st_p, studentList);      // Update the student that found by findStudent().
                        if(addSt>0)        // If the returned id is valid.
                        {
                            printf("\n The student with id: [%d] has been updated successfully! \n New details: ", addSt);
                            print(*st_p);
                        }
                        else
                        {
                            printf("\n Student with id: [%d] has NOT been updated...", addSt);
                        }
                    } //if(st_p!=NULL)_end.
                    else    // Student pointer is NULL: the student with the given id NOT found in the list.
                    {
                        printf("\n Student with id: [%d] doesn't exist in the list!", searchID);    //**
                    }
                    
                    break;
                } //case4-end.
                case 5:        // Delete student.
                {
                    if(listIsEmpty(studentList))
                    {
                        printf("\nThe students' list is empty! There isn't any student to delete...\n");
                        break;
                    }
                        
                    searchID = readInput("delete");    // Action: delete student. Read user's input and return the id given by the user to search and delete.
                    if(searchID < 1)        // Invalid id given by user.
                    {
                        printf("There is NO student with id <= 0...");
                        break;        // Breaks switch-case: go to main menu.
                    }
                    
                    st_p = findStudent(searchID, studentList);        // Search the list based on the given id.
                    if(st_p != NULL)
                    {
                        printf("\n Student's details: ");
                        print(*st_p);        

                        do
                        {
                            printf("\n Do you want to delete the student with id [%d]? (Yes/No): ", st_p->id);
                            scanf("%s", upd_del_choice);

                            str_cmp_Y = strcmp(upd_del_choice, "Yes");
                            str_cmp_N = strcmp(upd_del_choice, "No");

                            isYes = (str_cmp_Y == 0);
                            isNo = (str_cmp_N == 0);
                            
                            if( !(isYes || isNo) )        //**
                            {
                                printf("\n Please give a valid answer. \n");
                            }
                        }while( !(isYes || isNo) );        // Ask input from the user until it's "Yes" or "No".

                        if(isNo)        // If the user doesn't want to delete this student: go to main menu.
                        {
                            break;      // Breaks out of switch-case: go to main menu.
                        }
                        
                        addSt = deleteStudent(*st_p, studentList);
                        if(addSt>0)        // If the returned id is valid.
                        {
                            printf("\n The student with id: [%d] has been deleted successfully!", addSt);
                        }
                        else
                        {
                            printf("\n Student with id: [%d] has NOT been deleted...", addSt);
                        }
                    } //if(st_p!=NULL)_end.
                    else        // Student pointer is NULL: the student with the given id NOT found in the list.
                    {
                        printf("\nStudent with id: [%d] doesn't exist in the list!", searchID);    //**
                    }
                        
                    break;
                } //case5-end.
                case 0:        // Exit the program.
                {
                    printf("\nYou are going to exit the program...\n");

                    save(filename, studentList);        // Save the list into the file: the file is created if it doesn't exist.
                    
                    exit(0);            // Normal exit.
                } //case0-end.
                default:        // The choice is out of the range [1...5].
                {
                    printf("Integer input is out of range [0-5]! Please try again.\n");
                }
            } //switch 
        } //else: input is clear.
    } //while
    
    return 0;        // Normal main return / end of program.
}
