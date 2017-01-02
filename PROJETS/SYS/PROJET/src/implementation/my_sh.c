/**
 * \file my_sh.c
 * \Basic command interpreter for the cnam advanced system course.
 * \author Baptiste T. <baptistetrautmann@hotmail.fr>
 * \version 0.1
 * \date 27 december 2016
 *
 * Basic command interpreter for the CNAM advanced system course.
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include<time.h>
#include<getopt.h>

//#include<my_sh.h> // TODO

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096
#define BUFFER_SIZE 4096

#define READ_END 0
#define WRITE_END 1

#define USAGE_SYNTAX "[OPTIONS]"
#define USAGE_PARAMS "OPTIONS: TODO\n"

#define HISTO_PATH "misc/Historique"

/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * \return void
 */
void print_usage(char* bin_name)
{
  dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}


/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * \param void* to_free pointer to an allocated mem
 * \see man 3 free
 * \return void
 */
void free_if_needed(void* to_free)
{
  if (to_free != NULL) free(to_free);  
}


/**
 *
 * \see man 3 strndup
 * \see man 3 perror
 * \return
 */
char* dup_optarg_str()
{
  char* str = NULL;

  if (optarg != NULL)
  {
    str = strndup(optarg, MAX_PATH_LENGTH);
    
    // Checking if ERRNO is set
    if (str == NULL) 
      perror(strerror(errno));
  }

  return str;
}

void adx_store_data(const char *filepath, const char *data)
{
  FILE *fp = fopen(filepath, "ab");
  if (fp != NULL)
  {
      fputs(data, fp);
      fclose(fp);
  }
}

int split_line(const char* input_line, char* split_char, char*** res){

  char ** return_tab;

  return_tab  = NULL;
  char *  p    = strtok (input_line, " ");
  int n_spaces = 0;


   while (p) {
    return_tab = realloc (return_tab, sizeof (char*) * ++n_spaces);

    if (return_tab == NULL)
      exit (-1); /* memory allocation failed */

    return_tab[n_spaces-1] = p;

    p = strtok (NULL, " ");
  }

  /* realloc one extra element for the last NULL */

  return_tab = realloc (return_tab, sizeof (char*) * (n_spaces+1));
  return_tab[n_spaces] = 0;

  *res = return_tab;

  return n_spaces;
}

int is_built_in(const char* command){
  /*if(strcmp("cd",command) != 0){

    return 0;
  } else if(strcmp("pwd", command) != 0){

    return 0;
  } else if(strcmp("exit", command) != 0){

    return 0;
  } else if(strcmp("echo", command) != 0){

    return 0;
  } else {
    return 1; // TODO
  }*/

  //TEMP
  return 1;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void simple_exec(const char* parameters){
  /*if(is_built_in(commande) == 0){
    printf("TODO");
  } else{*/
    char* command_path = "/bin/";
    char* command_final;
    char ** parameters_final;
    int i = 0, nb_arg;

    nb_arg = split_line(parameters, " ", &parameters_final); 

    //command_final = concat(command_path, parameters_final[0]);

    command_final = parameters_final[0];

    //parameters_final[0] = command_final;
    while(i<nb_arg){

      //parameters_final[i] = parameters_final[i+1];
      printf("Parameters_final[%d] = %s \n",i,parameters_final[i]);
      i++;
    }

    parameters_final[nb_arg] = NULL;
    printf("Parameters_final[%d] = %s \n",i,parameters_final[i]);  

    //printf("execl(%s, %s, %s, NULL)\n\n", command_final,command_final, parameters);
    execvp(command_final,parameters_final);
  //}
}


void exec_pipe(char* input_line){

    /*char * reste_split;
    nb_arg = split_line(input_line, " | ", &splitted_input);
    i = 2;

    while(i<nb_arg){
      strcat(reste_split, " ");
      strcat(reste_split,splitted_input[i]);
      
      i++;
    }

    dprintf(STDOUT,"Split[0] => %s ;;; reste => %s",splitted_input[0],reste_split);
    //exec_command(splitted_input[1]);
    //Brancher l'output de la partie 1 sur l'input de la partie 2 appelée en récursif 

    pipe(new_pipe_desc);

    int pid_fork = fork();


    //TO DEBUG
    if(pid_fork == 0){ // Fils

      if(pipe_read_end != NULL){
        dup2(new_pipe_desc[WRITE_END],pipe_read_end[READ_END]); // On redirige la sortie de l'ancien pipe vers l'écriture du nouveau pipe
        close(pipe_read_end[WRITE_END]);
        close(0);
      } else {
        dup2(new_pipe_desc[WRITE_END], 1); // On redirige la sortie standard vers l'écriture du pipe       
      }
      close(new_pipe_desc[READ_END]); // On ferme la sortie du pipe

      exec_command(splitted_input[0],NULL); // ps

    } else { //Père

      //wait(&status);

      dup2(pipe_desc[READ_END],0); // On redirige la sortie du pipe vers l'entrée   

      if(reste_split == NULL){ // Si il n'y a plus de redirection
        
      }
      // Rediriger la sortie dans le pipe, mais s'arreter lors de la dernière ligne

      //Il faut ouvrir le nouveau pipe 

     
      close(pipe_desc[WRITE_END]); // On ferme l'entrée du pipe


      printf("Statut du fils %d\n", status);

      execvp(cmd2,args2); //grep
    }

    if(pipe_read_end != NULL){

    } else {

    }

    //APPEL RECURSIF ICI AVEC FORK !!!
    // POUVOIR PASSER UN DESCRIPTEUR DE FICHIER A LA FONCTION
    exec_command(splitted_input[0]);


    i=0;*/

  printf("Ici éxecuter | \n");

  return EXIT_SUCCESS;
}

void exec_redirect(char * input_line){

  printf("exec_redirect\n");

  return EXIT_SUCCESS;
}

void exec_background(char * input_line){
  printf("Exec background\n");

  remove_substring_post_char('&', &input_line);
  
  int pid_fork = fork();

  if(pid_fork == 0){ // Fils

    exec_command(input_line); // ps

  }

  return EXIT_SUCCESS;
}

void remove_substring_post_char(const char c, char ** string_to_sub){

  char * res = *string_to_sub;
  int i = 0;
  int last_char_position = NULL;

  while(res[i] != '\0'){
    if(res[i] == c){
      last_char_position = i;
    }
    i++;
  }

  i = last_char_position;

  res[i] = '\0';
  i++;
  while(res[i] != '\0' && last_char_position != NULL){
    res[i] = NULL;
  }

}

void exec_sequential(char * input_line){

  char ** result_semicolon_split;
  int nb_split;
  int i;

  nb_split = split_by_char(input_line, * result_semicolon_split); // ( ps; ls -l; cat test) => [0]["ps"] [1]["ls -l"] [2]["cat test"]

  for(i = 0; i<nb_split; i++){

    //Lancer l'éxécution de chaque bloc
  }

  return EXIT_SUCCESS;
}

void exec_command(char * input_line)
{

  //char** splitted_input;
  //char* intact_input = malloc(strlen(input_line)+1);
  //char* split_char = " ";
  int nb_arg,i = 0;

  //strcpy(intact_input,input_line);

  /*printf("res strstr(input_line)/& = %d\n", strstr(input_line, " &"));
  printf("res strstr(input_line)/> = %d\n", strstr(" > ", input_line));
  printf("res strstr(input_line)/| = %d\n", strstr(" | ", input_line));*/

  //1 -> Découper l'exec par le caractère ';'
  //2 -> Placer les executions en tache de fond avec le car '&'
  //3 -> Gérer les blocks de rediréctions (< << > >>)
  //4 -> gérer les opérateurs logiques (&& ||)
  //5 -> Gérer le piping

  
    if(strstr(input_line, ";") != 0){
      exec_sequential(input_line);
    }/* else {

      if(strstr(input_line, " &") != 0){
        exec_background(input_line);      
      } else {

        if(strstr(input_line, "< << > >>") != 0){
          exec_redirect(input_line);
        } else {
          
          if(strstr(input_line, "&& ||") != 0){
            exec_logic_operation(input_line);
          } else {
            
            if(strstr(input_line, "|") != 0){
              exec_piping(input_line):
            } else {

              exec_line(input_line);
            }
          }
        }
      }
    }*/

  
}

char * read_and_histo(){

  char * res = malloc(sizeof(char) * 200);
  time_t timer;
  char buffer[26];
  struct tm* tm_info;

  // Lecture de l'input user
  fgets(res, 200, stdin);

  //Création d'un timestamp
  time(&timer);
  tm_info = localtime(&timer);
  strftime(buffer, 28, "%d/%m/%Y %H:%M:%S : ", tm_info);

  // Historisation
  adx_store_data(HISTO_PATH, buffer);
  adx_store_data(HISTO_PATH, res);

  return res;
}


/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv) //Interpréter directement argv pour les appels récursifs ?
{

  char* input_line;  

  do{
    dprintf(STDOUT,"my_sh> ");
    
    input_line = read_and_histo();

    //exec_command(input_line);

  } while(true); //strcmp(input_line,"EXIT\n") != 0); // Remplacer par un cas dans le switch

  return EXIT_SUCCESS;
}