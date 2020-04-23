
#include <sys/stat.h>
#include <errno.h>
 
extern char _heap_min;
extern char _heap_max;
extern int errno;
 
caddr_t _sbrk(int incr) {
	static char *current = &_heap_min;
	char *previous = current;
 
	if (current + incr > (char*)& _heap_max) {
		errno = ENOMEM;
		return (caddr_t) - 1;
	}
 
	current += incr;
	return (caddr_t) previous ;
}