#glibExtractor#

Script to extract all files from within a GLIB file which was used in the game GALACTIX (DOS)

##GLIB File specification##

The name of a glib file is always 22 bytes long and null-terminated until its end. A file cannot be larger than 2^16-1 bytes because the filesize is stored as ashort. The maximum file size inside an archive was 64000 bytes.

###Header:###
The header stores the amount of files inside the archive. It also contains a file named "GLIB FILE" and its end is the end of the header.

First 4 bytes: Number of files

Next 22 bytes: "GLIB FILE" string 

Next 2 bytes: End of "GLIB FILE" (end of GLIB Header)

###Body:###
The body follows immediately after the header and consists of concatenated file entries.

**GLIB File:**

   FileName: char[22]
   
   Size: short
   
   EndOfFile: int

###Usage:###
Simply drag the archive onto the executable to extract all contained files
