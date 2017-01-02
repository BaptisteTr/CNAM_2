

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