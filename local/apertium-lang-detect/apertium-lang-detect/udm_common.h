/************** Language and charset guesser *************/

#include <apertium-lang-detect/apertium-lang-detect-config.h>

#define UDM_LM_MAXGRAM		3
#define UDM_LM_HASHMASK		0x0FFF

typedef struct {
	int count;
	char str[UDM_LM_MAXGRAM+3];
} UDM_LANGITEM;

typedef struct {
	float  expectation;			/* Average value   */
	float  dispersion;			/* Math dispersion */
	char * lang;				/* Map Language    */
	char * charset;				/* Map charset     */
	UDM_LANGITEM  memb[UDM_LM_HASHMASK+1];	/* Items list      */
} UDM_LANGMAP;

typedef struct {
	size_t nmaps;
	UDM_LANGMAP * maps;
} UDM_LANGMAPLIST;

typedef struct{
	UDM_LANGMAPLIST LangMapList;
	int errcode;
	char errstr[1024];
} UDM_ENV;

#define UDM_FREE(x)		{if(x){free(x);x=NULL;}}
#define UdmGetToken(a,b,c)	strtok(a,b)

/*********************************************************/

