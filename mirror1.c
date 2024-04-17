//Keta Jani - 110124165

//Chintan Tripathi  - 

//Section - 2 (Thursday 11:30 am)

#include <stdio.h>//Genrl Utility
#include <stdlib.h>//In and Out Ops
#include <string.h>//strng lib
#include <unistd.h>//Posix api
#include <sys/socket.h>//for sockt funcs
#include <sys/types.h>//sys related funcs
#include <time.h>//time relatd funcs
#include <dirent.h>//For Drectry related ops
#include <fcntl.h>//File Cntrl ops
#include <netinet/in.h>//netwrrk related funcs
#include <arpa/inet.h>//netwrk addrss, ip funcs
#include <sys/stat.h>//to chk fil statusses
#include <stdint.h>

#define PRT_MFA 5555//Porto No
#define MAXI_COMMND_LENTH 1024//Maxi lenth of commnd
#define MAXI_ARGUS 64 //Maxi Argus allowed
#define BUFFRR_SZE 1024//maxi lenth of buffrr siz

int prmry_srvr_client_cnt = 0, mrror_srvr_client_cnt = 0, ttl_clients = 0;//contr var
int nxt_client = 1;//var for servr seletn
//int ttl_clients = 0;//conter for ttl clients
char *hme;//str GloBal Var
char *commnds[MAXI_ARGUS];//Str Commnds frm Client
int no_of_argus = 0;//Ttl no of argumnts

void splt_strng(char *strng){//func to splt strngs on space
	no_of_argus = 0;//reset the argus cont
	char *tken = strtok(strng, " ");//tkenize the input
	while (tken != NULL){//chk if its null
		commnds[no_of_argus] = tken;//str the tken in commnds arry
		tken = strtok(NULL, " ");//get the nxt tken
		no_of_argus++;//incre the argus cont
	}
}

int render_srch_fil(int nv_scket, char *fil_namo, char *on_directn){//Func to recurr serch fil in drectry
	struct stat st;//str fil info
	DIR *drectry;//pintr to drectry strem
	struct dirent *entrri;//rep drectry entrii
	char fulll_drectin[MAXI_COMMND_LENTH];//var to str fulll patth
	if (stat(on_directn, &st) == -1){//chk if getttinn fil info
		return 1;//ret err
	}
	if (S_ISDIR(st.st_mode)){//chk if patth is drectry
		if ((drectry = opendir(on_directn)) == NULL){//opn drectry fr reding
			return 1;//ret err
		}
		while ((entrri = readdir(drectry)) != NULL){//itre thrugh drectry entrii
			if (strcmp(entrri->d_name, ".") == 0 || strcmp(entrri->d_name, "..") == 0){//skip . & .. entrii
				continue;//skipin
			}
			snprintf(fulll_drectin, MAXI_COMMND_LENTH, "%s/%s", on_directn, entrri->d_name);//creat fulll patth
			if (render_srch_fil(nv_scket, fil_namo, fulll_drectin) == 0){//recurr call the func
				return 0;//ret suces
			}
		}
		closedir(drectry);//cls the drectri strem
	}else{//othrwise
		if (strcmp(fil_namo, on_directn + strlen(on_directn) - strlen(fil_namo)) == 0){//chk fil namo mat
			if (stat(on_directn, &st) == -1){//gt fi info
				return 1;//ret err
			}
			printf("Found File %s\n", on_directn);//prnt suces msg
			printf("Checkin info.. Size: %d bytes\n", (intmax_t)st.st_size);//prnt sze
			printf("Chckin Invention Date created: %s", ctime(&st.st_ctime));//prnt dte
			// Extract file permissions
				char file_permissions[11]; // 9 permissions + 1 for null terminator
				memset(file_permissions, 0, sizeof(file_permissions)); // Initialize with null characters
				sprintf(file_permissions, "%c%c%c%c%c%c%c%c%c",
					(st.st_mode & S_IRUSR) ? 'r' : '-',
					(st.st_mode & S_IWUSR) ? 'w' : '-',
					(st.st_mode & S_IXUSR) ? 'x' : '-',
					(st.st_mode & S_IRGRP) ? 'r' : '-',
					(st.st_mode & S_IWGRP) ? 'w' : '-',
					(st.st_mode & S_IXGRP) ? 'x' : '-',
					(st.st_mode & S_IROTH) ? 'r' : '-',
					(st.st_mode & S_IWOTH) ? 'w' : '-',
					(st.st_mode & S_IXOTH) ? 'x' : '-');

				char *msssg = malloc(MAXI_COMMND_LENTH * sizeof(char));//allocate memory
				sprintf(msssg, "Info disclosure:\nFile: %s\nSize: %d bytes\nDate created: %s\nPermissions: %s",
					on_directn, (intmax_t)st.st_size, ctime(&st.st_ctime), file_permissions);//create message with file details and permissions
				send(nv_scket, msssg, strlen(msssg), 0);//send message through socket
				free(msssg); // Free allocated memory
				return 0;//return success
		}
	}
	return 1;//ret def err
}

void render_fil(int nv_scket, char *fil_oprtn){//Func for snding fils on opertns
	char commnd[MAXI_COMMND_LENTH];//str cmmnd to exec
	char fil_namo[500];//str the fil namo
	char fil_plural[500];//tmp strng
	int snd_fil = 0;//flg to chk wen fil is sent
	if (strcmp(fil_oprtn, "w24fz") == 0){//chk the condtn
		char *fnd_commnd = malloc(MAXI_COMMND_LENTH * sizeof(char));//alloc mem
		sprintf(fnd_commnd, "find %s -type f -name '*.*' -not -path '%s/Library/*' -size +%dc -size -%dc -print0 ", hme, hme, atoi(commnds[1]), atoi(commnds[2]));//constrct the commnd
		sprintf(commnd, "%s | if grep -q . ; then %s | tar -czf %s_%d.tar.gz --null -T -  ; fi", fnd_commnd, fnd_commnd, fil_oprtn, nv_scket);//fin commnd wich pipes reslts
	}else if (strcmp(fil_oprtn, "w24fda") == 0 || strcmp(fil_oprtn, "w24fdb") == 0) {
    char *fnd_commnd = malloc(MAXI_COMMND_LENTH * sizeof(char));

    // Check if the operation is for files newer than the specified date
    if (strcmp(fil_oprtn, "w24fda") == 0) {
        sprintf(fnd_commnd, "find %s -type f -name '*.*' -not -path '%s/Library/*' -newermt '%s 00:00:00' ! -newermt '%s 23:59:59' -print0", hme, hme, commnds[1], commnds[2]);
    }
    // Check if the operation is for files older than the specified date
    else if (strcmp(fil_oprtn, "w24fdb") == 0) {
        sprintf(fnd_commnd, "find %s -type f -name '*.*' -not -path '%s/Library/*' ! -newermt '%s 00:00:00' -print0", hme, hme, commnds[1]);
    }

    sprintf(commnd, "%s | if grep -q . ; then %s | tar -czf %s_%d.tar.gz --null -T - ; fi", fnd_commnd, fnd_commnd, fil_oprtn, nv_scket);
}else if (strcmp(fil_oprtn, "w24ft") == 0){//chk the condtn
		sprintf(commnd, "find %s -type f '('", hme);//constrct the commnd
		
			for (int i = 1; i < no_of_argus; i++){//itrr thruu commnd
				if (i != no_of_argus - 1){//chk the coditn
					sprintf(fil_plural, " -name '*.%s' -o", commnds[i]);//contrct each filter
				}else{//othrwise
					sprintf(fil_plural, " -name '*.%s'", commnds[i]);//constrct the last filter
				}
				strcat(commnd, fil_plural);//concatene the filter
			}
		
		sprintf(fil_plural, " ')' -print0 ");//appnd fin prt of commnd
		strcat(commnd, fil_plural);//concatene commnd
		sprintf(fil_plural, " | if grep -q . ; then %s |", commnd);//contrct the prt with pip
		strcat(commnd, fil_plural);//concatene commnd
		sprintf(fil_plural, "tar -czf %s_%d.tar.gz --null -T - ; fi", fil_oprtn, nv_scket);//contrct the fin prt
		strcat(commnd, fil_plural);//concatene commnd
	}
	printf("%s", commnd);//prnt the commnd
	int statuus = system(commnd);//exe the commnd
	if (statuus != 0){//chk the exit stats
		printf("\nOops! RuNNing Command Err...\n");//prnt err msg
		exit(0);//ret err
	}else{//othrwise
		printf("\nNice! Commmand Successful...\n");//prnt suces msg
	}
	sprintf(fil_namo, "%s_%d.tar.gz", fil_oprtn, nv_scket);//creat fil namo with socket
	if (access(fil_namo, F_OK) != -1){//chk if fil exists
		snd_fil = 1;//set the flg
	}else{//othrwise
		snd_fil = 0;//reset the send fil flag
	}
	if (snd_fil){//chk if sending fil is requird
		char rd_flg[10] = "SENDFILE=1";//flag to snd fil
		while (1){//infi loop
			printf("\n Sending flag: %s\n", rd_flg);//prnt snding mssg
			send(nv_scket, rd_flg, strlen(rd_flg), 0);//snd the fil
			char ackno[12];//acknowledg bffr
			printf("Waiting for acknowledgement...\n");//prnt mssg
			recv(nv_scket, ackno, 12, 0);//receve info
			printf("Acknowledgement received: %s\n", ackno);//prnt msg
			if (strncmp(ackno, "flagReceived", 12) == 0){//chk conditn
				break;//ext loop on acknowledg
			}
		}
		int fil_desc = open(fil_namo, O_RDONLY);//opn the fil for reding
		if (fil_desc == -1){//chk coditn
			printf("Error Some problem with opening!!!!\n");//prnt err msg
			exit(0);//ext with normal
		}
		struct stat st;//strct for string
		if (stat(fil_namo, &st) >= 0){//get fil info
			off_t bffrr_sizee = st.st_size;//gt the fil sze
			send(nv_scket, &bffrr_sizee, sizeof(bffrr_sizee), 0);//snd fil size
		}else{//othewise
			perror("stat");//err
			exit(EXIT_FAILURE);//ext with err
		}
		char buffrrr[BUFFRR_SZE];//bffrr fr reding fil data
		ssize_t byts_red;//no of bytes
		while ((byts_red = read(fil_desc, buffrrr, BUFFRR_SZE)) > 0){// byt read
			ssize_t byts_snt = send(nv_scket, buffrrr, byts_red, 0);//snd data
			if (byts_snt < byts_red){//chk condtn
				printf("Errorr in Sending...\n");//prnt err
				exit(0);//ext with norml
			}
		}
		remove(fil_namo);//rm the fil aftr snding
		close(fil_desc);//cls fil descrpt
		printf("SUCESS!!!...\n");//prnt suces
	}else{//othrwise
		char rd_flg[10] = "SENDFILE=0";//prpare flag
		while (1){//infi loop
			printf("\n Sending flag: %s\n", rd_flg);//prnt sending msg
			send(nv_scket, rd_flg, strlen(rd_flg), 0);//snd the fil
			char ackno[12];//acknowledge fil
			printf(".. Waiting fr acknowledgement...\n");//prnt msg
			recv(nv_scket, ackno, 12, 0);//receve fil
			printf("Acknowledgemnt received ...: %s\n", ackno);//prnt acknow msg
			if (strncmp(ackno, "flagReceived", 12) == 0){//chk condithn
				break;//ext the loop
			}
		}
		char *err = "ErrOR: smthing not Found";//err msg
		send(nv_scket, err, strlen(err), 0);//send err msg
		printf(" Message success...\n");//prnt mssg
	}
}



int srvr_selectn(){//Func to chk nxt client for load distribution
	if (ttl_clients <= 12){//chk the ttl no of clients
		if (ttl_clients % 6 == 0 && ttl_clients != 0){//chk ttl clients in count
			if (nxt_client == 0){//alterenate betwn nxt clint
				nxt_client = 1;//set nxxt client
			}else{//otherwise
				nxt_client = 0;//set nxxt client
			}
		}
	}else{//otherwise
		if (ttl_clients % 2 == 0){//chk ttl clients is even
			nxt_client = 1;//set nxxt client
		}else{//otherwise
			nxt_client = 0;//set nxxt client
		}
	}
	return nxt_client;//ret the selec client
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

void list_directories(const char *option) { //chck cdrctry
     struct dirent **dir_list; 
    int num_entries;

 
    num_entries = scandir(".", &dir_list, NULL, alphasort);    // Get drcty entrys

    if (num_entries < 0) { //if no entry
        perror("scandir"); //errr stmnt
        exit(EXIT_FAILURE);
    }

   
    if (strcmp(option, "-a") == 0) {  // hndle optns
       
        qsort(dir_list, num_entries, sizeof(struct dirent *), (int (*)(const void *, const void *))compare_directory_names);  // List all drctrs (including hidden ones) in alphabetical ordr
        for (int i = 0; i < num_entries; ++i) { //itrtethrgh
            if (dir_list[i]->d_type == DT_DIR) { //if cndn
                printf("%s\n", dir_list[i]->d_name); //prnt stmnt
            }
            free(dir_list[i]); // listdtrctyr
        }
    } else if (strcmp(option, "-t") == 0) { //check fr optn t
      
        struct DirInfo dir_info_array[256];   // lst drctyn by mdfcn time
        int num_dirs = 0; 

        DIR *dir;
        struct dirent *entry;
        dir = opendir("."); //opn drdcrty
        if (dir != NULL) {
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_type == DT_DIR) {
                    strcpy(dir_info_array[num_dirs].name, entry->d_name);

                    // Get drctry mdfcn time
                    struct stat st;
                    char path[256]; //chr defn
                    sprintf(path, "./%s", entry->d_name); //chcck fr path
                    stat(path, &st); //stat path
                    dir_info_array[num_dirs].mod_time = st.st_mtime; //drinfo arry

                    num_dirs++; //incrmnt
                }
            } 
            closedir(dir); //close dir
        }

       
        qsort(dir_info_array, num_dirs, sizeof(struct DirInfo), compare_dir_info);  // Sort drcrt entries by mdfctn time

        printf("Directories (oldest first):\n"); //prnt stmnt
        for (int i = 0; i < num_dirs; i++) { //itrte thrgh
            printf("%s\n", dir_info_array[i].name); //prnt stmnt
        }
    } else {
        printf("Invalid option\n"); //errr stmnts
    }
}

int main(){//Strt Prgm
    hme = getenv("HOME");//extrct glbal var
    int sckt_fil_dscptr, retrn;//fil_descrptr and str rturn val frm func
	int optn = 1;//fr scket optns
	struct sockaddr_in srvr_addrss;//str servr addrss
	int nv_scket;//str fil descrptr
	struct sockaddr_in nv_addrss;//str addrss frm clint
	socklen_t addrss_sze;//str siz of addrss
	char bffrr[1024];//bfr to str info
	pid_t chld_prc_id;//prc id of chld
	sckt_fil_dscptr = socket(AF_INET, SOCK_STREAM, 0);//creat scket with ipv4 connec and tcp
	if (sckt_fil_dscptr < 0){//chk if it wrks
		printf("Connction Error.\n");//prnt mssg with err
		exit(1);//destry prm
	}
	printf("Mirror Created\n");//prnt mssg with suces
	memset(&srvr_addrss, '\0', sizeof(srvr_addrss));//set memry to zros
	srvr_addrss.sin_family = AF_INET;//set addrss family
	srvr_addrss.sin_port = htons(PRT_MFA);//set prt no to servr
	srvr_addrss.sin_addr.s_addr = INADDR_ANY;//bnds servr to all netwrk intrface
	if (setsockopt(sckt_fil_dscptr, SOL_SOCKET, SO_REUSEADDR, &optn, sizeof(optn))){//set scket optns
        perror("Somethingss Failed");//prnt err
		exit(1);//detry prgm
	}
	retrn = bind(sckt_fil_dscptr, (struct sockaddr *)&srvr_addrss, sizeof(srvr_addrss));//to recev incmeing calls
	if (retrn < 0){//chk suces
		printf("ERRor Receveing.\n");//prnt err msg
		exit(1);//detry prgm
	}
	printf("Bind at %d\n", PRT_MFA);//prnt suces msg
	if (listen(sckt_fil_dscptr, 10) == 0){//pasive mode to wait for clint connecs
		printf("Listening here...\n");//prnt mssg
	}else{//unsuces condn
		printf("Error in Receveing.\n");//prnt err msg
	}
	while (1){//infi loop
        nv_scket = accept(sckt_fil_dscptr, (struct sockaddr *)&nv_addrss, &addrss_sze);//wayt for clint connec
		if (nv_scket < 0){//chk any errs
			exit(1);//detry prgm
		}
		printf("%d. Connection taken from %s:%d", ttl_clients + 1, inet_ntoa(nv_addrss.sin_addr), ntohs(nv_addrss.sin_port));//prnt clint ip and prt no
		ttl_clients++;
		pid_t chld_prc_id = fork();//forkin curr prc
		if (chld_prc_id == 0){//chk for parent
			close(sckt_fil_dscptr);//cls sockt fildescrpt
			while (1){//infi loop
				memset(bffrr, '\0', sizeof(bffrr));//mem for bffr
				recv(nv_scket, bffrr, BUFFRR_SZE, 0);//receve data frm clint servr
				if (strlen(bffrr) == 0){//chk if its nt null
					continue;//skp loop
				}
				printf("\nClient request: %s\n", bffrr);//prnt mssg
				if (strcmp(bffrr, "quitc") == 0){//chk if its quit
					printf("Disconected %s:%d\n", inet_ntoa(nv_addrss.sin_addr), ntohs(nv_addrss.sin_port));//prnt mssg for deconnt
					close(sckt_fil_dscptr);//cls sockt
					exit(0);//destry prgm
				}else{//only wen conditn fails
					splt_strng(bffrr);//splt strn
					char *commnd = malloc(strlen(commnds[0]) + 1); //str 1st commnd and +1 for the null termintr
					strcpy(commnd, commnds[0]);//cpy commnd
					commnd[strlen(commnd)] = '\0';//end with null charc
					char *mssgs = malloc(MAXI_COMMND_LENTH * sizeof(char));//str mssg
					if (strcmp(commnd, "w24fs") == 0){//chk commnd with conditn
						if (render_srch_fil(nv_scket, commnds[1], hme) != 0){//
							mssgs = "No trace of Files";//to str mssg with err
							send(nv_scket, mssgs, strlen(mssgs), 0);//snd mssg

						}
					}else if (strcmp(commnd, "w24fz") == 0){//chk fr commnd
						render_fil(nv_scket, "w24fz");//render func
					}else if (strcmp(commnds[0], "w24fda") == 0 || strcmp(commnds[0], "w24fdb") == 0) {//chk fr commnd
					  render_fil(nv_scket, commnd);
					}else if (strcmp(commnd, "w24ft") == 0){//chk fr commnd
						render_fil(nv_scket, "w24ft");//render func
					}else if (strcmp(commnd, "dirlist") == 0) {
                        list_directories(commnds[1]);
                    }
					memset(mssgs, 0, sizeof(char) * sizeof(mssgs));//str mssgs aftr exec
				}
			}
		}
	}
	close(nv_scket);//cls sockt
	return 0;//retrn val
}//Fin

