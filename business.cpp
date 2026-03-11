//business side interface for movie ticket booking system

//declaration section
#include<stdio.h>
#include <stdlib.h>
#include<conio.h>
#include<string.h>


//for generating file name
struct date {
    int m, day;        
};

//for reading details of movie
struct movie {
    char name[20];
    struct time {
        int ho, min;
    } t1;
    int seat[5][3];
};

//function declaration
void add();
void edit();

int main(){
	
	char d=' ';
	printf("\n\n%49cMovie Ticket Booking System\n",d); 
	
	int ch;
	top:
	printf("\nwhat do you want to do? press respective number\n");
	printf("1. add movie \n2. edit movie seats \n3. press anything to exit");
	scanf("%d",&ch);
	switch(ch){
		case 1:
			add();
			break;
		case 2:
			edit();
			break;
		default:
		printf("invalid choice");
	}
	char n;
	printf("if you want to continue press Y or type anything else\n");
	n=getch();
	if(n=='y'||n=='Y'){
		goto top;
	}
	return 0;
}
void add() {
	system("cls");
    printf("Enter date (month and day): ");
    struct date d;
    scanf("%d %d", &d.m, &d.day);
    
    // Generate filename
    char filename[20];
    sprintf(filename, "%02d_%02d.txt", d.m, d.day);
    
    // Open file for writing
    FILE *file = fopen(filename, "w"); 
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    int num_movies;
    printf("Enter the number of movies you want to add: ");
    scanf("%d", &num_movies);
    
    for (int i = 0; i < num_movies; i++) {
        struct movie m;
        
        //input section for movie details
		printf("Enter movie name: ");
        scanf("%s", m.name);
        printf("Enter movie time (hour and minute): ");
        scanf("%d %d", &m.t1.ho, &m.t1.min);
        printf("Enter available seats (5x2 matrix) press 0 for available 1 for booked :\n");
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 3; k++) {
            	
				b:
            	printf("[%d][%d]",j+1,k+1);
                scanf("%d", &m.seat[j][k]);
                
                // checking garbage value
                if (!(m.seat[j][k]==0 || m.seat[j][k]==1)){  
		    	printf("invalid value");
    			goto b;
            	}	
            }
        }
        
        // Write movie details to file
        fprintf(file, "Name: %s\nTime: %02d:%02d\nAvailable Seats:\n", m.name, m.t1.ho, m.t1.min);
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 3; k++) {
                fprintf(file, "%d ", m.seat[j][k]);
            }
            fprintf(file, "\n");
        }
        fprintf(file, "\n");
    }
    system("cls"); 
    fclose(file); // Close file
    printf("\n \n Movies added successfully.\n\n");
}
void edit(){
	system("cls");
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
    
    // Ask which movie to edit
    int movie_choice;
    printf("Enter the movie number you want to edit: press 1 for first movie; 2 for second and so on");
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
    
    // Ask user to manipulate seats
    int row, col, value;
    char n;
    edit1:
    b:
    printf("Enter row and column numbers (1-5, 1-3) and value (0 for available, 1 for booked): ");
    scanf("%d %d %d", &row, &col, &value);
    if (!(value==0 || value==1)){
    	printf("invalid value");
    	goto b;
	}
    // Update the seat in the array
    seats[row - 1][col - 1] = value;
    
    //giving option to manipulate more
	printf("if you want to continue press Y or type anything else\n");
	n=getch();
	if(n=='y'||n=='Y'){
		goto edit1;
	}
	
	
	// Create a temporary file
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file");
        fclose(file);
        return;
    }
    
    
    // copying all line till edit section to temp file
	rewind(file);
    for (i = 1; i <=((movie_choice-1) * 9+3); i++) { 
        fgets(line, sizeof(line), file);
        fputs(line, temp_file);
        
    }
          
    int c=i+6;
    // Write updated seats to the temp file
    for (i = 0; i < 5; i++) {
    	fgets(line, sizeof(line), file);
        fprintf(temp_file, "%d %d %d\n", seats[i][0], seats[i][1], seats[i][2]);
        
    }
    
    //to move through blank line in original file
    fgets(line, sizeof(line), file);
    
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
    
    system("cls");
       
    
    printf("\n\nMovie seats edited successfully.\n \n");
}