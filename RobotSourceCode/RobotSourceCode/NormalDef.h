#ifndef NORMALDEF_H
#define NORMALDEF_H
#include "util/PlayerTask.h"
class NormalDef
{
public:
	NormalDef();
	~NormalDef();
	PlayerTask plan(int id);
private:
	
};
typedef Singleton<NormalDef> normalDef;

#endif