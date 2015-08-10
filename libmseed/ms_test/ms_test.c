/***************************************************************************
 * mstest.c
 *
 * Skara MiniSeed test program
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <libmseed.h>

/*
#define VERSION "[libmseed " LIBMSEED_VERSION " example]"
#define PACKAGE "msview"

static short int verbose   = 0;
static short int ppackets  = 0;
static short int basicsum  = 0;
static int   reclen        = -1;
static char *inputfile     = 0;

static int parameter_proc (int argcount, char **argvec);
static void usage (void);
static void term_handler (int sig);*/

static FILE* outfile;

static void record_handler (char *record, int reclen, void *srcname)
{
    if (fwrite(record, reclen, 1, outfile) != 1)
    {
        ms_log(2, "Error writing to output file %s\n", (char*)srcname);
    }
}

int main(int argc, char **argv)
{
    int psamples;
    int precords;
    MSRecord *msr;
    char srcname[50];
    int* dataptr;
    int verbose; // debug text
    int i;

    int retcode;
    int totalrecs;
    int totalsamps;

    msr = msr_init(NULL);

    /* Populate MSRecord values */
    strcpy(msr->network, "XX");
    strcpy(msr->station, "TEST");
    strcpy(msr->channel, "BHE");
    msr->starttime = ms_seedtimestr2hptime("2004,350,00:00:00.00");
    msr->samprate = 40.0;
    //msr->record = malloc(8192);
    //memset(msr->record, 0x22, 8192);
    msr->reclen = 4096;         /* 4096 byte record length */
    msr->record = NULL;
    msr->encoding = DE_STEIM2;  /* Steim 2 compression */
    msr->byteorder = 1;         /* big endian byte order */

    dataptr = (int*) malloc(1234*sizeof(int));
    //memset(dataptr, 0x34, 1234*sizeof(int));
    for (i = 0; i < 1234; ++i)
    {
        dataptr[i] = rand() % 1000;
    }
    verbose = 1;

    msr->datasamples = dataptr; /* pointer to 32-bit integer data samples */  
    msr->numsamples = 1234;
    msr->sampletype = 'i';      /* declare type to be 32-bit integers */

    msr_srcname (msr, srcname, 0);

    /* Pack the record(s) */
    outfile = fopen("myfile.mseed", "wb+");
    precords = msr_pack (msr, &record_handler, srcname, &psamples, 1, verbose);
    fclose(outfile);

    ms_log(0, "Packed %d samples into %d records\n", psamples, precords);

    msr_free(&msr);
    free(dataptr);

    msr = 0;
    retcode = 0;
    totalsamps = 0;
    totalrecs = 0;
    /* Loop over the input file */
    while ((retcode = ms_readmsr(&msr, "myfile.mseed", -1, NULL, NULL, 1, 1, verbose)) == MS_NOERROR)
    {
        totalrecs++;
        totalsamps += msr->samplecnt;

        msr_print (msr, 1);
        if (msr->numsamples)
        {
            printf("numsamples = %d, data[0] = %X, data[last] = %X, data[rand()] = %X\n", msr->numsamples, ((int*)msr->datasamples)[0], ((int*)msr->datasamples)[msr->numsamples-1], ((int*)msr->datasamples)[rand() % msr->numsamples-1]);
        }
    }

    if (retcode != MS_ENDOFFILE)
        ms_log (2, "Cannot read %s: %s\n", "myfile.mseed", ms_errorstr(retcode));

    /* Make sure everything is cleaned up */
    ms_readmsr(&msr, NULL, 0, NULL, NULL, 0, 0, 0);

    ms_log (1, "Records: %d, Samples: %d\n", totalrecs, totalsamps);

    getchar();
    return 0;
}

/*
  MSRecord *msr = 0;

  int dataflag   = 0;
  int totalrecs  = 0;
  int totalsamps = 0;
  int retcode;
  
  /* Process given parameters (command line and parameter file) * /
  if (parameter_proc (argc, argv) < 0)
    return -1;
  
  /* Loop over the input file * /
  while ( (retcode = ms_readmsr (&msr, inputfile, reclen, NULL, NULL, 1,
				 dataflag, verbose)) == MS_NOERROR )
    {
      totalrecs++;
      totalsamps += msr->samplecnt;
      
      msr_print (msr, ppackets);
    }
  
  if ( retcode != MS_ENDOFFILE )
    ms_log (2, "Cannot read %s: %s\n", inputfile, ms_errorstr(retcode));
  
  /* Make sure everything is cleaned up * /
  ms_readmsr (&msr, NULL, 0, NULL, NULL, 0, 0, 0);
  
  if ( basicsum )
    ms_log (1, "Records: %d, Samples: %d\n", totalrecs, totalsamps);
  
  return 0;*/
 /* End of main() */
