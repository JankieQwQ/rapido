#ifndef RAPIDO_H_
#define RAPIDO_H_
#define endl "\n"
void rprint(const char* str,char endl='\n');
void rnextline();
bool rfileoutput(const char* str,const char* filename);
void* rfileintput(const char* filename);
int rinput();

#endif