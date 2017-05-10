# glibExtractor

A small tool to extract all files from a GLIB file archive which was used to store the assets of the game GALACTIX (MS-DOS)

## GLIB file specification#
The files inside the GLIB file are not encrypted or compressed.
The name of a file stored inside a GLIB file record always takes 22 bytes and is zero-padded until its end. A file cannot be larger than 2^16-1 bytes because the filesize is stored in 2 bytes. No file inside the archives was bigger than 64000 bytes.

`GLIB Archive = {GLIB_Header} + {GLIB_Record * recordCount} + {BinaryData}`

### Header:
```C++
// sizeof(int) == 4

struct GLIB_Header
{
   int recordCount;
   char[22] glibFile; // "GLIB FILE" string, zero-padded
   unsigned short size; // always zero (dummy file)
}
// sizeof(GLIB_Header) == 28
```
### Body:
```C++
struct GLIB_Record
{
   int binaryOffset; // global offset of the first byte for this file
   char[22] filename; // zero-padded
   unsigned short size; // file size in bytes
}
// sizeof (GLIB_Record) == 28

// binary data until end of file...
```

Note: The "GLIB FILE" from the header is probably a GLIB_Record itself, with the first field being the file count instead of the binary offset and its file size being zero (since it acts as a dummy file). One could argue what is the header and what is the body here. One could even say, the header is only the first 4 bytes. Either way the extraction algorithm stays the same.

### Usage:
Simply drag the archive onto the executable to extract all contained files.
