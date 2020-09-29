#include <curses.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>


struct s_dir {
	   int tipo;
	   char *nombre;
   } res[128];
  

int LeeDirectorio(char *directorio){
       DIR *dir = opendir(directorio);
   struct dirent *dp;
   int i=0;
   while((dp=readdir(dir)) != NULL) {
      res[i].tipo = dp->d_type;
      res[i].nombre=dp->d_name;
	   
      i++;
   }
	
  closedir(dir);
  return i; 
}
  
int main()
{
   
   int c, i=0;
  char cwd[256];
  getcwd(cwd,256);
  int max= LeeDirectorio(cwd);
  

  for(int j=0; j<10; j++) {
	  if (res[j].tipo == DT_DIR) {
		 printf("D ");
	  }
	  else {
		 printf("F ");
	  }
      printf("%s%d\n",res[j].nombre,j);
   }
  
   initscr();
   raw();
   noecho(); /* No muestres el caracter leido */
   do {
      for (int j=0; j < max; j++) {
         if (j == i) {
           attron(A_REVERSE);
         }
         mvprintw(5+j,5,res[j].nombre);
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
	  case 10:
		if(res[j].tipo == DT_DIR)
		{
			strcat(cwd,"/",256);
			strcat(cwd,res[j].nombre,256);
			max= LeeDirectorio(cwd);
			i=0;
			clear();
		}
		      else
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
