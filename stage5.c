#include <stdio.h>
#include <string.h>
#include <thread.h>


#define N 10


int r_num ;
int NW[N+2][N+2] ;
thread_t tid, thr[N+2] ;

int noOfEntreisQue[N];

mutex_t lock[N+2] ;

FILE *logFile;

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
void *peer(void *arg);

int main(int argc, char *argv[]){

	FILE *input_file ;	
	char readLine[50] = "";
	int noOfPeers = 0, i = 0, j = 0, k = 0,n = 0, noOfPeersPerLine = 0, count = 0, tmp_node = 0;
	int msg[10], TTL[10];
	char *arr;
	int status ;
	
	logFile = fopen("stage5_log", "w");	

	for(i = 0; i< N+2; i++){
		for(j = 0 ; j < N+2; j++) NW[i][j] = 0 ;
	}
	
	for(i = 0; i < N+1 ; i++) Q_head[i] = NULL ;
	
	for(i = 0; i < N+1 ; i++) Q_tail[i] = NULL ;

	for(i = 0; i < N+1 ; i++) noOfEntreisQue[i] = 0 ;

	for (n = 0 ; n < N ; n++) mutex_init(&lock[n], USYNC_THREAD, NULL);

	if(argc != 2){
		printf("Please pass the file name as argument \n");
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
			
		for(j = 0 ; j <= noOfPeersPerLine ; j++)
		{
			if(j == 0 )
			{
				arr = strtok(NULL, " ") ;
			}
			else
			{
				tmp_node = atoi(arr) ;	
				NW[i][tmp_node] = NW[tmp_node][i] = 1 ;
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

		for(j = 2 ; j <= r_num+1 ; j++)
		{
			thr_create(NULL, 0, peer, (void*)j, THR_BOUND, &thr[j]);
			sendMSG(1,2, 2000, 3);
			usleep(20000);
			while (thr_join(0, &tid, (void**)&status)==0){ }

		}
		
		for (i = 0 ; i <= r_num+1; i++)
			mutex_destroy(&lock[i]);	
}

void sendMSG(int senderId, int receiverId, int msg, int TTL){
	node_ptr newNode ;
	newNode = (node*)malloc(sizeof(node));
	newNode -> sender = senderId ;
	newNode -> msg = msg ;
	newNode -> TTL = TTL ;
	newNode -> next = NULL ;
	enterQ(newNode, receiverId);	

}

void enterQ(node_ptr newNode, int qId){

	mutex_lock(&lock[qId]);
	int noOfentry ;

	node_ptr y ;
	y = (node*)malloc(sizeof(node)) ;
	
	noOfentry = noOfEntreisQue[qId]  ;
	noOfentry++ ;
	noOfEntreisQue[qId] =  noOfentry;  

	if(Q_head[qId] == NULL){
		Q_tail[qId] = Q_head[qId] = newNode ;
	}
	else
	{
		Q_tail[qId]->next = newNode ;
		Q_tail[qId] = newNode ;
	}
	
	mutex_unlock(&lock[qId]);

}

node_ptr grabMSG(int x){

	node_ptr y ;
	int noOfEntry ;
	y = (node*)malloc(sizeof(node));
	mutex_lock(&lock[x]);

	noOfEntry = noOfEntreisQue[x] ;
	noOfEntry-- ;
	noOfEntreisQue[x] =  noOfEntry;

	if(Q_head[x] == NULL)
	{
		mutex_unlock(&lock[x]) ;
		return NULL ;
	}
	else
	{
		y = Q_head[x] ;

		if(Q_head[x] == Q_tail[x])
		{
			Q_head[x] = Q_tail[x] = NULL;

		} 
		else
		{
			Q_head[x] = Q_head[x] -> next ;
		}
		
		mutex_unlock(&lock[x]) ;

		return y ;
	}
	
}

void *peer(void *arg)
{
	int myId = (int)arg ;
	node_ptr y;
	
	int i, senderId, msgReceived, TTLReceived;

	int noOfEntries = 0 ;
	noOfEntries = noOfEntreisQue[myId] ;

	int myNeighbours[N+1];

	for(i = 0 ; i <= r_num+1 ; i++)
		myNeighbours[i] = NW[myId][i];

	for(i = 0 ;  i <= N+1 ; i++)
	{
		if(myNeighbours[i] == 1)
		{
//			printf("MyNeighbour Id %d \t", i) ;
		}

	}
//	printf("\n");
	usleep( 20000 );

	while(noOfEntreisQue[myId] > 0)
	{

		y = grabMSG(myId);
		
		if(y != NULL)
		{
			senderId = y->sender ;
			msgReceived = y->msg ;
			TTLReceived = y->TTL ;
	
			if(TTLReceived >= 1)
			{
				TTLReceived--;
				for(i = 0 ; i < N+1 ; i++)
				{
					if(myNeighbours[i] == 1 && senderId != i)
					{
				sendMSG(myId, i, msgReceived, TTLReceived);
//						printf("Came From %d MyID %d Sent to %d msg %d TTL %d \n", senderId, myId, i, msgReceived, TTLReceived) ; 

						fprintf(logFile, "%d %d %d %d %d \n", senderId, myId, i, msgReceived, TTLReceived) ; 
					}
				
				}
				
			}
		}

		free(y);
		usleep(2000);
	}
		return NULL;		
}
