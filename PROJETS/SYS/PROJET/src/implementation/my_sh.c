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

#define EXIT_SUCCESS 0
#define EXIT_ERROR 1

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

int exec_command(const char* parameters){
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
    //execvp(command_final,parameters_final);
  //}

    return EXIT_SUCCESS;
}

/*
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

/*  printf("Ici éxecuter | \n");

  return EXIT_SUCCESS;
}*/

char * substring_from_pos(char * input_line, int pos_start, int pos_end){

  char * res = malloc(sizeof(char) * ((pos_end-pos_start)+1));
  int i = pos_start;
  int char_counter = 0;

  if(pos_start >= strlen(input_line)){
    return EXIT_ERROR;
  }

  while(input_line[i] != '\0' && i<pos_end){
    res[char_counter] = input_line[i];
    i++;
    char_counter++;
  }

  res[char_counter] = '\0';

  return res;

}

void substring_from_pointer_to_pointer(const char * input_line,const char * pointer_start,const char * pointer_end, char ** res){

  char * substring = malloc(sizeof(char) * (pointer_end-pointer_start+1));


  dprintf(STDOUT,"POINTER_START = %d \n\n",pointer_start);
  char * i = pointer_start;
  int counter = 0;

  //dprintf(STDOUT,"substring building :\n");

  while((counter+i)<pointer_end){

    dprintf(STDOUT,"COUNTER = %d ;\n I = %d;\n\n",counter,i);

    substring[counter] = (counter+i);
    counter++;
  }

  substring[counter] = '\0';

  /*dprintf(STDOUT,"Substring finale = %s\n", substring);

  dprintf(STDOUT,"Substring pointer = %d\n", substring);*/

  *res = substring;

  /*dprintf(STDOUT,"res finale = %s\n", *res);

  dprintf(STDOUT,"res pointer = %d\n", *res);*/

}

int is_substring(const char * substring, const char * main_string){

  int i = 0;

  //dprintf(STDOUT,"\n\nis_substring\n\n");

  while(i<strlen(substring)){

    //dprintf(STDOUT,"%c", main_string[i]);

    if(substring[i] != main_string[i]){
      return 1;
    }
    i++;
  }

  //dprintf(STDOUT,"\n");
  return 0;
    
}

char * get_substring_adress_in_string(const char * substring, const char * input_string){ // Récupérer l'adresse de la première occurence de substring dans input_string

  char * res = NULL;

  char * i = input_string;

  while(is_substring(substring, i) == 1 && (i-input_string) < strlen(input_string)){
    i++;
  }

  if((i-input_string) < strlen(input_string)){
    res = i-1;
  }

  dprintf(STDOUT,"RES == %d\n",res);

  return res;

}

void remove_substring_post_string(const char* c, char ** string_to_sub){

  char * res = *string_to_sub;
  char * pointer = strstr(res, c) - res;
  int i = 0;

  pointer = '\0';
  pointer++;

  while(&pointer != '\0'){
    pointer = NULL;
    pointer++;
  }
  pointer = NULL;

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

int split_by_char(char * input_line, char c, char *** substring_tab){

  char ** res = NULL;
  char * substring;
  int i = 0,start_split = 0, end_split = 0, nb_split = 0;

  while(input_line[i] != '\0'){
    if(input_line[i] == c){
      end_split = i;

      substring = substring_from_pos(input_line, start_split, end_split);

      res = realloc (res, sizeof (char*) * (nb_split+1));

      res[nb_split] = substring;
      nb_split++;
      start_split = i + 1;
    }
    i++;
  }

  res = realloc (res, sizeof (char*) * (nb_split+1));

  substring = substring_from_pos(input_line, start_split, strlen(input_line)-1);

  res[nb_split] = substring;

  *substring_tab = res;

  return nb_split;
}

void exec_semicolon(char * input_line){

  char ** result_semicolon_split;
  int nb_split;
  int i;

  nb_split = split_by_char(input_line, ';', &result_semicolon_split); // ( ps; ls -l; cat test) => [0]["ps"] [1]["ls -l"] [2]["cat test"]

  dprintf(STDOUT,"EXEC semicolon ! nb_split = %d\n",nb_split);

  for(i = 0; i<=nb_split; i++){

    dprintf(STDOUT,"main_exec(%s)\n",result_semicolon_split[i]);
    main_exec(result_semicolon_split[i]);
  }


  return EXIT_SUCCESS;
}


int exec_background(char * input_line){
  dprintf(STDOUT,"Exec background!\n");

  remove_substring_post_char('&', &input_line);
  
  int pid_fork = fork();

  if(pid_fork == 0){ // Fils

    main_exec(input_line); // ps
    exit(0);
  }
  return EXIT_SUCCESS;
}

void exec_logical_operator(char * input_line){

  // Récupérer la première occurence de && ou || et lancer l'exec de la chaine de caractères avant
  char * pointer_and;
  char * pointer_or;
  char * second_part, first_part;

  pointer_and = get_substring_adress_in_string("&&", input_line);
  pointer_or = get_substring_adress_in_string("||", input_line);

  if(pointer_and == NULL && pointer_or != NULL){

    substring_from_pointer_to_pointer(input_line, input_line, pointer_or, &first_part);
    second_part = pointer_or+2;

    dprintf(STDOUT,"EXEC : %s || %s\n\n",first_part,second_part);

    if(main_exec(first_part) == 1){
      main_exec(second_part);
    }

  } else if(pointer_and != NULL && pointer_or == NULL){

    dprintf(STDOUT,"pointer pointer_and : %d\n", pointer_and);
    substring_from_pointer_to_pointer(input_line, input_line, pointer_and, &first_part); // Modifie pointer_and
    second_part = pointer_and+2;

    dprintf(STDOUT,"pointer pointer_and : %d\n", pointer_and);
    /*dprintf(STDOUT,"pointer input_line : %d\n", input_line);

    dprintf(STDOUT,"second_part pointer : %d\n",second_part);
    dprintf(STDOUT,"Ŝecond_part : %s",second_part);

    dprintf(STDOUT,"First part pointer : %d\n",first_part);
    dprintf(STDOUT,"First part : %s",first_part);*/

    //dprintf(STDOUT,"First part : %s && %s",first_part,second_part);

    if(main_exec(first_part) == 0){
      main_exec(second_part);
    }

  } else if(pointer_and < pointer_or){

    substring_from_pointer_to_pointer(input_line, input_line, pointer_and, &first_part);
    second_part = pointer_and+2;

    if(main_exec(first_part) == 0){
      main_exec(second_part);
    }

  } else if(pointer_and > pointer_or){

    substring_from_pointer_to_pointer(input_line, input_line, pointer_or, &first_part);
    second_part = pointer_or+2;

    if(main_exec(first_part) == 1){
      main_exec(second_part);
    }

  }

  return EXIT_SUCCESS;
}

void exec_redirection(char * input_line){


  return EXIT_SUCCESS;
}

void exec_piping(char * input_line){

  return EXIT_SUCCESS;
}

int main_exec(char * input_line)
{

  if( strstr(input_line,";")){
    exec_semicolon(input_line); // Lance autant d'exec de que bloc
  } 
  
  else if( input_line[strlen(input_line)-1] == '&'){
    exec_background(input_line);// Lance l'exec dans un processus fils sans attente
  }
  
  else if( strstr(input_line, "&&") || strstr(input_line, "||")){
    exec_logical_operator(input_line); // Lance des exec pour chaque bloc logique et test leurs validités
  }
  
  else if( strstr(" >",input_line) || strstr(" >>",input_line) || strstr("< ",input_line) || strstr("<< ",input_line)){
    exec_redirection(input_line); // Lance une chaine d'exec avec des redirections d'output/input
  }
  
  else if( strstr(" | ",input_line)){
    exec_piping(input_line);  // Lance des execs chainés dans des pipes
  }
  
  else{
    exec_command(input_line); // Lance une éxécution simple de la commande passée (built-in ou bash)
  }
  
  return EXIT_SUCCESS;

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

    main_exec(input_line);

  } while(1); //strcmp(input_line,"EXIT\n") != 0); // Remplacer par un cas dans le switch

  return EXIT_SUCCESS;
}