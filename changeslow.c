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
int* changeslow(int* V, int V_size, int A) {
	int i, j;
	int newA;//A with value in V subtracted
	int* C = malloc(V_size*sizeof(int));//number of coins for each coin value in V
	int* newC;//values calculated using newA + 1 coin
	int* minC;//the C with the fewest number of coins
	
	for(i = 0; i < V_size; i++) {
		if(V[i] == A) {//if A is one of the values in V, return C with that as the only coin
			//setting all values in C to 0, excluding C[i]
			for(j = 0; j < V_size; j++) {
				C[j] = 0;
			}
			C[i] = 1;
			return C;
		}
	}
	
	for(i = 0; i < V_size; i++) {
		if(V[i] > A) {//if the coin is larger than the amount we're checking, return minC
			return minC;
		}

		newA = A - V[i];
		newC = changeslow(V, V_size, newA);//getting change for sub amount new A
		newC[i] += 1;//adding coin to C
		
		if(i == 0) {//if this is the first newC found, it's automatically minC
			minC = newC;
		}
		else {//comparing previous best C to new one
			if(sumarray(newC, V_size) < sumarray(minC, V_size)) {//if the new C has fewer coins than minC, it becomes minC
				minC = newC;
			}
		}
		
	}
	return minC;
	
	/*
	  for i from 0 to V.size
                    if V[i] == A 
                              Coins[V.size] = 0s
                              Coins[i] = 1
                              return Coins

	  for i from 0 to V.size
				newA = A - V[i]
				newCoins = changeslow(V, newA)
				newCoins[i] += 1
				if i == 0
					 minCoins = newCoins
				else 
					 minCoins = fewestcoins(minCoins, newCoins)
	  return minCoins
	*/
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
	fprintf(f_out, "Changeslow:\n");

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

		
		Ctofile(f_out, changeslow(V, v_count, A), v_count);

	    token = strtok_r(NULL, "\n\r", &token_ptr);
	}
	fclose(f_out);	
	
	end = clock();//ending timer
	run = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Running Time: %.2f\n", run);
	
	return 0;
}