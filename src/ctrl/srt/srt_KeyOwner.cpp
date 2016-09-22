#include "cmnd/BasicCommands.h"
#include "core/SRTExpans.h"
#include "core/ObjectNodeUtil.h"
#include "ctrl/srt/srt_KeyOwner.h"

using namespace core;

namespace ctrl {
namespace srt {

void KeyOwner::pushOwnsKey(cmnd::Stack& aStack, TimeLine& aLine, int aFrame)
{
    if (ownsKey)
    {
        aStack.push(new cmnd::GrabNewObject<SRTKey>(key));
        aStack.push(aLine.createPusher(TimeKeyType_SRT, aFrame, key));
        ownsKey = false;
    }
}

void KeyOwner::deleteOwnsKey()
{
    if (key && ownsKey)
    {
        delete key;
    }
    ownsKey = false;
    key = nullptr;
}

bool KeyOwner::updatePosture(const SRTExpans& aExpans)
{
    XC_PTR_ASSERT(key);

    if (ownsKey)
    {
        key->data() = aExpans.data();
    }

    mtx = aExpans.parentMatrix();
    invMtx = mtx.inverted(&hasInv);

    if (!hasInv)
    {
        return false;
    }

    invSRMtx = invMtx;
    invSRMtx.setColumn(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));

    ltLocMtx = key->data().localMatrix();
    ltLocMtx.translate(-ObjectNodeUtil::getCenterOffset3D(aExpans));

    return true;
}

} // namespace srt
} // namespace ctrl