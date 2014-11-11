// Structure représentant une commande //


typedef struct{
 pid_t pid;
 char* nom;
 char* arguments;
 int backquoted;
 int fd_in;
 int fd_out;
}
