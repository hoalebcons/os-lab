#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/time.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/time64.h>
#include <linux/dirent.h>
#include <linux/string.h>
struct folder_info {
	char name[128];
	mode_t permission;
	uid_t uid;
	gid_t gid;
	long long size;
	long long atime;
};
struct folder_stat {
	long studentID;
	struct folder_info folder;
	struct folder_info parent_folder;
	struct folder_info last_access_child_folder;
};
SYSCALL_DEFINE2(get_folder_stat, char*, path, struct folder_stat*, stat)
{
	char* kpath;
	struct folder_stat* info;
	kpath = kmalloc(PATH_MAX, GFP_KERNEL);
	info = kmalloc(sizeof(struct folder_stat), GFP_KERNEL);
	copy_from_user(info, (struct folder_stat*)stat,
		sizeof(struct folder_stat));
	(*info).studentID = (long)1710162;
	int get_path = strncpy_from_user(kpath, path, 255);
	if (get_path > 0) {
		printk("path: %s\n", kpath);
	}
	struct folder_info folder;
	struct folder_info parent_folder;
	struct folder_info last_access_child_folder;
	struct file* current_fi;
	current_fi = kmalloc(sizeof(struct file), GFP_KERNEL);
	current_fi =
		filp_open(kpath, O_RDONLY, 0);
	if (IS_ERR(current_fi)) {
		printk("%ld\n", PTR_ERR(current_fi));
		return ENOENT;
	}
	struct inode* current_fi_inode;
	current_fi_inode = kmalloc(sizeof(struct inode), GFP_KERNEL);
	current_fi_inode = current_fi->f_inode;
	struct dentry* current_fi_dentry;
	current_fi_dentry = current_fi->f_path.dentry;
	strlcpy(folder.name, current_fi_dentry->d_name.name, 255);
	struct dentry* current_dentry;
	// INIT AND ASSIGN SOME VALUES
	folder.size = 0;
	folder.permission = current_fi_inode->i_mode;
	folder.gid = current_fi_inode->i_gid.val;
	folder.uid = current_fi_inode->i_uid.val;
	parent_folder.size = 0;
	time64_t newest_access;
	newest_access = 0;
	// LOOP IN CURRENT FOLDER
	list_for_each_entry(current_dentry, &current_fi_dentry->d_subdirs,
		d_child) {
		if (current_dentry == NULL) {
			printk(KERN_INFO "EXITING");
			break;
		}
		if (strlen(current_dentry->d_iname) == 0) {
			continue;
		}
		if (current_dentry->d_inode == NULL) {
			continue;
		}
		// CHECK IF A FILE IS A FOLDER
		if (S_ISDIR(current_dentry->d_inode->i_mode)) {
			struct dentry* sub_dir;
			time64_t current_access =
				current_dentry->d_inode->i_atime.tv_sec;
			// IF THE LAST_ACCESS TIME IN STRUCT < CURRENT FOLDER, REASSIGN
			if (current_access > newest_access) {
				newest_access = current_access;
				// ASSIGN NEW LASTES CHILD FOLDER
				last_access_child_folder.atime = current_access;
				last_access_child_folder.gid =
					current_dentry->d_inode->i_gid.val;
				last_access_child_folder.uid =
					current_dentry->d_inode->i_uid.val;
				strlcpy(last_access_child_folder.name,
					current_dentry->d_name.name, 255);
				last_access_child_folder.permission =
					current_dentry->d_inode->i_mode;
				last_access_child_folder.size = 0;
			}
			// LOOP IN CHILD FOLDER FOR SIZE
			list_for_each_entry(
				sub_dir, &current_dentry->d_subdirs, d_child) {
				if (strlen(sub_dir->d_iname) == 0) {
					continue;
				}
				if (sub_dir->d_inode == NULL) {
					continue;
				}
				if (sub_dir->d_inode->i_size > 0) {
					last_access_child_folder.size =
						last_access_child_folder.size +
						sub_dir->d_inode->i_size;
				}
			}
		}
		if (current_dentry->d_inode->i_size < 0) {
		}
		else {
			// INCREMENT WITH THE CURRENT FILE IN LOOP
			folder.size =
				folder.size + current_dentry->d_inode->i_size;
		}
	}
	// PRINT IN KERNEL CURRENT FOLDER INFO
	printk(KERN_INFO "CURRENT INFORMATION");
	printk(KERN_INFO "CURRENT NAME: %s \n", folder.name);
	printk(KERN_INFO "CURRENT SIZE: %lld \n", folder.size);
	printk("CURRENT GID: %u", folder.gid);
	printk("CURRENT UID: %lu", (unsigned long int)folder.uid);
	printk("CURRENT ATime: %lld", folder.atime);
	printk("CURRENT PERMISSION: %d", folder.permission);
	// PRINT IN KERNEL CHILD FOLDER INFO
	printk(KERN_INFO "CHILD INFORMATION");
	printk(KERN_INFO "CHILD NAME: %s \n", last_access_child_folder.name);
	printk(KERN_INFO "CHILD SIZE: %lld \n", last_access_child_folder.size);
	printk("CHILD GID: %u", last_access_child_folder.gid);
	printk("CHILD UID: %lu",
		(unsigned long int)last_access_child_folder.uid);
	printk("CHILD ATime: %lld", last_access_child_folder.atime);
	printk("CHILD PERMISSION: %d", last_access_child_folder.permission);
	printk(KERN_INFO "PARENT INFORMATION");
	// LOOPING FILES IN PARENT FOR SIZE
	list_for_each_entry(current_dentry, &current_fi_dentry->d_child,
		d_child) {
		if (current_dentry == NULL) {
			printk(KERN_INFO "EXITING");
			break;
		}
		if (strlen(current_dentry->d_iname) == 0) {
			continue;
		}
		if (current_dentry->d_name.name == NULL) {
			continue;
		}
		if (current_dentry->d_inode == NULL) {
			continue;
		}
		if (current_dentry->d_inode->i_size > 0) {
			parent_folder.size = parent_folder.size +
				current_dentry->d_inode->i_size;
		}
	}
	// ASSIGN PARENT INFO
	strlcpy(parent_folder.name, current_fi_dentry->d_parent->d_name.name,
		255);
	parent_folder.atime =
		current_fi_dentry->d_parent->d_inode->i_atime.tv_sec;
	parent_folder.gid = current_fi_dentry->d_parent->d_inode->i_gid.val;
	parent_folder.uid = current_fi_dentry->d_parent->d_inode->i_uid.val;
	parent_folder.permission = current_fi_dentry->d_parent->d_inode->i_mode;
	// PRINT IN KERNEL PARENT FOLDER INFO
	printk(KERN_INFO "PARENT NAME: %s \n", parent_folder.name);
	printk(KERN_INFO "PARENT SIZE: %lld \n", parent_folder.size);
	printk("PARENT GID: %u", parent_folder.gid);
	printk("PARENT UID: %lu", (unsigned long int)parent_folder.uid);
	printk("PARENT ATime: %lld", parent_folder.atime);
	printk("PARENT PERMISSION: %d", parent_folder.permission);
	// CLOSE OPENED PATH
	filp_close(current_fi, 0);
	// ASSIGNED STRUCT TO BIGGER STRUCT
	info->folder = folder;
	info->last_access_child_folder = last_access_child_folder;
	info->parent_folder = parent_folder;
	// COPY TO USER SPACE
	copy_to_user(stat, info, sizeof(struct folder_stat));
	copy_to_user(&stat->parent_folder, &info->parent_folder,
		sizeof(struct folder_info));
	copy_to_user(&stat->folder, &info->folder, sizeof(struct folder_info));
	copy_to_user(&stat->last_access_child_folder,
		&info->last_access_child_folder,
		sizeof(struct folder_info));
	// kfree(kpath);
	kfree(info);
	// kfree(ks);
	return 0;
}
