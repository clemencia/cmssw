/****************************************************************************
*
* This is a part of the TOTEM offline software.
* Authors: 
*   Jan Kašpar (jan.kaspar@gmail.com) 
*    
****************************************************************************/

#ifndef DataFormats_TotemRawData_TotemRawEvent
#define DataFormats_TotemRawData_TotemRawEvent

#include <ctime>
#include <map>

/**
 * Encapsulates meta-data (non-VFAT data) of TOTEM raw event.
**/
class TotemRawEvent
{
  public:  
    TotemRawEvent() {}

    ~TotemRawEvent() {}

    ///Additional data provided in OptoRx headers and footers.
    struct OptoRxMetaData
    {
      unsigned int BX, LV1;
    };

    /// Trigger data provided by LoneG.
    struct TriggerData
    {
      unsigned char type;
      unsigned int event_num, bunch_num, src_id;
      unsigned int orbit_num;
      unsigned char revision_num;
      unsigned int run_num, trigger_num, inhibited_triggers_num, input_status_bits;
    };

  public:
    /// number of event in the datafile (0xb tag)
    unsigned long dataEventNumber;
   
    /// number of configuration in the datafile (0xb tag)
    unsigned long dataConfNumber;
    
    /// timestamp
    time_t timestamp;

    /// additional data from OptoRx frames, indexed by OptoRxId
    std::map<unsigned int, OptoRxMetaData> optoRxMetaData;

    /// trigger (LoneG) data
    TriggerData triggerData;

    /// map: LDC id -> LDC timestamp
    std::map<unsigned int, time_t> ldcTimeStamps;
};

#endif
