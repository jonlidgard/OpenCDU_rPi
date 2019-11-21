#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <syslog.h>
#include <stdbool.h>

static const char *const evval[3] = {
"RELEASED",
"PRESSED",
"REPEATED"
};

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void handler(int sig)
{
    printf("\nExiting...(%d)\n", sig);
    exit (0);
}

void perror_exit (char *error)
{
    perror(error);
    handler(9);
}

void mylog(int level, const char *msg)
{
 	syslog(level, msg);
//	fprintf(stderr,"%s\n",msg);
}

int main(int argc, char *argv[])
{
        char device[] = "/dev/input/by-id/usb-Code_Mercenaries_TEKWorx_Limited_interfaceIT_CDU-event-kbd";
	char keytable[113][6];
	char ctrltable[91][6];
        memset(keytable,0,sizeof(keytable));
        memset(ctrltable,0,sizeof(ctrltable));


//	memset(keytable, (int)NULL, 64 * 6 * sizeof(const char *));
//	memset(ctrltable, (int)NULL, 45 * 5 * sizeof(const char *));

    strcpy(&keytable[2][0] , "1");
    strcpy(&keytable[3][0] , "2");
    strcpy(&keytable[4][0] , "3");
    strcpy(&keytable[5][0] , "4");
    strcpy(&keytable[6][0] , "5");
    strcpy(&keytable[7][0] , "6");
    strcpy(&keytable[8][0] , "7");
    strcpy(&keytable[9][0] , "8");
    strcpy(&keytable[10][0] , "9");
    strcpy(&keytable[11][0] , "0");

    strcpy(&keytable[13][0] , "-");
    strcpy(&keytable[14][0] , "CLR");

    strcpy(&keytable[16][0] , "Q");
    strcpy(&keytable[17][0] , "W");
    strcpy(&keytable[18][0] , "E");
    strcpy(&keytable[19][0] , "R");
    strcpy(&keytable[20][0] , "T");
    strcpy(&keytable[21][0] , "Y");
    strcpy(&keytable[22][0] , "U");
    strcpy(&keytable[23][0] , "I");
    strcpy(&keytable[24][0] , "O");
    strcpy(&keytable[25][0] , "P");

    strcpy(&keytable[28][0] , "EXEC");

    strcpy(&keytable[30][0] , "A");
    strcpy(&keytable[31][0] , "S");
    strcpy(&keytable[32][0] , "D");
    strcpy(&keytable[33][0] , "F");
    strcpy(&keytable[34][0] , "G");
    strcpy(&keytable[35][0] , "H");
    strcpy(&keytable[36][0] , "J");
    strcpy(&keytable[37][0] , "K");
    strcpy(&keytable[38][0] , "L");

    strcpy(&keytable[44][0] , "Z");
    strcpy(&keytable[45][0] , "X");
    strcpy(&keytable[46][0] , "C");
    strcpy(&keytable[47][0] , "V");
    strcpy(&keytable[48][0] , "B");
    strcpy(&keytable[49][0] , "N");
    strcpy(&keytable[50][0] , "M");

    strcpy(&keytable[52][0] , ".");
    strcpy(&keytable[53][0] , "/");
    strcpy(&keytable[57][0] , " ");

    strcpy(&keytable[59][0] , "L1");
    strcpy(&keytable[60][0] , "L2");
    strcpy(&keytable[61][0] , "L3");
    strcpy(&keytable[62][0] , "L4");
    strcpy(&keytable[63][0] , "L5");
    strcpy(&keytable[64][0] , "L6");

    strcpy(&keytable[65][0] , "R1");
    strcpy(&keytable[66][0] , "R2");
    strcpy(&keytable[67][0] , "R3");
    strcpy(&keytable[68][0] , "R4");
    strcpy(&keytable[87][0] , "R5");
    strcpy(&keytable[88][0] , "R6");

    strcpy(&keytable[104][0] , "PREV");
    strcpy(&keytable[109][0] , "NEXT");
    strcpy(&keytable[111][0] , "DEL");

    strcpy(&ctrltable[59][0] , "INIT");
    strcpy(&ctrltable[60][0] , "RTE");
    strcpy(&ctrltable[61][0] , "CLB");
    strcpy(&ctrltable[62][0] , "CRZ");
    strcpy(&ctrltable[63][0] , "DES");
    strcpy(&ctrltable[64][0] , "MENU");

    strcpy(&ctrltable[65][0] , "LEGS");
    strcpy(&ctrltable[66][0] , "DEP");
    strcpy(&ctrltable[67][0] , "HOLD");
    strcpy(&ctrltable[68][0] , "PROG");
    strcpy(&ctrltable[87][0] , "N1");
    strcpy(&ctrltable[88][0] , "FIX");


    struct input_event ev[64];
    int fd, rd, size = sizeof(struct input_event);
    char name[256] = "Unknown";
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    bool connected = false;

    char buffer[256];

    fprintf(stderr, "argc:%d\n",argc);
	char *keyptr;
    if (argc < 2) {
       fprintf(stderr,"usage %s hostname <port>\n", argv[0]);
       exit(0);
	}
	int i = 0;
	for (; i<argc; i++) {
	fprintf(stderr,"argv[%d]=%s\n",i,argv[i]);

	}

    if (argc == 3)
        portno = atoi(argv[2]);
    else
        portno = 8005;
 
    openlog("cducom", LOG_PID|LOG_CONS, LOG_USER);
        sprintf(buffer, "CDUCom starting: host %s, port %d", argv[1],portno);
 	mylog(LOG_INFO, buffer);
   
    if ((getuid()) != 0)
	 	mylog(LOG_WARNING, "You are not root! This may not work...n");
    if ((fd = open (device, O_RDONLY)) == -1) {    
        sprintf(buffer, "%s is not a valid device.\n", device);
	    mylog(LOG_ERR, buffer);
            exit(0);
	}

    ioctl (fd, EVIOCGNAME (sizeof (name)), name);
    sprintf (buffer, "Reading from : %s (%s)\n", device, name);
    mylog(LOG_INFO, buffer);


	while (true) {
	connected  = false;

	while (!connected) {
	    mylog(LOG_INFO, "Attempting to connect...");
	    sleep(5); // 5s
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    	    if (sockfd < 0) {
		mylog(LOG_ERR, "ERROR opening socket");
        	continue;
            } 
	   
	    mylog(LOG_INFO, "Opened socket...");

	    server = gethostbyname(argv[1]);
    	
            if (server == NULL) {
	        mylog(LOG_ERR, "ERROR, no such host");
                close(sockfd);
                continue;
    	    }
	 	
	    mylog(LOG_INFO, "Found host...");

	    bzero((char *) &serv_addr, sizeof(serv_addr));
    	    serv_addr.sin_family = AF_INET;
    	    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    	    serv_addr.sin_port = htons(portno);

       	    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
	 	mylog(LOG_ERR, "ERROR connecting, check firewall & AntiVirus!");
                close(sockfd);
        	continue;    	
            }

	    connected = true;
	}	

    
        mylog(LOG_INFO, "Connected..");
    
while (connected) {
        if ((rd = read (fd, ev, size*64)) < size)
            perror_exit("read()");

//        value = ev[0].value;
        int i = 0;
        
        if (ev[1].type == EV_KEY && ev[1].value == 1) {
        	if (ev[1].code == 29) { // ctrl
        		keyptr = &ctrltable[ev[3].code][0];
        	}
        	else {
        		keyptr = &keytable[ev[1].code][0];
			}
            sprintf (buffer, "%d - Code:%s - %s\n",i,keyptr,evval[ev[1].value]);
			mylog(LOG_INFO, buffer);
	        n = write(sockfd,keyptr,strlen(keyptr));
	        if (n < 0) {
			 	mylog(LOG_ERR, "ERROR sending!");
			    close(sockfd);
				connected = false;
			}
		}
    }
    }
    closelog();

    return 0;
}

