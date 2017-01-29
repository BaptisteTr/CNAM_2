/**
 * \file my_sh.c
 * \Basic command interpreter for the cnam advanced system course.
 * \author Baptiste T. <baptistetrautmann@hotmail.fr>
 * \version 0.1
 * \date 29 january 2017
 *
 * Basic command interpreter for the CNAM advanced system course.
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include "util.h"
#include "my_sh.h"

#include<time.h>
#include<getopt.h>

 // TODO

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
 * \fn void print_usage (char * bin_name)
 *
 * \brief displays binary usage on STDOUT
 * \param bin_name binary's name to print
 *
 * \return void
 */
void print_usage(char* bin_name)
{
  dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}

/**
 * \fn int exec_semicolon (char * input_line)
 *
 * \brief split input by ";" and execute each part sequentially
 * \param input_line string to work on
 *
 * \return execution result : 0 = OK
 */
int exec_semicolon(char * input_line){

  char ** result_semicolon_split;
  int nb_split;
  int i, res;

  nb_split = split_by_char(input_line, ';', &result_semicolon_split); // ( ps; ls -l; cat test) => [0]["ps"] [1]["ls -l"] [2]["cat test"]

  for(i = 0; i<=nb_split; i++){

    main_exec(result_semicolon_split[i]);
  }


  return EXIT_SUCCESS;
}

/**
 * \fn int exec_background (char * input_line)
 *
 * \brief execute string input in a child process and continue
 * \param input_line string to work on
 *
 * \return execution result : 0 = OK
 */
int exec_background(char * input_line){

  remove_substring_post_char('&', &input_line);
  
  int pid_fork = fork();

  if(pid_fork == 0){ // Fils

    main_exec(input_line); // ps
    exit(0);
  }
  return EXIT_SUCCESS;
}

/**
 * \fn int exec_logical_operator (char * input_line)
 *
 * \brief split input by the chars "&&" and "||" and execute each bloc correspondingly to the logic depending of the previous bloc result
 * \param input_line string to work on
 *
 * \return execution result : 0 = OK
 */
int exec_logical_operator(char * input_line){

  char * pointer_and;
  char * pointer_or;
  char ** res;

  pointer_and = get_substring_adress_in_string("&&", input_line); // Renommer en is_string_present
  pointer_or = get_substring_adress_in_string("||", input_line);

  if(pointer_and == NULL && pointer_or != NULL){

    split_by_string_first_occurence(input_line,"||", &res);

    if(main_exec(res[0]) == 1){
      main_exec(res[1]);
    }

  } else if(pointer_and != NULL && pointer_or == NULL){

    split_by_string_first_occurence(input_line,"&&", &res);

    if(main_exec(res[0]) == 0){
      main_exec(res[1]);
    }

  } else if(pointer_and < pointer_or){

    split_by_string_first_occurence(input_line,"&&", &res);

    if(main_exec(res[0]) == 0){
      main_exec(res[1]);
    }

  } else if(pointer_and > pointer_or){

    split_by_string_first_occurence(input_line,"||", &res);

    if(main_exec(res[0]) == 1){
      main_exec(res[1]);
    }

  }

  return EXIT_SUCCESS;
}

/**
 * \fn int exec_redirection (char * input_line)
 *
 * \brief split input by the chars "<", "<<", ">" or ">>" and redirect output from a bloc into another
 * \param input_line string to work on
 *
 * \return execution result : 0 = OK
 */
void exec_redirection(char * input_line){

  char * pointer_left_can, pointer_right_can, pointer_double_left_can, pointer_double_right_can;
  char ** res;
  int out, is_double = 0;

  pointer_left_can = get_substring_adress_in_string("<", input_line); // Renommer en is_string_present
  pointer_right_can = get_substring_adress_in_string(">", input_line);

  if( pointer_left_can != NULL 
    && (pointer_right_can == NULL || pointer_left_can < pointer_right_can ))  
  {

    if(*(pointer_left_can+2) == '<'){
      split_by_string_first_occurence(input_line,"<<", &res);      
      is_double = 1;
    } else {
      split_by_char_first_occurence(input_line,'<', &res);
    }

    if( is_valid_path(res[0]) == 0 ) { // Améliorer (tester si il s'agit d'une commande ou d'un fichier)

      clean_spaces(res[0]);

      if(is_double == 1) {
        out = open(res[0], O_RDWR|O_CREAT|O_APPEND, 06755);
      } else {
        out = open(res[0], O_RDWR|O_CREAT|O_TRUNC, 06755);
      }
      if (-1 == out) { perror("Impossible de créer le fichier ou de l'ouvrir"); return 255;}

      int save_out = dup(fileno(stdout));

      if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }


      main_exec(res[1]);

      fflush(stdout); close(out);
      dup2(save_out, fileno(stdout));
      close(save_out);


    } else if (res[0][0] == '2') { // Tester entier


      dup2(2, 1);

      main_exec(res[1]);

      dup2(1, 1);
    } else { // Rediriger dans un pipe (fork un fils qu'on attend et qui prend le pere en input? )

      fork_pipes(2,res);

    }


    //Tester la partie de gauche (fichier, output ou commande)


  } else {

    if(*(get_substring_adress_in_string(">", input_line)+2) == '>'){
      split_by_string_first_occurence(input_line,">>", &res);      
      is_double = 1;
    } else {
      split_by_char(input_line,'>', &res);
    }

    if( is_valid_path(res[1]) == 0) {


      clean_spaces(&res[1]);


      if(is_double == 1) {
        out = open(res[1], O_RDWR|O_CREAT|O_APPEND, 06755);
      } else {
        out = open(res[1], O_RDWR|O_CREAT|O_TRUNC, 06755);
      }
      
      if (-1 == out) { perror("Impossible de créer le fichier ou de l'ouvrir"); return 255;}
      int save_out = dup(fileno(stdout));

      if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }


      main_exec(res[0]);

      fflush(stdout); close(out);
      dup2(save_out, fileno(stdout));
      close(save_out);


        // file exists
    } else if (res[1][0] == '2') {
      dup2(2, 1);

      main_exec(res[0]);

      dup2(1, 1);
    } else {

      fork_pipes(2,res);
    }

  }

  return EXIT_SUCCESS;
}

/**
 * \fn int exec_piping (char * input_line)
 *
 * \brief split input by the char "|", execute each bloc sequentially redirecting output in the next iteration input
 * \param input_line string to work on
 *
 * \return execution result : 0 = OK
 */
void exec_piping(char * input_line){


  char ** result_pipe_split;
  int nb_split;

  nb_split = split_by_char(input_line, '|', &result_pipe_split); // ( ps | grep sh | grep ??) => [0]["ps"] [1]["grep sh"] [2]["grep ??"]

  fork_pipes(nb_split+1, result_pipe_split);

  return EXIT_SUCCESS;
}


/**
 * \fn int exec_command (const char * parameters)
 *
 * \brief split the input by spaces and execute execvp or a built_in command
 * \param parameters string to work on
 *
 * \return execution result : 0 = OK
 */
int exec_command(const char* parameters){
  /*if(is_built_in(commande) == 0){
    printf("TODO");
  } else{*/
  char* command_path = "/bin/";
  char* command_final;
  char ** parameters_final;
  int i = 0, nb_arg, res;

  nb_arg = split_line(parameters, " ", &parameters_final); 

  command_final = parameters_final[0];

  parameters_final[0] = command_final;
  /*while(i<nb_arg){

    printf("parameter : %s\n",parameters_final[i+1]);

    parameters_final[i] = parameters_final[i+1];
    i++;
  }*/

  //parameters_final[nb_arg] = NULL;


  pid_t pid = fork();

  if (pid == 0) {
      
    res = execvp (parameters_final[0], parameters_final);

    if(res == -1){
      printf("%s: commande introuvable\n", command_final);
    }
  }
  else{ 
    int status = -1;

    wait(&status); 
  }

    return res;
}


/**
 * \fn int main_exec (char * input_line)
 *
 * \brief main execution logic, implements the character hierarchy on how to interpret the input
 * \param input_line string to work on
 *
 * \return execution result : 0 = OK
 */
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
  
  else if( strstr(input_line," >") || strstr(input_line," >>") || strstr(input_line, "< ") || strstr(input_line, "<< ")){
    exec_redirection(input_line); // Lance une chaine d'exec avec des redirections d'output/input
  }
  
  else if( strstr(input_line, " | ")){
    exec_piping(input_line);  // Lance des execs chainés dans des pipes
  }
  
  else{

    exec_command(input_line); // Lance une éxécution simple de la commande passée (built-in ou bash)
  }
  
  return EXIT_SUCCESS;

}

/**
 * \fn char * read_and_histo ()
 *
 * \brief read user input, save it into a file with a timestamp and return the read value
 * \param 
 *
 * \return user input string
 */
char * read_and_histo(){

  char * res = malloc(sizeof(char) * 200);
  time_t timer;
  int res_length, i;
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

  for(i = 0; i<strlen(res); i++){

    if(res[i] == '\n'){
      res[i] = ' ';
    }

  }

  return res;
}

/**
 * \fn int main(int argc, char ** argv)
 *
 * \brief entry point, implement the main loop, launch user input and execution
 * \param argc not used here
 * \param argc not used here
 *
 * \return EXIT_SUCCESS
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