#include "MSeedSink.h"
#include "MSeedWriter.h"
#include "FileBinaryStream.h"

core::MSeedSink::MSeedSink(QString fileName)
{
    auto stream = std::make_shared<FileBinaryStream>(fileName, true);
    _writer = std::make_shared<MSeedWriter>(stream);
    _writer->verbose(MSeedPackVerbose::None);
}

bool core::MSeedSink::write(IntegerMSeedRecord::SharedPtr_t sampleRange)
{
    return _writer->write(sampleRange);
}

bool core::MSeedSink::flush()
{
    return _writer->flush();
}
