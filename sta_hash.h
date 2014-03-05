/* station info implemented with hash table
 */
#ifndef _STA_HASH_H_
#define _STA_HASH_H_

typedef unsigned char 	u8;
typedef unsigned short 	u16;
typedef unsigned int 	u32;
typedef unsigned long long 	u64;

#define MAC_ADDR_LEN 	6

struct sta_info
{
	struct sta_info *next; /* next entry in sta singly list */
	struct sta_info *hnext; /* next entry in sta hash table */
	u8	addr[MAC_ADDR_LEN];
	/* more sta info here ... */
}sta_info_t;

struct hostapd_data
{
	int num_sta; /* number of sta */
	stuct sta_info *sta_list; /* sta info list head */
#define STA_HASH_SIZE 256;
#define STA_HASH(sta) (sta[5]) /* hash key to index */
	struct sta_info *sta_hash[STA_HASH_SIZE]; /*sta hash table*/
}hostapd_data_t;


#endif /* _STA_HASH_H_ */