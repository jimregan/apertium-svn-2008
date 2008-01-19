#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>

#include <unistd.h>

#include <apertium-lang-detect/udm_common.h>
#include <apertium-lang-detect/udm_crc32.h>
#include <apertium-lang-detect/udm_guesser.h>

typedef struct res1{
        float quality;
	char language[8];
} resultado;
                

char * strdup(const char *);

/* Structure to sort guesser results */
typedef struct {
	UDM_LANGMAP * map;
	float quality;
} UDM_MAPSTAT;


	UDM_ENV env;
	UDM_LANGMAP mchar;
	UDM_MAPSTAT *mapstat;

int UdmLoadLangMapFile(UDM_ENV * Env, const char * filename){
	FILE * f;
	char str[1000], *charset, *lang, *s;
	int nitems=0;
	if(!Env->LangMapList.nmaps){
		Env->LangMapList.maps=(UDM_LANGMAP*)malloc(sizeof(UDM_LANGMAP));
	}else{
		Env->LangMapList.maps=(UDM_LANGMAP*)realloc(Env->LangMapList.maps,(Env->LangMapList.nmaps+1)*sizeof(UDM_LANGMAP));
	}
	Env->LangMapList.nmaps++;
	memset(&Env->LangMapList.maps[Env->LangMapList.nmaps-1],0,sizeof(UDM_LANGMAP));

	f=fopen(filename,"r");
	if(!f){
		Env->errcode=1;
		sprintf(Env->errstr,"Can't open LangMapFile '%s'\n",filename);
		return -1;
	}
	while(fgets(str,sizeof(str),f)){
		if(str[0]=='#'||str[0]==' '||str[0]=='\t')continue;

		if(!strncmp(str,"Charset:",8)){
			UDM_FREE(Env->LangMapList.maps[Env->LangMapList.nmaps-1].charset);
			if((charset=UdmGetToken(str+8," \t\n\r",&lasttok))){
				Env->LangMapList.maps[Env->LangMapList.nmaps-1].charset= (char *) strdup((const char *) charset);
			}
		}else
		if(!strncmp(str,"Language:",9)){
			UDM_FREE(Env->LangMapList.maps[Env->LangMapList.nmaps-1].lang);
			if((lang=UdmGetToken(str+9," \t\n\r",&lasttok))){
				Env->LangMapList.maps[Env->LangMapList.nmaps-1].lang=strdup(lang);
			}
		}else{
			int count;
			
			if(!(s=strchr(str,'\t')))continue;
			*s='\0';

			if(((count=atoi(s+1))==0)||(strlen(str)<1)||(strlen(str)>UDM_LM_MAXGRAM))
				continue;

			for(s=str;*s;s++){
				if(*s=='_')*s=' ';
			}
			if(*str){
				int hindex;
				hindex=((unsigned int)UdmCRC32(str,strlen(str)))&UDM_LM_HASHMASK;
				Env->LangMapList.maps[Env->LangMapList.nmaps-1].memb[hindex].count=count;
				/*
				strcpy(Env->LangMapList.maps[Env->LangMapList.nmaps-1].memb[hindex].str,str);
				*/
				nitems++;
			}
		}
	}
	fclose(f);
	
	if(!Env->LangMapList.maps[Env->LangMapList.nmaps-1].lang){
		Env->errcode=1;
		sprintf(Env->errstr,"No language definition in LangMapFile '%s'\n",filename);
		return -1;
	}

	if(!Env->LangMapList.maps[Env->LangMapList.nmaps-1].charset){
		Env->errcode=1;
		sprintf(Env->errstr,"No charset definition in LangMapFile '%s'\n",filename);
		return -1;
	}
	UdmPrepareLangMap(&Env->LangMapList.maps[Env->LangMapList.nmaps-1]);
	return 0;
}


int UdmLoadLangMapList_FromList(UDM_ENV * Env, int nfiles, char *files[])
{
  int i;
  int res = 0;
  char fullname[1024]="";

  Env->LangMapList.nmaps=0;
  
  for(i = 0; i != nfiles; i++)
  {
    sprintf(fullname,"%s/%s", ALL_MAPS_DIR, files[i]);
    res = UdmLoadLangMapFile(Env, fullname);
    if(res < 0)
    {
      return res;
    }
  }
  
  return 0;
}


int UdmLoadLangMapList(UDM_ENV * Env, const char * mapdir){
	DIR * dir;
	struct dirent * item;
	char fullname[1024]="";
	char name[1024]="";
	int res=0;

	Env->LangMapList.nmaps=0;
	dir=opendir(mapdir);
	if(!dir)return 0;

	while((item=readdir(dir))){
		char * tail;
		strcpy(name,item->d_name);
		if((tail=strstr(name,".lm"))){
			*tail='\0';
			sprintf(fullname,"%s/%s",mapdir,item->d_name);
			res=UdmLoadLangMapFile(Env,fullname);
			if(res<0){
				return res;
			}
		}
	}
	closedir(dir);
	return 0;
}


int UdmLoadLangMapListConstrainted_FromList(UDM_ENV * Env, int argc, char *argv[], int nfiles, char *files[])
{
  int i, j;
  char fullname[1024]="";
  int res;

  Env->LangMapList.nmaps=0;

  for(i = 0; i != nfiles; i++)
  {
    for(j = 1; j != argc; j++)
    {
      if(!strncmp(argv[j], files[i], 2))
      {
        sprintf(fullname,"%s/%s", ALL_MAPS_DIR, files[i]);
	res=UdmLoadLangMapFile(Env, fullname);
	if(res<0)
	{
	  return res;
	}
      }		     
    }
  }
}

int UdmLoadLangMapListConstrainted(UDM_ENV * Env, const char * mapdir, int argc, char *argv[])
{
	DIR * dir;
	struct dirent * item;
	char fullname[1024]="";
	char name[1024]="";
	int res=0;

	Env->LangMapList.nmaps=0;
	dir=opendir(mapdir);
	if(!dir)return 0;

	while((item=readdir(dir))){
		char * tail;
		int i;
		for(i = 1; i != argc; i++)
		{
		  if(!strncmp(argv[i], item->d_name, 3))
		  {
	            strcpy(name,item->d_name);
		    if((tail=strstr(name,".lm")))
		    {
		      *tail='\0';
		      sprintf(fullname,"%s/%s",mapdir,item->d_name);
		      res=UdmLoadLangMapFile(Env,fullname);
		      if(res<0)
		      {
			return res;
		      }
		    }
		  }
		}
	}
	closedir(dir);
	return 0;
}


void UdmFreeLangMapList(UDM_ENV * env){
	size_t i;
	
	if(!env)return;
	for(i=0;i<env->LangMapList.nmaps;i++){
		if(env->LangMapList.maps[i].charset){
			UDM_FREE(env->LangMapList.maps[i].charset);
		}
		if(env->LangMapList.maps[i].lang){
			UDM_FREE(env->LangMapList.maps[i].lang);
		}
	}
	free(env->LangMapList.maps);
}


void UdmBuildLangMap(UDM_LANGMAP * map,const char * text,size_t textlen){
	const char * end=text+textlen;
	int prevb=' ';

	for(;text<=end;text++){
		char buf[UDM_LM_MAXGRAM+3];
		size_t buflen=0;
		const char * t;
		int code;
		int prev=0;

		code=(unsigned char)(*text);
		if(code<' ')continue;
		if((code==' ')&&(prevb==' '))continue;
		prevb=code;

		t=text;
		for(buflen=0;buflen<UDM_LM_MAXGRAM;buflen++){
			int hindex;

			for(;t<=end;t++){
				code=(unsigned char)(*t);
				if(code<' ')continue;
				if((code==' ')&&(prev==' '))continue;
				prev=code;
				break;
			}
			if(t>end)break;
			t++;

			buf[buflen]=code;
			buf[buflen+1]='\0';

			hindex=UdmCRC32(buf,buflen+1);
			hindex=((unsigned int)(hindex))&(UDM_LM_HASHMASK);
			map->memb[hindex].count++;

#ifdef DEBUG_GUESSER
			/* Print collision */
			if(map->memb[hindex].str[0]){
				int res;
				res=strcmp(map->memb[hindex].str,buf);
				if(res){
					printf("Coll %04X '%s' '%s'\n",hindex,map->memb[hindex].str,buf);
					strcpy(map->memb[hindex].str,buf);
				}
			}
#endif
			strcpy(map->memb[hindex].str,buf);
		}
	}
}


static int cmpm(const void * i1,const void * i2){
	const UDM_LANGITEM * m1=i1;
	const UDM_LANGITEM * m2=i2;
	
	return(m2->count-m1->count);
}

void UdmPrintLangMap(UDM_LANGMAP * map){
	size_t i;
	
	printf("#\n");
	printf("#\n");
	printf("#\n");
	printf("\n");
	printf("Language: %s\n",map->lang);
	printf("Charset:  %s\n",map->charset);
	printf("\n");
	printf("\n");
	qsort(map->memb,UDM_LM_HASHMASK+1,sizeof(UDM_LANGITEM),&cmpm);
	for(i=0;i<=UDM_LM_HASHMASK;i++){
		char * s;
		if(!map->memb[i].count)break;
		if(i==400)break;
		
		for(s=map->memb[i].str;*s;s++)
			if(*s==' ')*s='_';
		
		printf("%s\t%d\n",map->memb[i].str,map->memb[i].count);
	}
}

void UdmPrepareLangMap(UDM_LANGMAP * map){
	int i;
	int nmemb=UDM_LM_HASHMASK+1;
	float expectation=0;
	float dispersion=0;

	/* Calculate math expectation */
	for(i=0;i<nmemb;i++){
		expectation+=map->memb[i].count;
	}
	expectation/=nmemb;

	/* Calculate math dispersion */
	for(i=0;i<nmemb;i++){
		dispersion+=(map->memb[i].count-expectation)*(map->memb[i].count-expectation);
	}
	map->expectation=expectation;
	map->dispersion=sqrt(dispersion/nmemb);
}


float UdmCheckLangMap(UDM_LANGMAP * map0,UDM_LANGMAP * map1){
	float res=0;
	float exp0=0;
	float exp1=0;
	float up=0;
	float nmemb=UDM_LM_HASHMASK+1;
	int i;
	UDM_LANGITEM * mp0, *mp1;

	/* Abort if one of dispertions is 0 */
	if((map0->dispersion<0.00001)||(map1->dispersion<0.00001))return 0;

	exp0=map0->expectation;
	exp1=map1->expectation;

	for(i=0,mp0=map0->memb,mp1=map1->memb;i<nmemb;i++,mp0++,mp1++){
		up+=(mp0->count-exp0)*(mp1->count-exp1);
	}
	up/=nmemb;

	res=up/map0->dispersion/map1->dispersion;
	return res;
}



static int statcmp(const void * i1, const void * i2){
	float fres;
	fres = ((const UDM_MAPSTAT*)(i2))->quality - ((const UDM_MAPSTAT*)(i1))->quality;
	if(fres<0)return -1;
	if(fres>0)return +1;
	return 0;
}

/*************************************************************************/


/***************************************************************/


int mguesser_init(int argc, char *argv[], int nfiles, char *files[]){
	int verbose=0;
	int print=0;
	int n=1000;

/*** MEUUUU ****/
n=4; /*numero de resultados que consideramos*/
/***************/

	/* Init structures */
	memset(&env,0,sizeof(env));
	memset(&mchar,0,sizeof(mchar));

	if(!print){
		/* Load all available lang ngram maps */
		if(verbose){
			fprintf(stderr,"Loading language maps from '%s'\n", ALL_MAPS_DIR);
		}
		if(argc > 2)
		{
		  UdmLoadLangMapListConstrainted_FromList(&env, argc, argv, nfiles, files);
		}
		else
		{
		  UdmLoadLangMapList_FromList(&env, nfiles, files);
		}
		if(env.errcode){
			printf("Error: '%s'\n",env.errstr);
			return 1;
		}

		if(verbose){
			fprintf(stderr,"%d maps found\n",env.LangMapList.nmaps);
		}
	}

	return 0;

} /*LOAD the maps to use with mguesser*/


/*Function to use mguesser with loaded maps*/
int mguesser(FILE *buffer, int num, resultado *resultados){
	int print=0;
	int n=1000;
	char buf[1024]="";
	char * charset=NULL;
	char * lang=NULL;
	
	/* Add each STDIN line statistics */
/*	while(fgets(buf,sizeof(buf),stdin)){*/
/*MEUUUUU*/
	while(fgets(buf,sizeof(buf),buffer)){
		UdmBuildLangMap(&mchar,buf,strlen(buf));
	}
	
	
	if(print){
		/* Display built langmap */
		if(!charset){
			fprintf(stderr,"You must specify charset using -c\n");
		}else
		if(!lang){
			fprintf(stderr,"You must specify language using -l\n");
		}else{
			mchar.lang=strdup(lang);
			mchar.charset=strdup(charset);
			UdmPrintLangMap(&mchar);
		}
	}else{
		int i;

		
		/* Prepare map to comparison */
		UdmPrepareLangMap(&mchar);

		/* Allocate memory for comparison statistics */
		mapstat=(UDM_MAPSTAT *)malloc(env.LangMapList.nmaps*sizeof(UDM_MAPSTAT));

		/* Calculate each lang map        */
		/* correlation with text          */
		/* and store in mapstat structure */

		for(i=0;i<env.LangMapList.nmaps;i++){
			mapstat[i].quality=UdmCheckLangMap(&env.LangMapList.maps[i],&mchar);
			mapstat[i].map=&env.LangMapList.maps[i];
		}

		/* Sort statistics in quality order */
		qsort(mapstat,env.LangMapList.nmaps,sizeof(UDM_MAPSTAT),&statcmp);


		/* Display results. Best language is shown first. */
		for(i=0;(i<env.LangMapList.nmaps)&&(i<n);i++){
			resultados[i].quality=mapstat[i].quality;
			strcpy(resultados[i].language,mapstat[i].map->lang);
		}

		/* Free variables */
		free(mapstat);

	}
	
	UdmFreeLangMapList(&env);

	return 0;
}

void mguesser_close(){
/*	UdmFreeLangMapList(&env); */
	return;	
}

