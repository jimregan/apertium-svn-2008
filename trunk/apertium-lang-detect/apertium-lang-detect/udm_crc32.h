#ifndef _UDM_CRC32_H
#define _UDM_CRC32_H

typedef unsigned int udmcrc32_t;

/* Returns crc32 of data block */
extern udmcrc32_t UdmCRC32(char * buf, size_t size);

/* Returns crc32 of null-terminated string */
#define UdmStrCRC32(buf) UdmCRC32((buf),strlen(buf))

#endif /* _UDM_CRC32_H */
