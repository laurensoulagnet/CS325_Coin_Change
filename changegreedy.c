#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//returns the sum of all values in C
int sumarray(int* C, int C_size) {
	int i;
	int sum = 0;//sum of values in C
	
	for(i = 0; i < C_size; i++) {
		sum += C[i];
	}
	return sum;
}

void Ctofile(FILE* f_out, int* C, int C_size) {
	int i;
	
	fprintf(f_out, "[");
	for(i = 0; i < C_size -1; i++) {//printing all but the last number in C
		fprintf(f_out, "%i, ", C[i]);
	}
	fprintf(f_out, "%i]\n", C[i]);//printing out the last number in C
	fprintf(f_out, "%i\n", sumarray(C, C_size));//printing A	
}

//takes an array of values V, and returns the number for each of these to use to make A in corresponding array C
int* changegreedy(int* V, int V_size, int A) {
	int max_pos;//the place of the largest coin in V smaller than A
	int newA;//A with value in V subtracted
	int* C = malloc(V_size*sizeof(int));//number of coins for each coin value in V
	int j;
	
	for(max_pos = 0; max_pos < V_size; max_pos++) {//getting max_pos
		if(A < V[max_pos]) {
			max_pos--;
			break;
		}
	}
	
	if(max_pos == V_size) {
		max_pos--;
	}
	
	if(A == V[max_pos]) {//if A matches a coin value in V, return that coin in C
		//setting all values in C to 0, excluding C[i]
		for(j = 0; j < V_size; j++) {
			C[j] = 0;
		}
		C[max_pos] = 1;
		return C;
	}
	else {//if A does not match a coin value in V, select the largest coin and get change for the rest
		newA = A - V[max_pos];
		C = changegreedy(V, V_size, newA);
		C[max_pos] += 1;
		
		return C;
	}
}

//for each block of input in the argv[1] file, calls the change function for the values listed. Results are printed in filename+change.txt
int main(int argc, char* argv[]){
	
	FILE* f_in;//the file read
	FILE* f_out;//the file written to
	char* fn_in = argv[1];//the name of the file to be read
	char* fn_out;//the name of the file to be written to
	char* f_text;//the text from the file
	long f_size;//size of file

	int V[1000];//array of coin values
	int v_count = 0;//number of coins
	int A;//the amount that needs to be made into change
	char* token;//section of input file from strtok
	char* token2;//section of input file from strtok
	char* token_ptr;//used for strtok_r
	clock_t begin, end;
	double run;//used to measure time
	
	//checking if user has provided filename
	if(argc != 2){
		printf("Error: incorrect number of arguments\n");
		printf("Usage: changeslow [filename]\n\n");
		exit(0);
	}
	
	//opening input file
	f_in = fopen(fn_in, "r");
	if(f_in == NULL) {
		printf("Error: cannot open input file\n");
	}
	
	//stripping .txt from input filename and adding change.txt, then open output file
	fn_out = argv[1];
	strtok(fn_out, ".");
	strcat(fn_out, "change.txt");
	f_out = fopen(fn_out, "w+");
	if(f_out == NULL) {
		printf("Error: cannot open output file\n");
	}
	
	//getting text from file
	fseek(f_in, 0, SEEK_END);
	f_size = ftell(f_in);
	rewind(f_in);

	f_text = malloc(f_size + 1);
	fread(f_text, f_size, 1, f_in);
	fclose(f_in);

	begin = clock();//starting timer
	fprintf(f_out, "Changegreedy:\n");

	token = strtok_r(f_text, "\n\r", &token_ptr);
	//run while there are blocks to be read
	while( token != NULL ) 
	{	
		//getting V
		v_count = 0;
		token2 = strtok(token, ", []\n\r");
		while(token2!= NULL) {//adding all values to V
			V[v_count] = atoi(token2);
			v_count++;
			token2 = strtok(NULL, ", []\n\r");
		}
		token = strtok_r(NULL, "\n\r", &token_ptr);//getting A
		A = atoi(token);
		
		//call function here
		//print function result here

		printf("RESULT SUM: %i\n", sumarray(changegreedy(V, v_count, A), v_count));//remove later LAUREN
		Ctofile(f_out, changegreedy(V, v_count, A), v_count);
		
	    token = strtok_r(NULL, "\n\r", &token_ptr);
	}
	fclose(f_out);	
	
	end = clock();//ending timer
	run = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Running Time: %.2f\n", run);
	
	return 0;
}