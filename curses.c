#include <curses.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/mman.h>


struct s_dir {
	   int tipo;
	   char *nombre;
   } res[128];
  

/* Variable global para mejor legibilidad */
int fd; // Archivo a leer


char *hazLinea(char *base, int dir) {
	char linea[100]; // La linea es mas pequeña
	int o=0;
	// Muestra 16 caracteres por cada linea
	o += sprintf(linea,"%08x ",dir); // offset en hexadecimal
	for(int i=0; i < 4; i++) {
		unsigned char a,b,c,d;
		a = base[dir+4*i+0];
		b = base[dir+4*i+1];
		c = base[dir+4*i+2];
		d = base[dir+4*i+3];
		o += sprintf(&linea[o],"%02x %02x %02x %02x ", a, b, c, d);
	}
	for(int i=0; i < 16; i++) {
		if (isprint(base[dir+i])) {
			o += sprintf(&linea[o],"%c",base[dir+i]);
		}
		else {
			o += sprintf(&linea[o],".");
		}
	}
	sprintf(&linea[o],"\n");

	return(strdup(linea));
}

char *mapFile(char *filePath) {
    /* Abre archivo */
    fd = open(filePath, O_RDONLY);
    if (fd == -1) {
    	perror("Error abriendo el archivo");
	    return(NULL);
    }

    /* Mapea archivo */
    struct stat st;
    fstat(fd,&st);
    int fs = st.st_size;

    char *map = mmap(0, fs, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
    	close(fd);
	    perror("Error mapeando el archivo");
	    return(NULL);
    }

  return map;
}

int leeChar() {
  int chars[5];
  int ch,i=0;
  nodelay(stdscr, TRUE);
  while((ch = getch()) == ERR); /* Espera activa */
  ungetch(ch);
  while((ch = getch()) != ERR) {
    chars[i++]=ch;
  }
  /* convierte a numero con todo lo leido */
  int res=0;
  for(int j=0;j<i;j++) {
    res <<=8;
    res |= chars[j];
  }
  return res;
}

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
int edita(char *nombre)
{
	char *map = mapFile( nombre);
    				if (map == NULL) {
				      exit(EXIT_FAILURE);
				      }

				    for(int i= 0; i<25; i++) {
					// Haz linea, base y offset
					char *l = hazLinea(map,i*16);
					    mvprintw(i,0,l);
				    }
				    refresh();

				    leeChar();
				    endwin();

				    if (munmap(map, fd) == -1) {
				      perror("Error al desmapear");
				    }
				    close(fd);
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
	  case 13:
		if(res[i].tipo == DT_DIR)
		{
			if(strcmp(res[i].nombre,".")==0)
			{
			}
			else{
				if(strcmp(res[i].nombre,"..")==0)
			{
					char *p= strrchr(cwd,'/');
					if(p!=cwd){
						*p=0;
					}
					else{
						*(p+1)=0:
					}
			}
			strncat(cwd,"/",256);
			strncat(cwd,res[i].nombre,256);
			max= LeeDirectorio(cwd);
			i=0;
			clear();
			}
		}
		      else
		      {
			       

				edita(res[i].nombre);
			      clear();
		      }
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
