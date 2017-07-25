#include <stdio.h>
#include <string.h>
#include <thread.h>


#define N 10


int r_num ;
thread_t tid, thr[N+2] ;
int NW[N+2][N+2] ;

mutex_t lock[N+2] ;

int main(int argc, char *argv[]){

	FILE *F ;	
	char s[50] = "";
	int noOfPeers = 0, i = 0, j = 0, k = 0, peercount = 0, count = 0, tmp_node = 0;
	int msg[10], TTL[10];
	char *arr;
	
	for(i = 0; i< N+2; i++){
		for(j = 0 ; j < N+2; j++) NW[i][j] = 0 ;
	}
	
	if(argc != 2){
		printf("%s is the file name \n", argv[0]);
	}
	
	F = fopen(argv[1], "r") ;	

	if(F == 0 ){
		printf("Could not open file \n"); 
	}

	fgets(s, 50, F);

	sscanf(s, "%d", &r_num);

	printf("No Of Peers %d \n", r_num);

	for(i = 1 ; i <= r_num ; i++)
		{
			fgets(s, 50, F) ;
			arr = strtok(s, " ");
			peercount = atoi(arr) ;
			
			printf("No Of peers at line %d is %d \n", i+1, peercount) ;
			
			for(j = 0 ; j <= peercount ; j++){
				if(j == 0 ){
					arr = strtok(NULL, " ") ;
				}
				else{
					tmp_node = atoi(arr) ;	
					NW[i][tmp_node] = NW[tmp_node][i] = i ;
					arr = strtok(NULL, " " ) ;	

				}	
				printf("\n");
			}

		}


		for(i = 0 ; i< N+2; i++){
			for(j = 0; j< N+2; j++)	printf(" %d   ", NW[i][j]);
			printf("\n");
		}

		while(1){

			if(feof(F)){
				break;
			}			

			fgets(s, 50, F) ;
			arr = strtok(NULL, " ") ;
			arr = strtok(s, " ");
			msg[k] = atoi(arr);

			printf("Message %d----> %d \t",k+1, msg[k]);
			arr = strtok(NULL, " ") ;
			TTL[k] = atoi(arr) ;
			printf(" TTL %d ---> %d \n", k+1, TTL[k]);
			k++ ;
		}
		k = 0 ;	
		

}
