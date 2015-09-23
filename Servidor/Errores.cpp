#include "Errores.h"


void error(int code, char *err)//TODO capaz se podria manejar con excepciones
{
  char *msg=(char*)malloc(strlen(err)+14);
  sprintf(msg, "Error %d: %s\n", code, err);
  fprintf(stderr, msg);
  exit(1);
}
