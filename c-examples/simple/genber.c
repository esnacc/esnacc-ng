// file: .../c++examples/simple/genber.C---builds an PersonnelRecord value and writes BER form of the value to a file called "pr.ber"
//
// MS 92
//
// $Header: /baseline/SNACC/c-examples/simple/genber.c,v 1.2 2003/12/17 19:05:03 gronej Exp $
// $Log: genber.c,v $
// Revision 1.2  2003/12/17 19:05:03  gronej
// SNACC baseline merged with PER v1_7 tag
//
// Revision 1.1.2.1  2003/11/05 14:58:57  gronej
// working PER code merged with esnacc_1_6
//
// Revision 1.1.1.1  2000/08/21 20:36:07  leonberp
// First CVS Version of SNACC.
//
// Revision 1.5  1995/07/24 15:40:32  rj
// changed `_' to `-' in file names.
//
// Revision 1.4  1994/12/11  15:36:14  rj
// const for a constant value [DEC]
//
// Revision 1.3  1994/10/08  01:27:03  rj
// several \size_t'
//
// Revision 1.2  1994/08/31  08:56:33  rj
// first check-in. for a list of changes to the snacc-1.1 distribution please refer to the ChangeLog.
//


#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "asn-incl.h"
#include "sbuf.h"
#include "nibble-alloc.h"
#include "p-rec.h"

int
main (int argc, char *argv[])
{
    FILE *outputFile;
    SBuf outputBuf;
    SBuf *outputBufP;
    unsigned long int encodedLen;
    int dataSize = 1024;
    int i;
    char data[1024];
    PersonnelRecord pr;
    ChildInformation **childHndl;

    InitNibbleMem(512,512);

    pr.name = (Name *)Asn1Alloc(sizeof(Name));
    pr.name->givenName.octs = "John";
    pr.name->givenName.octetLen = strlen(pr.name->givenName.octs);

    pr.name->initial.octs = "E";
    pr.name->initial.octetLen = strlen(pr.name->initial.octs);

    pr.name->familyName.octs = "Smith";
    pr.name->familyName.octetLen = strlen(pr.name->familyName.octs);

    pr.title.octs = "El Presidente";
    pr.title.octetLen = strlen(pr.title.octs);

    pr.employeeNumber = 91991;

    pr.dateOfHire.octs = "20071024";
    pr.dateOfHire.octetLen = strlen(pr.dateOfHire.octs);

    pr.nameOfSpouse = (Name *) Asn1Alloc(sizeof(Name));
    pr.nameOfSpouse->givenName.octs = "Mary";
    pr.nameOfSpouse->givenName.octetLen =
        strlen(pr.nameOfSpouse->givenName.octs);
    pr.nameOfSpouse->initial.octs = "F";
    pr.nameOfSpouse->initial.octetLen =
        strlen(pr.nameOfSpouse->initial.octs);
    pr.nameOfSpouse->familyName.octs = "Smith";
    pr.nameOfSpouse->familyName.octetLen =
        strlen(pr.nameOfSpouse->familyName.octs);

    pr.children = AsnListNew(sizeof(void*));

    childHndl = AsnListAppend(pr.children);
    *childHndl = Asn1Alloc(sizeof(ChildInformation));

    (*childHndl)->dateOfBirth.octs = "20090123";
    (*childHndl)->dateOfBirth.octetLen = strlen((*childHndl)->dateOfBirth.octs);
    (*childHndl)->name = (Name*)Asn1Alloc(sizeof(Name));
    (*childHndl)->name->givenName.octs = "Jacob";
    (*childHndl)->name->givenName.octetLen =
        strlen((*childHndl)->name->givenName.octs);
    (*childHndl)->name->initial.octs = "J";
    (*childHndl)->name->initial.octetLen =
        strlen((*childHndl)->name->initial.octs);
    (*childHndl)->name->familyName.octs = "Smith";
    (*childHndl)->name->familyName.octetLen =
        strlen((*childHndl)->name->initial.octs);

    childHndl = AsnListAppend(pr.children);
    *childHndl = Asn1Alloc(sizeof(ChildInformation));
    (*childHndl)->dateOfBirth.octs = "20101130";
    (*childHndl)->dateOfBirth.octetLen = strlen((*childHndl)->dateOfBirth.octs);
    (*childHndl)->name = (Name*)Asn1Alloc(sizeof(Name));
    (*childHndl)->name->givenName.octs = "Jane";
    (*childHndl)->name->givenName.octetLen =
        strlen((*childHndl)->name->givenName.octs);
    (*childHndl)->name->initial.octs = "D";
    (*childHndl)->name->initial.octetLen =
        strlen((*childHndl)->name->initial.octs);
    (*childHndl)->name->familyName.octs = "Smith";
    (*childHndl)->name->familyName.octetLen =
        strlen((*childHndl)->name->initial.octs);

    SBufInit(&outputBuf, data, dataSize);
    SBufResetInWriteRvsMode(&outputBuf);

    encodedLen = BEncPersonnelRecord((GenBuf *)&outputBuf, &pr);

    if ((encodedLen <= 0) || (SBufWriteError(&outputBufP))) {
        fprintf(stderr, "failed encoding Personnel Record");
        exit(1);
    }

    outputFile = fopen("pr.ber", "w");
    if (!outputFile) {
        perror("fopen");
        exit(1);
    }

    SBufResetInReadMode(&outputBufP);
    for (;encodedLen > 0;encodedLen--){
        fputc(SBufGetByte(&outputBufP), outputFile);
    }
    printf("wrote the BER value to pr.ber\n");
    printf("test with \"def\" and \"indef\"\n");

    PrintPersonnelRecord(stdout, &pr, 0);
    printf("\n");
    return 0;
}
