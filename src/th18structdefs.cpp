#include "th18structdefs.h"
#include "touhou18.h"
namespace th18 {
using namespace th18;

    void zReplayManager::operator delete(void* self)
    {
        ((zReplayManager*)self)->destructor();
        operator_delete(self);
    }
}