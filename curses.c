#include <curses.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>


struct s_dir {
	   int tipo;
	   char *nombre;
   } res[128];
  
/*int leeChar() {
  int chars[5];
  int ch,i=0;
  nodelay(stdscr, TRUE);
  while((ch = getch()) == ERR); /* Espera activa 
  ungetch(ch);
  while((ch = getch()) != ERR) {
    chars[i++]=ch;
  }
  /* convierte a numero con todo lo leido 
  int res=0;
  for(int j=0;j<i;j++) {
    res <<=8;
    res |= chars[j];
  }
  return res;
}*/
int LeeDirectorio(char *directorio){
       DIR *dir = opendir(directorio);
   struct dirent *dp;
   int i=0;
   while((dp=readdir(dir)) != NULL) {
      res[i].tipo = dp->d_type;
      res[i].nombre=dp->d_name;
	   
      i++;
   }
	 for(int j=0; j<i; j++) {
	  if (res[j].tipo == DT_DIR) {
		 printf("D ");
	  }
	  else {
		 printf("F ");
	  }
      printf("%s\n",res[j].nombre);
   }
  closedir(dir);
  return i; 
}
  
int main()
{
   //char *lista[] = {"Uno", "Dos", "Tres", "Cuatro" };
  // int i = 0;
   int c, i=0;
  char cwd[256];
  getcwd(cwd,256);
  int max= LeeDirectorio(cwd);
  

  /*for(int j=0; j<i; j++) {
	  if (res[j].tipo == DT_DIR) {
		 printf("D ");
	  }
	  else {
		 printf("F ");
	  }
      printf("%s\n",res[j].nombre);
   }
*/
   //
  
   initscr();
   raw();
   noecho(); /* No muestres el caracter leido */
   //cbreak(); /* Haz que los caracteres se le pasen al usuario */
   do {
      for (int j=0; j < max; j++) {
         if (j == i) {
           attron(A_REVERSE);
         }
        char *direcmov= res[j].nombre;
         mvprintw(5+j,4,direcmov);
         attroff(A_REVERSE);
      }
     
      move(5+i,4);
      refresh();
      c = getchar();
      switch(c) {
         case 65:
            i = (i>0) ? i - 1 : max-1;
            break;
         case  66:
            i = (i<(max-1)) ? i + 1 : 0;
            break;
         default:
            // Nothing 
            break;
      }
      move(2,10);
      printw("Estoy en %d: Lei %d Num %d Dir: %s",i,c,max,cwd);
   } while (c != 'q');
   endwin();
   return 0;
}
