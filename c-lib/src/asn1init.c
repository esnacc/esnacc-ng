/* Included files */
#include "config.h"
#include "asn-incl.h"
#include "nibble-alloc.h"

void FreeHashTable(Table* pTable);


void ASN1init()
{
    ExpBufInit (1024);	/* Should this be higher? */

	/* decoding buffer init - use mem within buffer rather than allocating all
	 * the time... I will initially alloc 4K since the certs are generally in
	 * the 3k - 4k range
	 */
	InitNibbleMem(4096, 512);	/* intially alloc 4096, inc by 512 as required */

}

void ASN1Terminate()
{
	/* Frees the existing hash tables */
	FreeHashTable(anyOidHashTblG);
	anyOidHashTblG = NULL;
	FreeHashTable(anyIntHashTblG);
	anyIntHashTblG = NULL;

	/* Releases the nibble memory completely */
	ShutdownNibbleMem();
}

void FreeHashTable(Table* pTable)
{
	unsigned int i;
	HashSlot* pSlot;

	if (pTable == NULL)
		return;

	for (i = 0; i < TABLESIZE; i++)
	{
		pSlot = (HashSlot*) (*pTable)[i];
		if (pSlot != NULL)
		{
			if (pSlot->table != NULL)
				FreeHashTable(pSlot->table);
			else
				free(pSlot->value);

			free(pSlot);
			(*pTable)[i] = NULL;
		}
	}

	free(pTable);
}
