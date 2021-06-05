#pragma once

#include <stddef.h>
#include <stdint.h>
#include <drivers/storage/dev.h>

#define FS_TYPE_FAT32 0

#define FS_SEEK_BEGIN 0
#define FS_SEEK_CURR 1
#define FS_SEEK_END 2

typedef struct fs_node
{
    void* priv;

    size_t (*read)(struct fs_node* file, void* ptr, size_t off, size_t size);
    size_t (*write)(struct fs_node* file, const void* ptr, size_t off, size_t size);

} fs_node_t;

typedef struct fs_vol
{
    fs_node_t (*find_file)(struct fs_vol* dri, fs_node_t* dir, const char* name);
    
    int type;
    void* priv;

} fs_vol_t;

typedef struct mount
{
    dev_t* dev;
    fs_vol_t fs_dri;
    char* mnt_pt;

} mount_t;

#define MOUNT_LST_MAX 100

typedef struct mount_lst
{
    mount_t* mnts[MOUNT_LST_MAX];
    uint32_t cnt;

} mount_lst_t;

extern mount_t* root_mnt_pt;
extern mount_lst_t fs_mnts;

// mount.c
int fs_get_type(dev_t* dev);
mount_t* fs_mnt_dev(dev_t* dev, const char* mnt_pt);

// vfs.c
void vfs_open(fs_node_t* node);
size_t vfs_read(fs_node_t* file, void* ptr, size_t off, size_t size);
size_t vfs_write(fs_node_t* file, const void* ptr, size_t off, size_t size);
void vfs_close(fs_node_t* file);
int vfs_seek(fs_node_t* file, uint64_t off, int whence);
fs_node_t vfs_resolve_path(const char* path, const char* working_dir);
void vfs_mk_dev_file(fs_node_t node, const char* path);