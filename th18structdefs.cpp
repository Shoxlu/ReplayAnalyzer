#include "th18structdefs.h"
#include "touhou18.h"

void zReplayManager::operator delete(void* self)
{
    ((zReplayManager*)self)->destructor();
    operator_delete(self);
}