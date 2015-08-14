#include "MSeedReader.h"
#include <libmseed.h>

namespace core
{
    AbstractSampleRange::SharedPtr_t MSeedReader::read()
    {
        MSRecord *msr = NULL;
        int retcode;

        while ((retcode = ms_readmsr(&msr, _fileName.toLatin1().data(), 0, NULL, NULL, 1, 0, _verbose)) == MS_NOERROR)
        {
            /* Do something with the record here, e.g. print */
            msr_print(msr, _verbose);
        }

        if (retcode != MS_ENDOFFILE)
        {
            ms_log(2, "Cannot read %s: %s\n", _fileName.toLatin1().data(), ms_errorstr(retcode));
        }

        /* Cleanup memory and close file */
        ms_readmsr(&msr, NULL, 0, NULL, NULL, 0, 0, _verbose);

        return AbstractSampleRange::SharedPtr_t();
    }
}
