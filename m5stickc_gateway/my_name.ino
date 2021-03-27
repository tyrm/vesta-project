char my_name[32];

char* getName() {
  return my_name;
}

void setName(char * hostname) {
  strcpy(my_name, hostname);
}
