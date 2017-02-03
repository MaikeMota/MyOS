#ifndef __MY__OS__DRIVERS__VGA_H
#define __MY__OS__DRIVERS__VGA_H

#include <common/types.h>
#include <hardwarecommunication/port.h>

namespace MyOS
{
namespace drivers
{
class VideoGraphicsArray
{
  protected:
    MyOS::hardwarecommunication::Port8Bit miscPort;
    MyOS::hardwarecommunication::Port8Bit crtcIndexPort;
    MyOS::hardwarecommunication::Port8Bit crtcDataPort;
    MyOS::hardwarecommunication::Port8Bit sequenceIndexPort;
    MyOS::hardwarecommunication::Port8Bit sequenceDataPort;
    MyOS::hardwarecommunication::Port8Bit graphicsControllerIndexPort;
    MyOS::hardwarecommunication::Port8Bit graphicsControllerDataPort;
    MyOS::hardwarecommunication::Port8Bit attributeControllerIndexPort;
    MyOS::hardwarecommunication::Port8Bit attributeControllerReadPort;
    MyOS::hardwarecommunication::Port8Bit attributeControllerWritePort;
    MyOS::hardwarecommunication::Port8Bit attributeControllerResetPort;

    void WriteRegisters(MyOS::types::uint8_t *registers);
    MyOS::types::uint8_t *GetFrameBufferSegment();

    virtual MyOS::types::uint8_t GetColorIndex(MyOS::types::uint8_t r, MyOS::types::uint8_t g, MyOS::types::uint8_t b);

  public:
    VideoGraphicsArray();
    ~VideoGraphicsArray();

    virtual bool SupportsMode(MyOS::types::uint32_t width, MyOS::types::uint32_t height, MyOS::types::uint32_t colorDepth);
    virtual bool SetMode(MyOS::types::uint32_t width, MyOS::types::uint32_t height, MyOS::types::uint32_t colorDepth);
    virtual void PutPixel(MyOS::types::uint32_t x, MyOS::types::uint32_t y, MyOS::types::uint8_t colorIndex);
    virtual void PutPixel(MyOS::types::uint32_t x, MyOS::types::uint32_t y, MyOS::types::uint8_t r, MyOS::types::uint8_t g, MyOS::types::uint8_t b);
};
}
}

#endif