

/* main.c */
#include "test.h"

int main(){
 	P *p = createP();
	setIP_P(p, "1.2.3.4");
	sendP(p);
	destroy(p);
}






/* test.c */
struct P {
	int ip;
};

struct P* createP(void);
void setIP_P(struct P*, const char*);
void destory(struct P*);






/* test.h */
struct P;
typedef struct P;


P* createP(void);
void setIP_P(P*, const char*);
void destory(P*);
