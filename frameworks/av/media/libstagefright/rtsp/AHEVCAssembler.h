/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef A_HEVC_ASSEMBLER_H_

#define A_HEVC_ASSEMBLER_H_

#include "ARTPAssembler.h"

#include <utils/List.h>
#include <utils/RefBase.h>

namespace android {

struct ABuffer;
struct AMessage;

struct AHEVCAssembler : public ARTPAssembler {
    AHEVCAssembler(const sp<AMessage> &notify);

    typedef List<sp<ABuffer> > Queue;

protected:
    virtual ~AHEVCAssembler();

    virtual AssemblyStatus assembleMore(const sp<ARTPSource> &source);
    virtual void onByeReceived();
    virtual void packetLost();

private:
    sp<AMessage> mNotifyMsg;

    uint32_t mAccessUnitRTPTime;
    bool mNextExpectedSeqNoValid;
    uint32_t mNextExpectedSeqNo;
    bool mAccessUnitDamaged;
    bool mFirstIFrameProvided;
    uint64_t mLastIFrameProvidedAtMs;
    int32_t mWidth;
    int32_t mHeight;
    List<sp<ABuffer> > mNALUnits;

    int32_t addNack(const sp<ARTPSource> &source);
    void checkSpsUpdated(const sp<ABuffer> &buffer);
    void checkIFrameProvided(const sp<ABuffer> &buffer);
    bool dropFramesUntilIframe(const sp<ABuffer> &buffer);
    AssemblyStatus addNALUnit(const sp<ARTPSource> &source);
    void addSingleNALUnit(const sp<ABuffer> &buffer);
    AssemblyStatus addFragmentedNALUnit(List<sp<ABuffer> > *queue);
    bool addSingleTimeAggregationPacket(const sp<ABuffer> &buffer);

    void submitAccessUnit();

    inline int64_t findRTPTime(const uint32_t& firstRTPTime, const sp<ABuffer>& buffer);
    int32_t pickProperSeq(const Queue *q, uint32_t first, int64_t play, int64_t jit);
    bool recycleUnit(uint32_t start, uint32_t end, uint32_t connected,
             size_t avail, float goodRatio);
    int32_t deleteUnitUnderSeq(Queue *queue, uint32_t seq);
    void printNowTimeUs(int64_t start, int64_t now, int64_t play);
    void printRTPTime(int64_t rtp, int64_t play, int64_t exp, bool isExp);

    DISALLOW_EVIL_CONSTRUCTORS(AHEVCAssembler);
};

}  // namespace android

#endif  // A_HEVC_ASSEMBLER_H_
