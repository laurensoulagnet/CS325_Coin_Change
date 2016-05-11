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
int* changedp(int* V, int V_size, int A, int* DP_table[V_size]) {
	int i, j;
	int newA;//A with value in V subtracted
	int* newC = malloc(V_size*sizeof(int));//values calculated using newA + 1 coin
	int* minC = malloc(V_size*sizeof(int));//the C with the fewest number of coins

	if(sumarray(DP_table[A], V_size) != 0) {//if DP_table has already been set for this A, return it
		return DP_table[A];
	}
	
	for(i = 0; i < V_size; i++) {
		if(V[i] == A) {//if A is one of the values in V, return minC with that as the only coin
			//setting all values in minC to 0, excluding minC[i]
			for(j = 0; j < V_size; j++) {
				minC[j] = 0;
			}
			
			minC[i] = 1;
			DP_table[A][i] = 1;//copying to DP table
			
			printf("Setting Base Table for %i \n", A);
			for(j = 0; j < V_size;j++) {
				printf("DP_table[%i][%i] = %i\n", A, j, DP_table[A][j]);
			}
			return minC;
		}
	}

	for(i = 0; i < V_size; i++) {
		if(V[i] > A) {//if the coin is larger than the amount we're checking, return minC
		
			memcpy(DP_table[A], minC, V_size*sizeof(int));//copying minC to DP_table
			printf("Setting Table for %i when V[i] > A\n", A);
			for(j = 0; j < V_size;j++) {
				printf("DP_table[%i][%i] = %i\n", A, j, DP_table[A][j]);
			}
			
			return minC;
		}

		newA = A - V[i];
		memcpy(newC, changedp(V, V_size, newA, DP_table), V_size*sizeof(int));//getting change for sub amount new A
		newC[i] += 1;//adding coin to C
		
		if(i == 0) {//if this is the first newC found, it's automatically minC
			memcpy(minC, newC, V_size*sizeof(int));
		}
		else {//comparing previous best C to new one
			if(sumarray(newC, V_size) < sumarray(minC, V_size)) {//if the new C has fewer coins than minC, it becomes minC
				memcpy(minC, newC, V_size*sizeof(int));
				
			}
		}	
	}
	
	memcpy(DP_table[A], minC, V_size*sizeof(int));
	printf("nSetting Table for %i \n", A);
	for(j = 0; j < V_size;j++) {
		printf("DP_table[%i][%i] = %i\n", A, j, DP_table[A][j]);
	}
	
	return DP_table[A];
}

//for each block of input in the argv[1] file, calls the change function for the values listed. Results are printed in filename+change.txt
int main(int argc, char* argv[]){
	
	FILE* f_in;//the file read
	FILE* f_out;//the file written to
	char* fn_in = argv[1];//the name of the file to be read
	char* fn_out;//the name of the file to be written to
	char* f_text;//the text from the file
	long f_size;//size of file

	int V[1000];//array of coin values, max size 1000
	int v_count = 0;//number of coins
	int A;//the amount that needs to be made into change
	char* token;//section of input file from strtok
	char* token2;//section of input file from strtok
	char* token_ptr;//used for strtok_r
	int** DP_table;//table to hold values for dpChange
	int i, j;
	clock_t begin, end;
	double run;//used to measure time
	
	//checking if user has provided filename
	if(argc != 2){
		printf("Error: incorrect number of arguments\n");
		printf("Usage: changedp [filename]\n\n");
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
	fprintf(f_out, "Changedp:\n");

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
		
		//malloc for DP table
		DP_table = (int**)malloc((1+A)*sizeof(int*));
		for(i = 0; i < A+1; i++) {
			DP_table[i] = (int*)malloc(v_count*sizeof(int));
			for(j = 0; j < v_count; j++) {
				DP_table[i][j] = 0;
			}
		}
		
		//printf("RESULT SUM: %i\n", sumarray(changedp(V, v_count, A, DP_table), v_count));//remove later LAUREN
		Ctofile(f_out, changedp(V, v_count, A, DP_table), v_count);
		
		//free DP_table
		for(i = 0; i < A+1; i++) {
			free(DP_table[i]);
		}
		free(DP_table);
		
	    token = strtok_r(NULL, "\n\r", &token_ptr);
	}
	fclose(f_out);	
	
	end = clock();//ending timer
	run = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Running Time: %.2f\n", run);
	
	return 0;
}
