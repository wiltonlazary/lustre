#ifndef _OBD_EXT2
#define _OBD_EXT2

#define OBD_EXT2_RUNIT           _IOWR('f', 61, long)

#include <linux/iobuf.h>

#ifndef OBD_EXT2_DEVICENAME
#define OBD_EXT2_DEVICENAME "obdext2"
#endif

struct ext2_obd {
	struct super_block * ext2_sb;
};


/* development definitions */
extern struct obdfs_sb_info *obd_sbi;
extern struct file_operations *obd_fso;

/* ext2_obd.c */
extern struct obd_ops ext2_obd_ops;

/* balloc.c */
int ext2_new_block (const struct inode * inode, unsigned long goal,
                   u32 * prealloc_count, u32 * prealloc_block, int * err);
void ext2_free_blocks (const struct inode * inode, unsigned long block,
                      unsigned long count);
unsigned long ext2_count_free_blocks (struct super_block * sb);
int ext2_group_sparse(int group);
struct ext2_group_desc * ext2_get_group_desc(struct super_block * sb,
					     unsigned int block_group,
					     struct buffer_head ** bh);

/* bitmap.c */
unsigned long ext2_count_free(struct buffer_head * map, unsigned int numchars);

/* dir.c */
extern struct inode_operations ext2_dir_inode_operations;

/* file.c */
extern struct file_operations ext2_file_operations;
extern struct inode_operations ext2_file_inode_operations;

/* fsync.c */
int ext2_sync_file(struct file * file, struct dentry *dentry);

/* ialloc.c */
void ext2_free_inode (struct inode * inode);
struct inode * ext2_new_inode (const struct inode * dir, int mode,
				     int * err);
unsigned long ext2_count_free_inodes (struct super_block * sb);
void ext2_check_inodes_bitmap (struct super_block * sb);
int load_inode_bitmap (struct super_block * sb,
			      unsigned int block_group);

/* inode.c */
inline long ext2_block_map (struct inode * inode, long block);


/* super.c */
#ifdef EXT2_OBD_DEBUG
#  undef ext2_debug
#  define ext2_debug(format, a...) CDEBUG(D_EXT2, format, ## a)
#  define ext2_error ext2_warning
#  define ext2_panic ext2_warning
#  define ext2_warning(sb, func, format, a...) CDEBUG(D_WARNING, format, ## a)
#else
#  undef ext2_debug
#  define ext2_debug(format, a...) {}
#  define ext2_error(sb, func, format, a...) printk(KERN_ERR "%s: " format, func, ## a)
#  define ext2_panic(sb, func, format, a...) printk(KERN_CRIT "%s: " format, func, ## a)
#  define ext2_warning(sb, func, format, a...) printk(KERN_WARNING "%s: " format, func, ## a)
#endif

extern struct super_operations ext2_sops;
int obd_remount (struct super_block * sb, int * flags, char * data);
struct super_block * ext2_read_super (struct super_block * sb, void * data,
				      int silent);

/* punch.c */
void ext2_truncate (struct inode * inode);
int ext2_punch (struct inode * inode, loff_t start, size_t count);

static inline struct page *addr_to_page(char *buf)
{
	unsigned long addr = (unsigned long)buf;
        unsigned long map_nr;

#ifdef CONFIG_DISCONTIGMEM
        if (addr == 0) return;
#endif
        map_nr = MAP_NR(addr);
        if (map_nr < max_mapnr)
		return mem_map + map_nr;
	else 
		return 0;
}

#endif
