#include <stdio.h>
#include <string.h>
#include <thread.h>


#define N 10


int r_num ;
int NW[N+2][N+2] ;

mutex_t lock[N+2] ;

typedef struct MSG
{
	unsigned int sender;
	unsigned int TTL;
	unsigned int msg;
	struct MSG *next;
} node, *node_ptr;

node_ptr Q_head[N+1], Q_tail[N+1], tmpNode;

void sendMSG(int senderId, int receiverId, int msg, int TTL) ;
void enterQ(node_ptr newNode, int qId) ;
node_ptr grabMSG(int x) ;


int main(int argc, char *argv[]){

	FILE *input_file ;	
	char readLine[50] = "";
	int noOfPeers = 0, i = 0, j = 0, k = 0,n = 0, noOfPeersPerLine = 0, count = 0, tmp_node = 0;
	int msg[10], TTL[10];
	char *arr;
	
	for(i = 0; i< N+2; i++){
		for(j = 0 ; j < N+2; j++) NW[i][j] = 0 ;
	}
	
	for (n = 0 ; n < N ; n++) mutex_init(&lock[n], USYNC_THREAD, NULL);


	if(argc != 2){
		printf("%s is the file name \n", argv[0]);
	}
	
	input_file = fopen(argv[1], "r") ;	

	if(input_file == 0 ){
		printf("Could not open file \n"); 
	}

	fgets(readLine, 50, input_file);

	sscanf(readLine, "%d", &r_num);

	printf("No Of Peers %d \n", r_num);

	for(i = 1 ; i <= r_num ; i++)
		{
			fgets(readLine, 50, input_file) ;
			arr = strtok(readLine, " ");
			noOfPeersPerLine = atoi(arr) ;
			
			printf("No Of peers at line %d is %d \n", i+1, noOfPeersPerLine) ;
			
			for(j = 0 ; j <= noOfPeersPerLine ; j++){
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

			if(feof(input_file)){
				break;
			}			

			fgets(readLine, 50, input_file) ;
			arr = strtok(NULL, " ") ;
			arr = strtok(readLine, " ");
			msg[k] = atoi(arr);

			printf("Message %d----> %d \t",k+1, msg[k]);
			arr = strtok(NULL, " ") ;
			TTL[k] = atoi(arr) ;
			printf(" TTL %d ---> %d \n", k+1, TTL[k]);
			k++ ;
		}
		k = 0 ;	
		
		sendMSG(1, 3, 1000, 1); //sender = 1, receiver=2, msg = 1000, TTL=1
		sendMSG(1, 2, 2000, 2);
		sendMSG(1, 4, 2000, 4);	

		if((tmpNode = grabMSG(2)) != NULL)
		{
			printf("Message %d \t", tmpNode -> msg);
			printf("Source Id %d \t", tmpNode -> sender) ;
			printf("Receiver Id 2 \t");
			printf("TTL %d \n", tmpNode -> TTL);

		}else{

			printf("There is no message in Queue 2 \n") ;
		}

		if((tmpNode = grabMSG(3)) != NULL)
		{
			printf("Message %d \t", tmpNode -> msg);
			printf("Source Id %d \t", tmpNode -> sender) ;
			printf("Receiver Id 3 \t");
			printf("TTL %d \n", tmpNode -> TTL);

		}else{

			printf("There is no message in Queue 3 \n") ;
		}

		if((tmpNode = grabMSG(4)) != NULL)
		{
			printf("Message %d \t", tmpNode -> msg);
			printf("Source Id %d \t", tmpNode -> sender) ;
			printf("Receiver Id 4 \t");
			printf("TTL %d \n", tmpNode -> TTL);

		}else{

			printf("There is no message in Queue 4 \n") ;
		}

		
	
}

void sendMSG(int senderId, int receiverId, int msg, int TTL){
	node_ptr newNode ;
	newNode = (node*)malloc(sizeof(node));
	newNode -> sender = senderId ;
	newNode -> msg = msg ;
	newNode -> TTL = TTL ;
	enterQ(newNode, receiverId);	

}

void enterQ(node_ptr newNode, int qId){

	mutex_lock(&lock[qId]);
	if(Q_head[qId] == NULL){
		Q_tail[qId] = Q_head[qId] = newNode ;
	}else{
		Q_tail[qId] = newNode ;

	}
	mutex_unlock(&lock[qId]);

}

node_ptr grabMSG(int x){

	node_ptr y ;
	y = (node*)malloc(sizeof(node));
	mutex_lock(&lock[x]);
	if(Q_head[x] == NULL)
	{
		mutex_unlock(&lock[x]) ;
		return NULL ;
	}
	else
	{
		y -> msg = Q_head[x] -> msg ;
		y -> sender = Q_head[x] -> sender ;
		y -> TTL = Q_head[x] -> TTL ;

		if(Q_tail[x] == Q_head[x])
		{
			Q_head[x] = NULL ;
			Q_tail[x] = NULL ;
		}
		else
		{
			Q_head[x] = Q_head[x] -> next ;
		}
		
		
		mutex_unlock(&lock[x]) ;

		return y ;
	}
}
