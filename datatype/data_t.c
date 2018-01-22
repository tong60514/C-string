#include <data_t.h>


void free_data_t(data_t* d)
{
	free(d->meta.data_mem_ptr);
	free(d);
}
