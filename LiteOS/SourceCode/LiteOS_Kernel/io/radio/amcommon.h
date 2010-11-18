#ifndef AMH
#define AMH
#include "../../types/types.h"

/** \defgroup radio Radio related definitions 
These definitons collectively form radio related API. 
*/

/** @{*/

/** This enum defines the broadcast address */
enum
{
    BCAST_ADDRESS = 0xffff,
};

/** The payload length is by default 100 bytes */
#ifndef PAYLOAD_LENGTH
#define PAYLOAD_LENGTH 100
#endif
/* Return the relative address of member in a type, which is usually a struct  */
#define offsetof_radio(type, member) ((size_t) &((type *)0)->member)

/** The message format is specified by the standard 802.15.4 by IEEE (version 2003). */
typedef struct Radio_Msg
{
    ///length is not part of the header according to the format 
    uint8_t length;
    ///802.15.4 definitions 
    uint8_t fcfhi;
    uint8_t fcflo;
    uint8_t dsn;
    uint16_t destpan;
    uint16_t addr;
    ///frame payload 
    //  uint8_t type;
    //  uint8_t group;
    uint16_t port;
    int8_t data[PAYLOAD_LENGTH];
    ///remaining part of the radio message
    uint8_t strength;
    uint8_t lqi;
    bool crc;
    uint8_t ack;
    uint16_t time;
} Radio_Msg;

/* The enums for locating different parts of the mess */
enum
{
    /// size of the header NOT including the length byte
    MSG_HEADER_SIZE = offsetof_radio(struct Radio_Msg, data) - 1,
    //(2+1+2+2)+(LITEeader=5)=11
    /// size of the footer
    MSG_FOOTER_SIZE = 2,
    /// size of the full packet-including length byte
    MSG_DATA_SIZE = offsetof_radio(struct Radio_Msg, strength) + sizeof(uint16_t),
    //1+7+5+29+1+2
    /// size of the data length
    DATA_LENGTH = PAYLOAD_LENGTH,
    /// position of the length byte
    LENGTH_BYTE_NUMBER = offsetof_radio(struct Radio_Msg, length) + 1,
    /// size of MAC Header 
    LITEOS_HEADER_SIZE = 5,     //LITEheader=addr(2)+type(1)+groupid(1)+length(1)
};

/** The readio messge pointer  */
typedef Radio_Msg *Radio_MsgPtr;

/** @}*/
#endif
