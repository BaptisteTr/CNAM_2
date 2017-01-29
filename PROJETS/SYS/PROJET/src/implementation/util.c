#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>

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


  //dprintf(STDOUT,"POINTER_START = %d \n\n",pointer_start);
  char * i = pointer_start;
  int counter = 0;

  //dprintf(STDOUT,"substring building :\n");

  while((counter+i)<pointer_end){

    //dprintf(STDOUT,"COUNTER = %d ;\n I = %d;\n\n",counter,i);

    substring[counter] = (counter+i);
    counter++;
  }

  substring[counter] = '\0';

  *res = substring;

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

/**
 * \fn int split_by_char_first_occurence (char * input_line, char * character, char *** substring_tab)
 *
 * \brief search the input for the first occurence of character and split it in two parts, then put the results in substring_tab
 * \param input_line the string to process
 * \param character the character to search for
 * \param substring_tab the adress of the string array to put the results in
 *
 * \return the number of splits
 */
int split_by_char_first_occurence(char * input_line, char character, char *** substring_tab) {

  char ** res = NULL;
  char * substring;
  int i = 0, start_split = 0, end_split = 0, nb_split = 0;

  res = malloc(sizeof (char *) * 2);


  while(i<strlen(input_line)){

    if(input_line[i] == character){   
      end_split = i;

      res[0] =  substring_from_pos(input_line, start_split, end_split);

      start_split = i + 1;

      end_split = strlen(input_line);

      res[1] = substring_from_pos(input_line, start_split, end_split);

      i= strlen(input_line);
    }

    i++;
  }

  *substring_tab = res;

  return nb_split;

}

/**
 * \fn int split_by_string_first_occurence (char * input_line, char * string, char *** substring_tab)
 *
 * \brief search the input for the first occurence of string and split it in two parts, then put the results in substring_tab
 * \param input_line the string to process
 * \param string the string to search for
 * \param substring_tab the adress of the string array to put the results in
 *
 * \return the number of splits
 */
int split_by_string_first_occurence(char * input_line, char * string, char *** substring_tab){

  char ** res = NULL;
  char * substring;
  int i = 0, start_split = 0, end_split = 0, nb_split = 0;

  res = malloc(sizeof (char *) * 2);

  while(i<strlen(input_line)){
   
    if(input_line[i] == string[0] && input_line[i+1] == string[1]){   
      end_split = i;

      res[0] =  substring_from_pos(input_line, start_split, end_split);

      start_split = i + strlen(string);

      end_split = strlen(input_line);

      res[1] = substring_from_pos(input_line, start_split, end_split);

      i= strlen(input_line);
    }

    i++;
  }

  *substring_tab = res;

  return nb_split;
}

/**
 * \fn void clean spaces (char ** input_line)
 *
 * \brief modify the input by removing spaces at the end and the beginning
 * \param input_line the adress of the string to modify
 *
 * \return void
 */
void clean_spaces(char ** input_line) {

  int i;
  char * input = *input_line;

  for(i=strlen(input)-1; input[i] == ' ' || input[i] == '\0' || input[i] == '\n'; i--){
    input[i] = NULL;
  }
  input[i+1] = '\0';

  while(input[0] == ' '){
    input[0] = NULL;
    input++;
  }

  *input_line = input;
}

/**
 * \fn int is_valid_path (char * input_line)
 *
 * \brief check if the input is a valid file path
 * \param input_line the input to check
 *
 * \return 0 = TRUE, 1 = FALSE 
 */
int is_valid_path(char * input_line){ // 0 == true

  int res = 0, i = 0, j = strlen(input_line)-1, has_slash = 1;

  while(input_line[i] == ' '){
    i++;
  }

  while(input_line[j] == ' '){
    j--;
  }

  while(i<=j){

    if(input_line[i] == '/'){
      has_slash = 0;
    } else if(input_line[i] == ' '){
      res = 1;
    }
    i++;
  }


  if(has_slash == 0) {
    return res;
  } else {
    return 1;
  }

}

/**
 * \fn int init_pipe (int in, int out, char * input_line)
 *
 * \brief fork a child process and pipe in as input and out as output
 * \param in input pipe
 * \param out output pipe
 * \param input_line string to execute
 *
 * \return execution result : 0 = OK
 */
int init_pipe (int in, int out, char * input_line)
{
  pid_t pid;

  if ((pid = fork ()) == 0)
    {
      if (in != 0)
        {
          dup2 (in, 0);
          close (in);
        }

      if (out != 1)
        {
          dup2 (out, 1);
          close (out);
        }


      main_exec(input_line);

    }

  return pid;
}


/**
 * \fn int fork_pipes (int n, char ** input_line)
 *
 * \brief execute the list of input sequentially and pipe each output in the next input
 * \param n number of iteration
 * \param **input_line array of string to work on
 *
 * \return execution result : 0 = OK
 */
int fork_pipes (int n, char ** input)
{
  int i, res;
  pid_t pid;
  int in, fd [2];

  /* The first process should get its input from the original file descriptor 0.  */
  in = 0;

  /* Note the loop bound, we spawn here all, but the last stage of the pipeline.  */
  for (i = 0; i < n - 1; ++i)
    {
      pipe (fd);

      /* f [1] is the write end of the pipe, we carry `in` from the prev iteration.  */
      init_pipe (in, fd [1], input[i]);

      /* No need for the write end of the pipe, the child will write here.  */
      close (fd [1]);

      /* Keep the read end of the pipe, the next child will read from there.  */
      in = fd [0];
    }

  /* Last stage of the pipeline - set stdin be the read end of the previous pipe
     and output to the original file descriptor 1. */  
  if (in != 0)
    dup2 (in, 0);

  /* Execute the last stage with the current process. */

  res = main_exec(input [i]);

  close(in);
  close(fd[1]);
  close(fd[2]);
  dup2(0,0);
  dup2(1,1);

  return res;
}
