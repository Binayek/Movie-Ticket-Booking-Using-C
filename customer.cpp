//Customer side interface for movie booking system

//declaration section
#include<stdio.h>
#include <stdlib.h>
#include<conio.h>
#include<string.h>
#include <stdbool.h>

void view();
void book();
int login();

//for generating file name
struct date {
    int m, day;        
};

int main(){
	char n;
	
	char d=' ';
	printf("\n\n%49cMovie Ticket Booking System\n",d); 
	
	top:
	
	printf("\nwhat do you want to do? press respective number\n");
	printf("1. view available movies and seats \n2. book movies \n3. press any number to exit");
	scanf("%d",&n);
	switch(n){
		case 1:
			view();
			break;
		case 2:
			book();
			break;
		default:
		printf("invalid choice");
	}
	system("cls");
	printf("Want to continue press Y else press anything\n");
		n=getch();
		if(n=='y'||n=='Y'){
			goto top;
		}
	return 0;
}
void view() {
	system("cls");
    printf("Enter date (month and day) to view movie: ");
    struct date d;
    scanf("%d %d", &d.m, &d.day);
    
    char filename[20];
    sprintf(filename, "%02d_%02d.txt", d.m, d.day); // Generate filename
    
    FILE *file = fopen(filename, "r"); // Open file for reading and writing
    if (file == NULL) {
        printf("No movies for this day.\n");
        return;
    }
    	
	int line_number,i;
    printf("Movie schedule for %02d/%02d:\n", d.m, d.day);
    printf("--------------------------------\n");
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
        line_number++;
    }
    printf("--------------------------------\n");
    fclose(file);
}


void book(){
	system("cls");
	printf("\n\nyou need to login first\n\n");
	b:
	int a=login();
	if(a==0){
		printf("you have to login first");
		goto b;
	}
    printf("Enter date (month and day) to edit movie: ");
    struct date d;
    scanf("%d %d", &d.m, &d.day);
    
    char filename[20];
    sprintf(filename, "%02d_%02d.txt", d.m, d.day); // Generate filename
    
    FILE *file = fopen(filename, "r+"); // Open file for reading and writing
    if (file == NULL) {
        printf("No movies for this day.\n");
        return;
    }
    
    
	
	int line_number,i;
    printf("Movie schedule for %02d/%02d:\n", d.m, d.day);
    printf("--------------------------------\n");
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
        line_number++;
    }
    printf("--------------------------------\n");
    
    // Ask which movie to bool
    int movie_choice;
    printf("Enter the movie number you want to book: press 1 for first movie; 2 for second and so on");
    scanf("%d", &movie_choice);
    
	rewind(file);		//fseek(file, 0, SEEK_SET); // Move file pointer to the beginning
    
    system("cls"); 
    
    // Skip lines until the chosen movie
    for (i = 1; i <=((movie_choice-1) * 9); i++) { // Each movie entry is 8 lines (including empty lines)
        fgets(line, sizeof(line), file);
    }
    
    fgets(line, sizeof(line), file); // Skip movie name line 
    fgets(line, sizeof(line), file); // Skip movie time line
    fgets(line, sizeof(line), file); // Skip Available Seats header or could have done +3 above

    // Display current available seats for the chosen movie
    int seats[5][3]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    printf("Current available seats for the chosen movie:\n");
    for (i = 0; i < 5; i++) {
        fscanf(file, "%d %d %d", &seats[i][0], &seats[i][1], &seats[i][2]); 
        printf("%d %d %d\n", seats[i][0], seats[i][1], seats[i][2]);
    }
    
    // Ask user to book seats
    int row, col,e=0;
    char n;
    edit1:
    e++;
    ef:
    printf("Enter row and column numbers (1-5, 1-3) you want to book: ");
    scanf("%d %d", &row, &col);
    if (seats[row - 1][col - 1]==1){
    	printf("that seat is already booked\n\n");
    	goto ef;
	}
	else{
		printf("\n \n seat booked successfully\n \n");
		
		// Update the seat in the array
    	seats[row - 1][col - 1] = 1;
    
	    //giving option to book more
		printf("if you want to book more please type Y or type anything else\n");
		n=getch();
		if(n=='y'||n=='Y'){
			goto edit1;
		}
	}
	
	system("cls");
    printf("\nthank you for booking with us\n");
    printf("\nPlease bring %d money when coming to the hall\n",e*150);
    
	// Create a temporary file
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file");
        fclose(file);
        return;
    }
    
    //update in file section
    // copying all line till edit section to temp file
	rewind(file);
    for (i = 1; i <=((movie_choice-1) * 9+3); i++) { 
        fgets(line, sizeof(line), file);
        fputs(line, temp_file);
        
    }
          
    int c=i+6;

    // Write updated seats to the temp file
    for (i = 0; i < 5; i++) {
        fprintf(temp_file, "%d %d %d\n", seats[i][0], seats[i][1], seats[i][2]);
    }
    
    //copying remaining line to the temp file
    for(c;c<=line_number;c++){
    	fgets(line, sizeof(line), file);
    	fputs(line, temp_file);
	}
	
	// Close both files
    fclose(file);
    fclose(temp_file);

    // Delete the original file
    remove(filename);

    // Rename the temporary file to the original filename
    rename("temp.txt", filename);
}

int login() {
	int choice;
    char us[20], pa[20],a[20], b[20];

    printf("\nMenu:\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    FILE *fp = fopen("login.txt", "r+");
            if (fp == NULL) {
                printf("Error opening file.\n");
                return 0;
            }

    switch (choice) {
        case 1:
            printf("Enter username: ");
            scanf("%s", us);
            printf("Enter password: ");
            scanf("%s", pa);
            fseek(fp,0,SEEK_END);
            fprintf(fp, "%s %s\n", us, pa);
            fclose(fp);
            system("cls");
            printf("\n\n User registered successfully. You can now book a ticket.\n\n");
            return 1; // Registration successful
        case 2:
            printf("Enter username: ");
            scanf("%s", us);
            printf("Enter password: ");
            scanf("%s", pa);
            system("cls");
            while (fscanf(fp, "%s %s", a, b) != EOF) {
                if (strcmp(a, us) == 0 && strcmp(b, pa) == 0) {
                    printf("Login successful.\n");
                    fclose(fp);
                    return 1; // Login successful
                }
            }
            fclose(fp);
            printf("Invalid username or password.\n");
            return 0; // Login failed
        case 3:
            printf("Exiting program.\n");
            return 0; // Exit program
        default:
            printf("Invalid choice.\n");
            return 0; // Invalid choice
	}
}
