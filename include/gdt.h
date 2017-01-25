#ifndef __MYOS__GDT_H
#define __MYOS__GDT_H

#include <common/types.h>

namespace MyOS
{
class GlobalDescriptorTable
{
public:
  class SegmentDescriptor
  {
  private:
    MyOS::types::uint16_t limit_lo;
    MyOS::types::uint16_t base_lo;
    MyOS::types::uint8_t base_hi;
    MyOS::types::uint8_t type;
    MyOS::types::uint8_t limit_hi;
    MyOS::types::uint8_t base_vhi;

  public:
    SegmentDescriptor(MyOS::types::uint32_t base, MyOS::types::uint32_t limit, MyOS::types::uint8_t type);
    MyOS::types::uint32_t Base();
    MyOS::types::uint32_t Limit();
  } __attribute__((packed));

private:
  SegmentDescriptor nullSegmentSelector;
  SegmentDescriptor unusedSegmentSelector;
  SegmentDescriptor codeSegmentSelector;
  SegmentDescriptor dataSegmentSelector;

public:
  GlobalDescriptorTable();
  ~GlobalDescriptorTable();

  MyOS::types::uint16_t CodeSegmentSelector();
  MyOS::types::uint16_t DataSegmentSelector();
};
}
#endif