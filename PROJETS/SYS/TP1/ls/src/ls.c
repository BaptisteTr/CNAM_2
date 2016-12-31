/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include<getopt.h>

#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096
#define BUFFER_SIZE 4096

#define USAGE_SYNTAX "[OPTIONS] -i INPUT -o OUTPUT"
#define USAGE_PARAMS "OPTIONS:\n\
  -i, --input  INPUT_FILE  : input file\n\
  -o, --output OUTPUT_FILE : output file\n\
***\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"

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


/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * \see man 3 getopt_long or getopt
 * \see struct option definition
 */
static struct option binary_opts[] = 
{
  { "help",    no_argument,       0, 'h' },
  { "verbose", no_argument,       0, 'v' },
  { "input",   required_argument, 0, 'i' },
  { "output",  required_argument, 0, 'o' },
  { 0,         0,                 0,  0  } 
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */ 
const char* binary_optstr = "hvi:o:";



/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{
  /**
   * Binary variables
   * (could be defined in a structure)
   */
  short int is_verbose_mode = 0;
  char* bin_input_param = NULL;
  char* bin_output_param = NULL;

  // Parsing options
  int opt = -1;
  int opt_idx = -1;

  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
      case 'i':
        //input param
        if (optarg)
        {
          bin_input_param = dup_optarg_str();         
        }
        break;
      case 'o':
        //output param
        if (optarg)
        {
          bin_output_param = dup_optarg_str();
        }
        break;
      case 'v':
        //verbose mode
        is_verbose_mode = 1;
        break;
      case 'h':
        print_usage(argv[0]);

        free_if_needed(bin_input_param);
        free_if_needed(bin_output_param);
 
        exit(EXIT_SUCCESS);
      default :
        break;
    }
  } 

  /**
   * Checking binary requirements
   * (could defined in a separate function)
   */
  if (bin_input_param == NULL)
  {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

    // Freeing allocated data
    free_if_needed(bin_input_param);
    free_if_needed(bin_output_param);
    // Exiting with a failure ERROR CODE (== 1)
    exit(EXIT_FAILURE);
  }


  // Printing params
  dprintf(STDOUT, "** PARAMS **\n%-8s: %s\n%-8s: %s\n%-8s: %d\n", 
          "input",   bin_input_param, 
          "output",  bin_output_param, 
          "verbose", is_verbose_mode);

  // Business logic must be implemented at this point

  DIR* directory = NULL;
  char * file_path = malloc(4096);

  if(realpath(bin_input_param,file_path) == NULL){
    dprintf(STDERR,"Impossible de trouver le chemin du dossier.%s\n",file_path);
    return EXIT_FAILURE;    
  }

  directory = opendir(file_path);
  struct dirent* read_file = NULL;
  struct passwd* file_passwd;
  struct group* file_grp;
  struct stat file_stat;
  struct tm* file_update;

  if(directory == NULL){
    dprintf(STDERR,"L'input n'est pas un dossier\n");
    return EXIT_FAILURE;
  }

  if(chdir(file_path) == -1){
    dprintf(STDERR,"Impossible de trouver le chemin du dossier.\n");
    return EXIT_FAILURE;
  }



  while((read_file = readdir(directory)) != NULL){

    stat(read_file->d_name, &file_stat);
    printf("%s\t",read_file->d_name);

    file_passwd = getpwuid(file_stat.st_uid);
    
    file_grp = getgrgid(file_stat.st_gid);

    file_update = localtime(&file_stat.st_mtime);
    
    printf( (S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf( (file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf( (file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (file_stat.st_mode & S_IXOTH) ? "x" : "-");

    printf("\t%s:%s\t",file_passwd->pw_name,file_grp->gr_name); // owner + group
    printf("%ld\t",file_stat.st_size);
    printf("%d/%d/%d %d:%d",file_update->tm_mday, file_update->tm_mon, file_update->tm_year+1900, file_update->tm_hour, file_update->tm_min); //Modif date
    printf("\n");
  }

  



  /* LOREM IPSUM DOT SIR AMET */

  // Freeing allocated data
  free_if_needed(bin_input_param);
  free_if_needed(bin_output_param);


  return EXIT_SUCCESS;
}
