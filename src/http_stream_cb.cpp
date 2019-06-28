#include "http_stream_cb.h"
#include "log.h"

using namespace std;

namespace xport
{

HttpStreamCB::HttpStreamCB(const shared_ptr<IReader>& reader)
    :mReader(reader), mSize(reader->to() > 0 ? reader->to() - reader->from() + 1 : -1){
    logi("output %d=>%d ,total %d", reader->from(), reader->to(), mSize);
}

string HttpStreamCB::read(uint64_t offset){
    if (mSize > 0 && offset >= mSize) {
        mReader.reset();
        logi("read enough");
        return "";
    }

    auto buf = mReader->read();
    // logi("begin read:%d", buf.size());

    if (mSize > 0){
        auto left = mSize - offset;
        if (left < buf.size()){
            return buf.substr(0, left);
        }
    }

    return buf;
}

} // namespace xport
