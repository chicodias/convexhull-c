/* minusc.c
 *
 * Converte arquivos para nomes com letras
 * minusculas e modifica CR+LF para LF.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

void minusc(char *s)
{
  for (; *s != '\0'; s++)
    *s = tolower(*s);
}

int conv_arq(char *nome)
{
  FILE *r, *w;
  char str[1024];
  int c;

  strcpy(str, nome);
  minusc(str);

  if (strcmp(str, nome) == 0) {
    printf("%s: nome ja em minusculas, ignorando\n", nome);
    return 0;
  }

  if ((r = fopen(nome, "r")) == NULL) {
    printf("%s: impossivel ler arquivo\n", nome);
    return 1;
  }
  if ((w = fopen(str, "w")) == NULL) {
    printf("%s: impossivel escrever arquivo\n", str);
    fclose(r);
    return 1;
  }

  printf("%s => %s\n", nome, str);

  while ((c = fgetc(r)) != EOF)
    if (c != '\r')
      fputc(c, w);
  fclose(r);
  fclose(w);
  remove(nome);
  return 0;
}

int main(int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; i++)
    conv_arq(argv[i]);
  return 0;
}
