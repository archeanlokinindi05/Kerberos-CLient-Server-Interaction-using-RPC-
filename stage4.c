#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/vnode.h>
#include <sys/fs/ufs_fs.h>
#include <sys/fs/ufs_inode.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <pwd.h>

char *selTable[]={"-name","-mtime","-user",NULL};
char selectedCmd[10] ;
char pasdArg[100] ;

void procArg(char *source, char *selection, char *arg, int argCount, char *argv[]);
bool cmpFileName(char *fileName) ;
bool cmpMTime(char *fileName) ;
bool cmpUserId(char *fileName) ;

main(int argc, char **argv)
{

	struct stat buf;
	unsigned long ftype;

//	if (argc != 2) {printf ("usage: UID filename \n"); exit(0);}

	procArg(argv[1], argv[2], argv[3], argc, argv);

	lstat(argv[1], &buf);

	ftype = buf.st_mode & 0xF000;

//	printf("ftype=%x\n", ftype);
	
	if (ftype == S_IFDIR)
	{
//		printf ("\tDir \t %s \n", argv[1]);
		visitDir(argv[1]);
	}
	else printf ("reg file\n");
	
//	printf (" inode num=%d \n", buf.st_ino);

}


int visitDir(char *dirPath)
{

	DIR *mydirhandle;
	time_t tm;
	int fd ;
	struct dirent *mydirent;

	struct stat statinfo;
	unsigned long ftype;
	struct tm *t;
	char buf[200];
	char *fullDirAddr = NULL ;

//	printf("In visit dir \n");

	if((mydirhandle = opendir(dirPath)) == NULL)
	{
		perror(dirPath);
		return 0;
    	}

	while((mydirent = readdir(mydirhandle)) != NULL)
    	{ 
     		if((strcmp(mydirent->d_name, ".") == 0) || (strcmp(mydirent->d_name, "..") == 0))
      	{
        		continue;
      	}

//  		printf("dir name ---> %s\n", mydirent->d_name);

		fullDirAddr = malloc(strlen(dirPath)+strlen(mydirent->d_name)+2);

		strcpy(fullDirAddr, dirPath);
		strcat(fullDirAddr, "/");
		strcat(fullDirAddr, mydirent->d_name);

		lstat(fullDirAddr, &statinfo) ;

		ftype = statinfo.st_mode & 0xF000;

//		printf("ftype=%x\n", ftype);
	
		if (ftype == S_IFDIR)
		{
			if(cmpFileName(mydirent->d_name) || cmpMTime(fullDirAddr) || cmpUserId(fullDirAddr))
			{
				printf ("\tDIR \t %s \n", fullDirAddr);
			}
				visitDir(fullDirAddr);
				free(fullDirAddr);
		}
		else if(ftype == S_IFREG)
		{
			if(cmpFileName(mydirent->d_name) || cmpMTime(fullDirAddr) || cmpUserId(fullDirAddr))
			{
				printf ("\tReg \t %s \n", fullDirAddr);
			}
		}
		else
		{
			if(cmpFileName(mydirent->d_name) || cmpMTime(fullDirAddr) || cmpUserId(fullDirAddr))
			{
				printf ("-\tother \t %s \n", fullDirAddr);
			}
		}

//		if(lstat(fullDirAddr, &statinfo) != 0)
//		{

//			ftype = statinfo.st_mode & S_IFMT;

//			printf("ftype=%ld\n", ftype);
	
//			if (ftype == S_IFDIR)
//			{
//				printf ("\t DIR \t   %s\n", mydirent->d_name);
//				visitDir(fullDirAddr);
//				free(fullDirAddr) ;
//			}
//			else if(ftype == S_IFREG)
//			{
				
//				printf ("%tReg \t   %s", mydirent->d_name);
//				tm = statinfo.st_mtime; //time of last access

				/* convert time_t to broken-down time representation */
//				t = localtime(&tm);

				/* format time days.month.year hour:minute:seconds */
//				strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", t);
//
//				printf("Last modified time = %s\n", buf);

//			}
//			else
//			{
//				printf("\tother \t %s \n", mydirent->d_name); 
//			}

//		}
//
		free(fullDirAddr) ;

     	}        

	closedir(mydirhandle);
	return 0 ;
}

void procArg(char *source, char *selection, char *arg, int argCount, char *argv[])
{

	int i ;

	printf("Source=%s\tSelection=%s\tArgument=%s\n", source, selection, arg);

	for(i = 0 ; i < 3 ; i++)
	{
		if(strcmp(selection, selTable[i]) == 0)
		{
			strcpy(selectedCmd, selTable[i]) ;
			strcpy(pasdArg, arg) ;	

		}

	}	
}

bool cmpFileName(char *fileName)
{

	if((strcasecmp(fileName, pasdArg) == 0))
	{
		return true ;
	}
	else
	{
		return false ;
	}
}

bool cmpMTime(char *fileName)
{
	struct stat statinfo;
	time_t tm;
	time_t t;
	
	lstat(fileName, &statinfo) ;
	tm = statinfo.st_mtime ;
	t = time(&t) ;

	if((t - (atoi(pasdArg))*24*60*60) < tm)
	{
//		printf("MTime %ld \n", tm);
//		printf("CURRENT TIME %ld \n", t) ;	

//		printf("diff seconds %d \n ", (t - (atoi(pasdArg))*24*60*60)) ;

		return true ;
	}
	else
	{
		return false ;
	}		

}

bool cmpUserId(char *fileName)
{
	struct passwd *fileAtt;
	struct stat statinfo;
	
	lstat(fileName, &statinfo) ;

	fileAtt = getpwnam(pasdArg) ;

//	printf("File user ID of search item %d \t File user id of the given folder file %d \n ", (int)fileAtt->pw_uid, (int)statinfo.st_uid);
	
	if(fileAtt->pw_uid == statinfo.st_uid)
	{

		return true ;
	}
	else
	{
		return false ;
	}

}


