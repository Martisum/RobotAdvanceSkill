#ifndef REFDEF_H
#define REFDEF_H
#include "util/PlayerTask.h"
class RefDef
{
public:
	RefDef();
	~RefDef();
	PlayerTask plan(int id, string role);
private:
	
};
typedef Singleton<RefDef> refDef;

#endif