#include "MSeedReader.h"
#include <libmseed.h>

namespace core
{
    MSeedReader::MSeedReader(QString fileName): _fileName(fileName)
    {
    }

    QList<AbstractMSeedRecord::SharedPtr_t> MSeedReader::readAll(bool *success)
    {
        QList<AbstractMSeedRecord::SharedPtr_t> records;
        MSRecord *msr = NULL;
        int retcode;

        while ((retcode = ms_readmsr(&msr, _fileName.toLatin1().data(), 0, NULL, NULL, 1, 1, _verbose)) == MS_NOERROR)
        {
            AbstractMSeedRecord::SharedPtr_t record;
            switch (msr->sampletype)
            {
            case 'i':
                {   
                    auto iRecord = std::make_shared<IntegerMSeedRecord>();
                    iRecord->data().resize(msr->samplecnt);
                    memcpy(iRecord->data().data(), msr->datasamples, msr->samplecnt * 4);
                    record = iRecord;
                }
                break;
            default:
                throw Common::Exception("Sample type is not supported.");
            }

            record->channelName(msr->channel);
            record->location(msr->location);
            record->network(msr->network);
            record->station(msr->station);
            record->samplingRateHz(msr->samprate);
            char isotime[30];
            ms_hptime2isotimestr(msr->starttime, isotime, 1);
            auto tmp = QDateTime::fromString(isotime, Qt::ISODate);
            auto startTime = QDateTime(tmp.date(), tmp.time(), Qt::UTC);
            record->startTime(startTime);

            records.push_back(record);

            /* Do something with the record here, e.g. print */
            msr_print(msr, _verbose);
        }

        if (retcode != MS_ENDOFFILE)
        {
            if (success != nullptr)
            {
                *success = false;
            }
            ms_log(2, "Cannot read %s: %s\n", _fileName.toLatin1().data(), ms_errorstr(retcode));
        }
        else
        {
            if (success != nullptr)
            {
                *success = true;
            }
        }

        /* Cleanup memory and close file */
        ms_readmsr(&msr, NULL, 0, NULL, NULL, 0, 0, _verbose);

        return records;
    }
}
