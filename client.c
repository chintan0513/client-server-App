//Keta Jani - 110124165

//Chintan Tripathi - 110127406

//Section - 2 (Thursday 11:30 am)


#include <stdio.h>//Genrl Utility
#include <stdlib.h>//In and Out Ops
#include <string.h>//strng lib
#include <unistd.h>//Posix api
#include <sys/socket.h>//for sockt funcs
#include <sys/types.h>//sys related funcs
#include <fcntl.h>//File Cntrl ops
#include <netinet/in.h>//netwrrk related funcs
#include <arpa/inet.h>//netwrk addrss, ip func
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <errno.h>


#define PRMRY_SRVR_PRT 4444//constan var fr primary port
#define MRROR_SRVR_PRT 5555//constan var for server port

#define MAXI_COMMND_LENTH 1024//constan var for maxi lenth
#define MAXI_ARGUS 64//constan for maxi argus
#define BFFR_SZE 1024//constan for buffer size

char *PRMRY_SRVR_IPR = "127.0.0.1";//defin const strngs for primary iproto address
char *MRROR_SRVR_IPR = "127.0.0.1";//defin const strngs for mrror iproto address

char *commnds[MAXI_ARGUS];//to store commnd argus
int no_of_argus = 0;//an intege to track number

void splt_strng(char *strng) {//Funct to split strngs to indi argus
	no_of_argus = 0;////initil the argus count to 0
	char *tken = strtok(strng, " ");//to tokenise the input strng
	while (tken != NULL) {//chk nullify
        commnds[no_of_argus] = tken;//store the tken in arr
        tken = strtok(NULL, " ");//move to next tken
        no_of_argus++;//incree the argus count
    }
}

int chk_digi_strng(char* strng) {//Funct to chk strng has digis
    for (int i = 0; strng[i] != '\0'; i++) {//iteret each charact
        if (!isdigit(strng[i])) {//chk curr charact
            return 0;//retrn when nt found
        }
    }
    return 1;//retrn when found
}

int chk_vlid_dt(char* strng) {//Funcs to chk strng reps a valid date
    int dayy, monthh, yeear;//initi var
    if (sscanf(strng, "%d-%d-%d", &yeear, &monthh, &dayy) != 3) {//prse strngs
        return 0;//retrn with no reslt
    }
    if (dayy < 1 || monthh < 1 || monthh > 12 || yeear < 0) {//chk valid ranges
        return 0;//retrn wen no reslt
    }
    int maxi_dayys = 31;//initi maxi days in a month
    if (monthh == 4 || monthh == 6 || monthh == 9 || monthh == 11) {//determ maxi
        maxi_dayys = 30;//set maxi days to 30 for some months
    } else if (monthh == 2) {//chk leap year
        if (yeear % 4 == 0 && (yeear % 100 != 0 || yeear % 400 == 0)) {//chkin conditn
            maxi_dayys = 29;//leap yer
        } else {
            maxi_dayys = 28;//nt leap yer
        }
    }
    if (dayy > maxi_dayys) {//chk if given day is within maxi range
        return 0;//retrn if days exceds the maxi for month
    }
    return 1;//retrn if the date is valid accrding to chk
}

int vlidat(){//funct to validatee usr commnds
	if (strcmp(commnds[0], "w24fz") == 0 ||//cmpar wit coditn
		strcmp(commnds[0], "w24fs") == 0 ||//cmpar wit coditn
		strcmp(commnds[0], "w24ft") == 0 ||//cmpar wit coditn
		strcmp(commnds[0], "w24fda") == 0 ||//cmpar wit 
		strcmp(commnds[0], "w24fdb") == 0 ||//cmpar wit coditn
		strcmp(commnds[0], "dirlist") == 0 ||//cmpar wit coditn
		strcmp(commnds[0], "quitc") == 0){//cmpar wit coditn
		if (strcmp(commnds[0], "w24fs") == 0){//cmpar wit coditn
			if (no_of_argus != 2){//chk argus lenth
				printf("\n<usage>: w24fs <filename>\n\n");//prnt mssg
				return 0;//retrn wit err
			}
			return 1;//retrn suces
		}else if (strcmp(commnds[0], "w24fz") == 0){//cmpar wit coditn
			if (no_of_argus < 3 || no_of_argus > 4){//chk argus lenth
				printf("\n<usage>: w24fz <size1> <size2>\n\n");//prnt mssg
				return 0;//retrn wit err
			}else if (chk_digi_strng(commnds[1]) == 0 || chk_digi_strng(commnds[2]) == 0){//chk digis
				printf("\n<usage>: w24fz <size1> <size2>\n");//prnt mssg
				printf("Size should be integer\n\n");//prnt mssg
				return 0;//retrn wit err
			}else if (atoi(commnds[1]) >= atoi(commnds[2])){//chk condtn
				printf("\n<usage>: w24fz <size1> <size2>\n");//prnt mssg
				printf("Size1 should be less than size2\n\n");//prnt mssg
				return 0;//retrn wit err
			}else if (no_of_argus==4){//chk extens
				printf("\n<usage>: w24fz <size1> <size2>\n\n");//prnt mssg
				return 0;//retrn wit err
			}
			return 1;//retrn suces
		}else if (strcmp(commnds[0], "w24fda") == 0 || strcmp(commnds[0], "w24fdb") == 0) {
    if (no_of_argus != 2) { // Check the number of arguments
        printf("\n<usage>: %s <date>\n\n", commnds[0]);
        return 0; // Return with error
    } else if (chk_vlid_dt(commnds[1]) == 0) { // Check date validity
        printf("\n<usage>: %s <date>\n", commnds[0]);
        printf("Enter a valid date\n\n");
        return 0; // Return with error
    }
    return 1; // Return success
}else if (strcmp(commnds[0], "dirlist") == 0) { // Handle dirlist command here
			if (no_of_argus != 2) { // Check if the number of arguments is correct
                printf("\n<usage>: dirlist <-a | -t>\n\n");
                return 0;
            }
            if (strcmp(commnds[1], "-a") != 0 && strcmp(commnds[1], "-t") != 0) { // Check if option is either -a or -t
                printf("\n<usage>: dirlist <-a | -t>\n\n");
                return 0;
            }
			return 1;//retrn suces
		}else if (strcmp(commnds[0], "w24ft") == 0){//cmpar wit coditn
			if (no_of_argus < 2 || no_of_argus > 4){//chk argus lenth
				printf("\n<usage>: w24ft <extension1> <extension2> <extension3>\n\n");//prnt mssg
				return 0;//retrn wit err
			}
			return 1;//retrn suces
		}
		return 1;//retrn suces
	}else{
		printf("Wrong  Command!!! \n");//prnt err msg
		return 0;//retrn wit err
	}
}

void fetch_tarr_fil(int clnt_scket,char *fil_namo){//Funct to tarr the fils
	char readd_bffrr[BFFR_SZE];//bffrr to read data frm scket
	off_t fil_sze;//ttl size of fil being received
	off_t ttl_byt_readd=0;//tll bytes read from scket
	off_t bffrr_sze;//bffrr size for each read opertin
	char readd_bffrr_fr_flagg[BFFR_SZE];//bffrr to read the data or file flag from the client
	 struct passwd *pw = getpwuid(getuid());
    const char *home_dir = pw->pw_dir;
     char w24project_path[1024];
    snprintf(w24project_path, sizeof(w24project_path), "%s/w24project", home_dir);
    
	mkdir(w24project_path, 0700);
	char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/%s", w24project_path, fil_namo);

	FILE *file = fopen(file_path, "wb"); // Open file for writing in binary mode
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

	printf("Please Wait here..!!!\n");//prnt the mssg
	recv(clnt_scket, &readd_bffrr_fr_flagg, sizeof(readd_bffrr_fr_flagg), 0);//receve data flag from client
	char recevd_flagg[10];//str the receved flag
	strncpy(recevd_flagg, readd_bffrr_fr_flagg, 10);//extrct the receved flag
	char fil_tipe_flagg[10] = "SENDFILE=1";//flags for handling diffrnt cases
	char dataa_flagg[10] = "SENDFILE=0";//flags for handling diffrnt cases
	if (strncmp(recevd_flagg, dataa_flagg, 10)==0){//chk receved flag data
		printf("Received the Data Flag!\n");//prnt mssg
		sleep(1);//sleeep fr 1 sec
		send(clnt_scket, "flagReceived", 12, 0);//send the receved acknowledgement
		recv(clnt_scket, &readd_bffrr, sizeof(readd_bffrr), 0);//receve data from client
		printf(">>%s\n",readd_bffrr);//prnt the recevved data
		return;//retrn the func
	}else if (strncmp(recevd_flagg, fil_tipe_flagg, 10)==0){//chk if receved flag indicats fil
		printf("Received the File Flag!\n");//prnt mssg
		sleep(1);//slep for a sec
		send(clnt_scket, "flagReceived", 12, 0);//snd acknowl to client
		int fil_desc = open(fil_namo, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);//opn the fil for ritin
		if (fil_desc == -1) {//chk err
			printf("Error openin...\n");//prnt err msg
			close(fil_desc);//cls the fil
		}else{//otherwise
			recv(clnt_scket, &bffrr_sze, sizeof(bffrr_sze), 0);//receve the size of incom data
			while (ttl_byt_readd<bffrr_sze){//continu reding data until the ttl bites read mtch
        ssize_t bytes_received = recv(clnt_scket, readd_bffrr, BFFR_SZE, 0);
				if (bytes_received == -1) {
            perror("Error in receiving");
            break;
        }
        size_t bytes_written = fwrite(readd_bffrr, 1, bytes_received, file); // Write data to file
         if (bytes_written < bytes_received) {
            perror("Error writing to file");
            break;
        }
				/*if (byytes_ritten < byttes_readd) {//chk ritin data incomplete
					perror("Error writin...\n");//err
					break;//ext loop
				}*/
        ttl_byt_readd += bytes_received;
			}
			fclose(file);//cls the fil descript
			
		}
	}
}

struct DirInfo {
    char name[256];
    time_t mod_time;
};

int compare_dir_info(const void *a, const void *b) { //func to compre dir info
    const struct DirInfo *dir_info_a = (const struct DirInfo *)a; // assign vrble
    const struct DirInfo *dir_info_b = (const struct DirInfo *)b; //asssgn vrble

    if (dir_info_a->mod_time < dir_info_b->mod_time) { //condtn check tinme
        return -1;
    } else if (dir_info_a->mod_time > dir_info_b->mod_time) { //cndtn to chck time
        return 1;
    } else {
        return 0; //rtn stmnt
    }
}

// fnctn to cmpore drcty names alphabetically
int compare_directory_names(const struct dirent **a, const struct dirent **b) {
    return strcmp((*a)->d_name, (*b)->d_name); //chck name
}

void list_directories(const char *option) {
    struct dirent **dir_list;
    int num_entries;

    num_entries = scandir(".", &dir_list, NULL, alphasort); // Get directory entries

    if (num_entries < 0) {
        perror("scandir");
        exit(EXIT_FAILURE);
    }

    if (strcmp(option, "-a") == 0) {
        qsort(dir_list, num_entries, sizeof(struct dirent *), (int (*)(const void *, const void *))compare_directory_names); // List all directories (including hidden ones) in alphabetical order
        for (int i = 0; i < num_entries; ++i) {
            if (dir_list[i]->d_type == DT_DIR) {
                printf("%s\n", dir_list[i]->d_name);
            }
            free(dir_list[i]);
        }
    } else if (strcmp(option, "-t") == 0) {
        struct DirInfo dir_info_array[256]; // List directories by creation time
        int num_dirs = 0;

        DIR *dir;
        struct dirent *entry;
        dir = opendir(".");
        if (dir != NULL) {
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_type == DT_DIR) {
                    strcpy(dir_info_array[num_dirs].name, entry->d_name);

                    // Get directory creation time
                    struct stat st;
                    char path[256];
                    sprintf(path, "./%s", entry->d_name);
                    stat(path, &st);
                    dir_info_array[num_dirs].mod_time = st.st_mtime;

                    num_dirs++;
                }
            }
            closedir(dir);
        }

        qsort(dir_info_array, num_dirs, sizeof(struct DirInfo), compare_dir_info); // Sort directory entries by creation time

        printf("Directories (oldest first):\n");
        for (int i = 0; i < num_dirs; i++) {
            printf("%s\n", dir_info_array[i].name);
        }
    } else {
        printf("Invalid option\n");
    }
}





int main(int argc, char* argv[]){//Strt
	int clnt_scket, rtrn_val;//declre scket descriptr
	struct sockaddr_in prmry_srvr_addrss;//declr structre fr prmary srvr
	struct sockaddr_in mrror_srvr_addrss;//declr strctre fr mrror srver
	char bffrr[MAXI_COMMND_LENTH];//declr buffr
	char strng[MAXI_COMMND_LENTH];//declr strng bffrr
	char reaad_bffrr[BFFR_SZE];//declr bffrr for reding frm sckets
	clnt_scket = socket(AF_INET, SOCK_STREAM, 0);//cret a scket
	if(clnt_scket < 0){//chk conditn
		printf("ERRORS in ConnectinG!!!.\n");//prnt mssg
		exit(1);//ext prgm
	}
	printf("socket is connected!!.\n");//prnt mssg
	memset(&prmry_srvr_addrss, '\0', sizeof(prmry_srvr_addrss));//initi mem for prmry srvr
	prmry_srvr_addrss.sin_family = AF_INET;//set the addrss famly
	prmry_srvr_addrss.sin_port = htons(PRMRY_SRVR_PRT);//set the porto no in netwrk bite order
	prmry_srvr_addrss.sin_addr.s_addr = inet_addr(PRMRY_SRVR_IPR);//set the ip addrss netwrk bite order
	rtrn_val = connect(clnt_scket, (struct sockaddr*)&prmry_srvr_addrss, sizeof(prmry_srvr_addrss));//cnnt to prmry srver
	if(rtrn_val < 0){//chk condtn
		printf("Error in connectin 1st target\n");//prnt err
		exit(1);//ext prgm
	}
	write(clnt_scket, "c", 1);//wrte charac to scket
	int nombr = read(clnt_scket, reaad_bffrr, 1);//reado frm scket
    reaad_bffrr[nombr] = '\0';//null termi data bffrr
	if(strcmp(reaad_bffrr,"P") == 0){//chk the receved dat to eq
		printf("Connected to PRIMARY SERVER.\n");//prnt mssg
	}else if (strcmp(reaad_bffrr,"M")==0){//chk if receved dat is eq
		close(clnt_scket);//cls the curr scket connec
		clnt_scket = socket(AF_INET, SOCK_STREAM, 0);//creat a new scket
		if(clnt_scket < 0){//chk scket cretin
			printf("Error in connection.\n");//prnt mssg
			exit(1);//ext prgm
		}
		memset(&mrror_srvr_addrss, '\0', sizeof(mrror_srvr_addrss));//initi the mem of mrror servr addrss
		mrror_srvr_addrss.sin_family = AF_INET;//set addrss fmily
		mrror_srvr_addrss.sin_port = htons(MRROR_SRVR_PRT);//set the porto no in ntwrk bite ordr
		mrror_srvr_addrss.sin_addr.s_addr = inet_addr(MRROR_SRVR_IPR);//set the ip no in netwrk bite ordr
		rtrn_val = connect(clnt_scket, (struct sockaddr*)&mrror_srvr_addrss, sizeof(mrror_srvr_addrss));//cnnt to mrror srvr
		if(rtrn_val < 0){//chk connctn to mrror srvr failed
			printf("Error  in connectin\n");//prnt mssg
			exit(1);//ext prgm
		}
		printf("Connected to MIRROR World.\n");//prnt mssg
	}else{//othrwise
		printf("PRIMARY SERVER malfunction...!!!!\n");//prnt mssg
		exit(1);//ext prgm
	}
	while(1){//infi lop
		printf("clientw24$ ");//prnt the prmpt
		scanf(" %[^\n]s",&bffrr[0]);//red the usr inpt
		strcpy(strng, bffrr);//cpy usr inpt to anthr strng
		splt_strng(strng);//splt the inpt commnds
		int vlalida = vlidat();//vlidate the inpt commnd
		if(vlalida==1){//chk conditn
			send(clnt_scket, bffrr, strlen(bffrr), 0);//snd the inpt commnd to the srvr
		}else{//othrwise
			continue;//skp the lop
		}
		if(strcmp(bffrr, "quitc") == 0){//chk usr wants to quit
			close(clnt_scket);//cls the scket connec
			printf("Disconnected!!!\n");//prnt discnnt
			exit(1);//ext prgm
		} 
		if (strcmp(commnds[0],"dirlist") == 0) {
            if (no_of_argus != 2) {
                printf("\n<usage>: dirlist <-a | -t>\n\n");
            } else {
                list_directories(commnds[1]);
            }
        } else if(strcmp(commnds[0],"w24fz")==0){//chk coditn
			 fetch_tarr_fil(clnt_scket, "temp.tar.gz");//ftch fils based on commnd
		}else if (strcmp(commnds[0], "w24fda") == 0 || strcmp(commnds[0], "w24fdb") == 0) {//chk fr commnd
			 fetch_tarr_fil(clnt_scket, "temp.tar.gz");//ftch fils based on commnd
		}else if(strcmp(commnds[0],"w24ft")==0){//chk coditn
			 fetch_tarr_fil(clnt_scket, "temp.tar.gz");//ftch fils based on commnd
		}else{//othrwise
			if(recv(clnt_scket, reaad_bffrr, BFFR_SZE, 0) < 0){//receve frm servr
				printf("Oops!Error in receivingg!!!.\n");//prnt mssg
			}else{//otherwise
				printf("Server: %s\n", reaad_bffrr);//prnt mssg
			}
		}
	}
	return 0;//ext prgm
}//Fin



// list of commands

/*
	dirlist -a
	dirlist -t
	w24fn mrr.c / mrr.txt
	w24fz 1 1000
	w24ft c txt / pdf
	w24fdb 2024-04-15
	w24fda 2024-04-15
	quit
*/