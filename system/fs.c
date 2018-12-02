#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


#ifdef FS
#include <fs.h>

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;


#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* YOUR CODE GOES HERE */

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}

/* read in an inode and fill in the pointer */
int
fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int
fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;
  
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs memget failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void
fs_print_fsd(void) {

  printf("fsd.ninodes: %d\n", fsd.ninodes);
  printf("sizeof(struct inode): %d\n", sizeof(struct inode));
  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);

  printf("sizeof(struct dirent):   %d\n", sizeof(struct dirent));
  printf("sizeof(struct director): %d\n", sizeof(struct directory));
  printf("sizeof(struct fsystem):  %d\n\n", sizeof(struct fsystem));
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}


int fs_open(char *filename, int flags) {
  printf("========== start of fs_open ==========\n");

  // check length of filename
  int len = strlen(filename) + 1;
  if (len > FILENAMELEN) {
    printf("filename too long!\n");
    return SYSERR;
  }

  // check flags
  if (flags != O_RDONLY && flags != 0_WRONLY && flags != 0_RDWR) {
    printf("Unsupported flag!\n");
    return SYSERR;
  }

  // check file name in directory
  struct directory *rootDir = &fsd.root_dir;
  int numEntr = rootDir->numentries;
  struct dirent *entrPtr;
  char *namePtr;
  int rval;
  int i;
  for (i = 0; i < numEntr; i++) {
    entrPtr = &rootDir->entry[i];
    namePtr = &entrPtr->name[0];
    rval = strncmp(namePtr, filename, len);
    if (rval == 0) {
      break;
    }
  }

  // file not exists
  if (i == numEntr) {
    printf("file not exists: %s\n", filename);
    return SYSERR;
  }

  // get directory entry
  entrPtr = &rootDir->entry[i];

  // number of files exceeds maximum
  if (next_open_fd >= NUM_FD) {
    printf("number of opened files exceeds maximum!\n");
    return SYSERR;
  }

  // get file number
  int fileNum = next_open_fd;
  next_open_fd++;

  // get inode number
  entrPtr = &rootDir->entry[i];
  int inodeNum = entrPtr->inode_num;

  // get inode
  struct inode *inodePtr;
  inodePtr = (struct inode *)getmem(sizeof(struct inode));
  rval = fs_get_inode_by_num(dev0, inodeNum, inodePtr);
  if (rval == (int)SYSERR) {
    printf("fs_get_indode_by_num failed!\n");
    return SYSERR;
  }

  // read only is not allowed on empty file
  if (inodePtr->size == 0 && flags == 0_RDONLY) {
    printf("read only not allowed on empty file!\n");
    return SYSERR;
  }

  // if Write only set file pointer to the end
  int filePtr = 0;
  if (indodePtr->size != 0 && flags == 0_WRONLY) {
    filePtr = inodePtr->size;
  }

  // set data on file table
  struct filetable *fileTab = &oft[fileNum];
  fileTab->state = FSTATE_OPEN;
  fileTab->fileptr = filePtr;
  fileTab->de = entrPtr;
  struct inode *in;
  in = &fileTab->in;
  memcpy(in, inodePtr, sizeof(struct inode));

  // // find free block
  // int nBlocks = fsd.nblocks;
  // i = FIRST_INODE_BLOCK + NUM_INODE_BLOCKS;
  // if (inodePtr->size == 0) {
  //   for (; i < nBlocks; i++) {
  //     rval = fs_getmaskbit(i);
  //     if (rval == 0) {
  //       break;
  //     }
  //   }

  //   // no block is empty
  //   if (i == nBlocks) {
  //     printf("no block is empty!\n");
  //     return SYSERR;
  //   }

  //   // set bit mask of that block
  //   fs_setmaskbit(i);

  //   // 
  //   inodePtr->blocks
  // }

  printf("========== end of fs_open ==========\n");
  return SYSERR;
}

int fs_close(int fd) {
  return SYSERR;
}

int fs_create(char *filename, int mode) {
  // get length of filename
  int len = strlen(filename) + 1;
  if (len > FILENAMELEN) {
    printf("filename too long!\n");
    return SYSERR;
  }

  if (mode != O_CREAT) {
    printf("Unsupported mode!\n");
    return SYSERR;
  }

  // check file name in directory
  struct directory *rootDir = &fsd.root_dir;
  int numEntr = rootDir->numentries;
  struct dirent *entrPtr;
  char *namePtr;
  int rval;
  int i;
  for (i = 0; i < numEntr; i++) {
    entrPtr = &rootDir->entry[i];
    namePtr = &entrPtr->name[0];
    rval = strncmp(namePtr, filename, len);
    if (rval == 0) {
      printf("filename already exists!\n");
      return SYSERR;
    }
  }

  // check number of directory entries
  if (numEntr >= DIRECTORY_SIZE) {
    printf("number of dir entries exceeds maximum!\n");
    return SYSERR;
  }

  // create inode
  struct inode *fileInode;
  fileInode = (struct inode *)getmem(sizeof(struct inode));
  if (fileInode == (void *)SYSERR) {
    printf("inode getmem fialed!\n");
    return SYSERR;
  }
  
  // set inode data
  int id = fsd.inodes_used;
  fsd.inodes_used++;
  fileInode->id = id;
  fileInode->type = INODE_TYPE_FILE;
  fileInode->device = dev0;
  fileInode->size = 0;

  // get inode and fill it
  rval = fs_put_inode_by_num(dev0, id, fileInode);

  if (rval == (int)SYSERR) {
    printf("fs_put_indode_by_num failed!\n");
    return SYSERR;
  }

  // create directory entry
  struct dirent *dirEntry;
  dirEntry = (struct dirent *)getmem(sizeof(struct dirent));
  if (dirEntry == (void *)SYSERR) {
    printf("dirent getmem failed!\n");
    return SYSERR;
  }

  // set data in directory entry
  dirEntry->inode_num = id;
  namePtr = &dirEntry->name[0];
  strncpy(namePtr, filename, len);

  // set directory entry to root_dir
  entrPtr = &rootDir->entry[numEntr];
  memcpy(entrPtr, dirEntry, sizeof(struct dirent));
  rootDir->numentries++;

  // open the created file
  rval = fs_open(filename, FSTATE_OPEN);
  if (rval == (int)SYSERR) {
    printf("fs_open failed!\n");
    return SYSERR;
  }

  return OK;
}

int fs_seek(int fd, int offset) {
  return SYSERR;
}

int fs_read(int fd, void *buf, int nbytes) {
  return SYSERR;
}

int fs_write(int fd, void *buf, int nbytes) {
  return SYSERR;
}

#endif /* FS */
