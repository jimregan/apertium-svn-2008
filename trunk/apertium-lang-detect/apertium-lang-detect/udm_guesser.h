#ifndef UDM_GUESSER_H
#define UDM_GUESSER_H


extern void  UdmBuildLangMap(UDM_LANGMAP * map,const char * text,size_t text_len);
extern void  UdmPrintLangMap(UDM_LANGMAP * map);
extern void  UdmPrepareLangMap(UDM_LANGMAP * map);
extern float UdmCheckLangMap(UDM_LANGMAP * map,UDM_LANGMAP * text);


extern int   UdmLoadLangMapList(UDM_ENV * env, const char * mapdir);
extern int   UdmLoadLangMapFile(UDM_ENV * env, const char * mapname);
extern void  UdmFreeLangMapList(UDM_ENV * env);

#endif
