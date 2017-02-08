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
#include "cd.c"

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
 * \fn int fork_pipes (int n, char ** input_line)
 *
 * \brief execute the list of input sequentially and pipe each output in the next input
 * \param n number of iteration
 * \param **input_line array of string to work on
 * \param saved_stdout output to dup
 * \param saved_stdin input to dup
 *
 * \return execution result : 0 = OK
 */
int fork_pipes (char ** input, int saved_stdout, int saved_stdin)
{

  int status1, status2, res;
  int fd[2]; 

  pipe(fd);

  if ( fork() == 0 ) {
    close(fd[0]); 
    dup2(fd[1], 1); 

    main_exec(input[0], saved_stdout, saved_stdin); 

    exit(0); 
  }

  if (fork() == 0 ){
    close(fd[1]);
    dup2(fd[0], 0);

    res = main_exec(input[1], saved_stdout, fd[0]);
    exit(0); 
  }

  close(fd[1]); 
  close(fd[0]);

  wait(&status1); 
  wait(&status2);

  return res;
}

/**
 * \fn int is_built_in (const char * command)
 *
 * \brief test if the command is built_in
 * \param command string to check
 *
 * \return 0 == TRUE, 1 == FALSE
 */
int exec_if_built_in(const char* command){

  if(strncmp("pwd", command,3) == 0){
    return pwd();
  } else if(strncmp("echo", command,4) == 0){
    return echo(command+5);
  } else {
    return 1;
  }
}

/**
 * \fn int exec_semicolon (char * input_line)
 *
 * \brief split input by ";" and execute each part sequentially
 * \param input_line string to work on
 * \param saved_stdout output to dup
 * \param saved_stdin input to dup
 *
 * \return execution result : 0 = OK
 */
int exec_semicolon(char * input_line, int saved_stdout, int saved_stdin){

  char ** result_semicolon_split;
  int nb_split;
  int i, res;

  nb_split = split_by_char(input_line, ';', &result_semicolon_split); // ( ps; ls -l; cat test) => [0]["ps"] [1]["ls -l"] [2]["cat test"]

  for(i = 0; i<=nb_split; i++){

    res = main_exec(result_semicolon_split[i], saved_stdout, saved_stdin);
  }


  return res;
}

/**
 * \fn int exec_background (char * input_line)
 *
 * \brief execute string input in a child process and continue
 * \param input_line string to work on
 * \param saved_stdout output to dup
 * \param saved_stdin input to dup
 *
 * \return execution result : 0 = OK
 */
int exec_background(char * input_line, int saved_stdout, int saved_stdin){

  remove_substring_post_char('&', &input_line);
  int res;
  
  int pid_fork = fork();

  if(pid_fork == 0){ // Fils

    res = main_exec(input_line, saved_stdout, saved_stdin); // ps
    exit(0);
  }
  return EXIT_SUCCESS;
}

/**
 * \fn int exec_logical_operator (char * input_line)
 *
 * \brief split input by the chars "&&" and "||" and execute each bloc correspondingly to the logic depending of the previous bloc result
 * \param input_line string to work on
 * \param saved_stdout output to dup
 * \param saved_stdin input to dup
 *
 * \return execution result : 0 = OK
 */
int exec_logical_operator(char * input_line, int saved_stdout, int saved_stdin){

  char * pointer_and, pointer_or;
  char ** res_split;
  int res;

  pointer_and = get_substring_adress_in_string("&&", input_line); 
  pointer_or = get_substring_adress_in_string("||", input_line);

  if(pointer_and == NULL && pointer_or != NULL){

    split_by_string_first_occurence(input_line,"||", &res_split);

    if(main_exec(res_split[0], saved_stdout, saved_stdin) != 0 ){
      res = main_exec(res_split[1], saved_stdout, saved_stdin);
    }

  } else if(pointer_and != NULL && pointer_or == NULL){

    split_by_string_first_occurence(input_line,"&&", &res_split);

    if(main_exec(res_split[0], saved_stdout, saved_stdin) == 0){
      res = main_exec(res_split[1], saved_stdout, saved_stdin);
    }

  } else if(pointer_and < pointer_or){

    split_by_string_first_occurence(input_line,"&&", &res_split);

    if(main_exec(res_split[0], saved_stdout, saved_stdin) == 0){
      res = main_exec(res_split[1], saved_stdout, saved_stdin);
    }

  } else if(pointer_and > pointer_or){

    split_by_string_first_occurence(input_line,"||", &res_split);

    if(main_exec(res_split[0], saved_stdout, saved_stdin) != 0){
      res = main_exec(res_split[1], saved_stdout, saved_stdin);
    }

  }

  return res;
}

/**
 * \fn int exec_redirection (char * input_line)
 *
 * \brief split input by the chars "<", "<<", ">" or ">>" and redirect output from a bloc into another
 * \param input_line string to work on
 * \param saved_stdout output to dup
 * \param saved_stdin input to dup
 *
 * \return execution result : 0 = OK
 */
int exec_redirection(char * input_line, int saved_stdout, int saved_stdin){

  char * pointer_left_can, pointer_right_can, pointer_double_left_can, pointer_double_right_can;
  char ** res_split;
  int res, out, is_double = 0;

  pointer_left_can = get_substring_adress_in_string("<", input_line); // Renommer en is_string_present
  pointer_right_can = get_substring_adress_in_string(">", input_line);

  if( pointer_left_can != NULL 
    && (pointer_right_can == NULL || pointer_left_can < pointer_right_can ))  
  {

    if(*(pointer_left_can+2) == '<'){
      split_by_string_first_occurence(input_line,"<<", &res_split);      
      is_double = 1;
    } else {
      split_by_char_first_occurence(input_line,'<', &res_split);
    }

    if( is_valid_path(res_split[0]) == 0 ) { 

      clean_spaces(res_split[0]);

      if(is_double == 1) {
        out = open(res_split[0], O_RDWR|O_CREAT|O_APPEND, 06755);
      } else {
        out = open(res_split[0], O_RDWR|O_CREAT|O_TRUNC, 06755);
      }
      if (-1 == out) { perror("Impossible de créer le fichier ou de l'ouvrir"); return 255;}

      int save_out = dup(fileno(stdout));

      if (-1 == dup2(out, fileno(stdout))) { perror("Impossible de rediriger stdout"); return 255; }

      res = main_exec(res_split[1], saved_stdout, saved_stdin); 

      fflush(stdout); close(out);
      dup2(save_out, fileno(stdout));
      close(save_out);


    } else if (res_split[0][0] == '2') {


      dup2(2, 1);

      res = main_exec(res_split[1], saved_stdout, saved_stdin);

      dup2(1, 1);
    } else { 

      char * swap = res_split[0];
      res_split[0] = res_split[1];
      res_split[1] = swap;
      res = fork_pipes(res_split, saved_stdout, saved_stdin);

    }

  } else {

    if(*(get_substring_adress_in_string(">", input_line)+2) == '>'){
      split_by_string_first_occurence(input_line,">>", &res_split);      
      is_double = 1;
    } else {
      split_by_char(input_line,'>', &res_split);
    }

    if( is_valid_path(res_split[1]) == 0) {


      clean_spaces(&res_split[1]);


      if(is_double == 1) {
        out = open(res_split[1], O_RDWR|O_CREAT|O_APPEND, 06755);
      } else {
        out = open(res_split[1], O_RDWR|O_CREAT|O_TRUNC, 06755);
      }
      
      if (-1 == out) { perror("Impossible de créer le fichier ou de l'ouvrir"); return 255;}
      int save_out = dup(fileno(stdout));

      if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }


      res = main_exec(res_split[0], saved_stdout, saved_stdin);

      fflush(stdout); close(out);
      dup2(save_out, fileno(stdout));
      close(save_out);


        // file exists
    } else if (res_split[1][0] == '2') {
      //dup2(2, 1);

      res = main_exec(res_split[0], 2, saved_stdin);

      dup2(1, 1);
    } else {

      res = fork_pipes(res_split, saved_stdout, saved_stdin);
    }

  }

  return res;
}

/**
 * \fn int exec_piping (char * input_line)
 *
 * \brief split input by the char "|", execute each bloc sequentially redirecting output in the next iteration input
 * \param input_line string to work on
 * \param saved_stdout output to dup
 * \param saved_stdin input to dup
 *
 * \return execution result : 0 = OK
 */
int exec_piping(char * input_line, int saved_stdout, int saved_stdin){


  char ** result_pipe_split;
  int nb_split, res;

  nb_split = split_by_char(input_line, '|', &result_pipe_split); // ( ps | grep sh | grep ??) => [0]["ps"] [1]["grep sh"] [2]["grep ??"]

  res = fork_pipes(result_pipe_split, saved_stdout, saved_stdin);

  return res;
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
  char* command_path = "/bin/";
  char* command_final;
  char* full_string = malloc(BUFFER_SIZE);
  char ** parameters_final;
  int i = 0, nb_arg, res;

  strcpy(full_string, parameters);

  nb_arg = split_line(parameters, " ", &parameters_final); 

  command_final = parameters_final[0];

  parameters_final[0] = command_final;

  if(strncmp("cd ",full_string,3) == 0){

    clean_spaces(&full_string);
    struct stat sb;
    
    if (stat(full_string+3, &sb) == 0 && S_ISDIR(sb.st_mode))
    {  
      return chdir(full_string+3);
    }
    else
    {
      errno = ENOTDIR;
      perror("");
      return 1;
    }

  } else if(strncmp("exit",full_string,4) == 0){
    exit(0);
  }

  pid_t pid = fork();

  if (pid == 0) {

    if(res = exec_if_built_in(full_string) == 0){

    } else{
      res = execvp (parameters_final[0], parameters_final);
    }

    if(res == -1){
      printf("%s: commande introuvable\n", command_final);
    }
  }
  else{ 
    int status = -1;
    wait(&status);
    res = status; 
  }

  return res;
}


/**
 * \fn int main_exec (char * input_line)
 *
 * \brief main execution logic, implements the character hierarchy on how to interpret the input
 * \param input_line string to work on
 * \param saved_stdout output to dup
 * \param saved_stdin input to dup
 *
 * \return execution result : 0 = OK
 */
int main_exec(char * input_line, int saved_stdout, int saved_stdin)
{

  dup2(saved_stdout, 1);
  dup2(saved_stdin, 0);

  int res;

  if( strstr(input_line,";")){
    res = exec_semicolon(input_line, saved_stdout, saved_stdin); // Lance autant d'exec de que bloc
  } 

  else if( input_line[strlen(input_line)-2] == '&'){
    res = exec_background(input_line, saved_stdout, saved_stdin);// Lance l'exec dans un processus fils sans attente
  }
  
  else if( strstr(input_line, "&&") || strstr(input_line, "||")){ 
    res = exec_logical_operator(input_line, saved_stdout, saved_stdin); // Lance des exec pour chaque bloc logique et test leurs validités
  }
  
  else if( strstr(input_line," >") || strstr(input_line," >>") || strstr(input_line, "< ") || strstr(input_line, "<< ")){
    res = exec_redirection(input_line, saved_stdout, saved_stdin); // Lance une chaine d'exec avec des redirections d'output/input
  }
  
  else if( strstr(input_line, " | ")){
    res = exec_piping(input_line, saved_stdout, saved_stdin);  // Lance des execs chainés dans des pipes
  }
  
  else{
    res = exec_command(input_line); // Lance une éxécution simple de la commande passée (built-in ou bash)
  }
  
  return res;

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
int main(int argc, char** argv) 
{
  char* input_line;  

  if(argc > 1){

    if(strstr(argv[1], "-c")){
      int i = 2;
      char * res = "";

      while(i<argc){

        input_line = concat(res,argv[i]);
        res = input_line;
        input_line = concat(res, " ");
        res = input_line;
        i++;
      }

      main_exec(input_line, 1, 0);
    }

  } else {

    do{
      char * currentDir = malloc(BUFFER_SIZE);
      getcwd(currentDir,BUFFER_SIZE);
      dprintf(STDOUT,"my_sh@%s> ",currentDir);
      
      input_line = read_and_histo();

      main_exec(input_line, 1, 0);

    } while(1); 
  }
  return EXIT_SUCCESS;
}