/*	This is pretty crude.  Originally, the Purdue code did all
 *	of the work, using sbrk() to get more memory from the system.
 *	I just use malloc().  Someday it might be worthwhile to put
 *	in the gc_ garbage collecting version and not worry about
 *	releasing memory.  I've probably broken some of the i-beam
 *	functions with this -- MEC
 */

struct memblock {
	char *block;
	unsigned nbytes;
	struct memblock *next;
};

static struct memblock *firstblock = 0;
extern char *malloc();
extern int aftrace;


alloc(nbytes)
unsigned nbytes;
{
	struct memblock *newblock, *block;
	unsigned count, sum;

	if (nbytes <= 0) return 0;
	newblock = (struct memblock *)malloc(sizeof(struct memblock));
	if (newblock == 0) goto failed;
	newblock->nbytes = nbytes;
	newblock->block = malloc(nbytes);
	if (newblock->block == 0) goto failed;
	if (aftrace) {
		printf("[alloc: %d bytes at %d]\n", nbytes, newblock);
	}
	newblock->next = firstblock;
	firstblock = newblock;
	return (int)newblock->block;
failed:
	printf("unable to obtain requested memory\n");
	printf("%d bytes were requested\n", nbytes);
	count = 0;
	sum = 0;
	for (block=firstblock; block; block=block->next) {
		count++;
		sum += block->nbytes;
	}
	printf("%d bytes currently in %d blocks\n", sum, count);
	abort();
}


aplfree(ap)
char *ap;
{
	struct memblock *item, *last;

	if (ap == 0) return 0;
	last = 0;
	for (item=firstblock; item; item=item->next) {
		if (item->block == ap) {
			if (aftrace) {
				printf("[dealloc: %d bytes at %d\n", item->nbytes, item->block);
			}
			if (last) last->next = item->next;
			else firstblock = item->next;
			free(item->block);
			free(item);
			return;
		}
		last = item;
	}
	printf("aplfree bad block address %d\n", ap);
}


afreset()
{
	struct memblock *item;

	for (item=firstblock; item; item=item->next) {
		free(item->block);
		free(item);
	}
	firstblock = 0;
}


memoryCheck()
{
	struct memblock *item;
	int count;
	long sum;

	if (firstblock == 0) {
		printf("no dynamic memory\n");
		return;
	}
	count = 0;
	sum = 0;
	for (item=firstblock; item; item=item->next) {
		count++;
		sum += item->nbytes;
	}
	printf("%d bytes in %d blocks\n", sum, count);
}
