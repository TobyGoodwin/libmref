#include <stdlib.h>
#include <string.h>

#include "mref.h"

size_t mref_field_length(struct mref *m, int f) {
    return m->fend[f] - m->fbeg[f];
}
