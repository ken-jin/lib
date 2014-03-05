/* station info hash table API
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "sta_hash.h"

/* get hash element */
sta_info_t * ap_get_sta(hostapd_data_t *hapd, const u8 *sta_addr)
{
	sta_info_t *s;

	s = hapd->sta_hash[STA_HASH(sta_addr)];
	while (s != NULL && memcmp(sta_addr, s->addr, MAC_ARRD_LEN) != 0)
		s = s->hnext;
	return s;
}

/* add a sta_info_t to hash table
 * insert the new added sta_info into the first of each hash bucket
 */
void ap_sta_hash_add(hostapd_data_t *hapd, sta_info_t *sta)
{
	sta->hnext = hapd->sta_hash[STA_HASH(sta->addr)];
	hapd->sta_hash[STA_HASH(sta->addr)] = sta;
}

/* delete a sta_info_t from hash table */
void ap_sta_hash_del(hostapd_data_t *hapd, sta_info_t *sta)
{
	sta_info_t *s;

	s = hapd->sta_hash[STA_HASH(sta->addr)];

	/* if sta is the first elem in hash bucket */
	if (memcmp(sta->addr, s->addr, MAC_ARRD_LEN) == 0)
	{
		hapd->sta_hash[STA_HASH(sta->addr)] = sta->hnext;
	}
	/* sta not the first one */
	while (s->hnext != NULL && memcmp(s->hnext->addr, sta->addr, MAC_ARRD_LEN) != 0)
		s = s->hnext;

	if (s->hnext != NULL)
		s->hnext = s->hnext->hnext;
	else
		fprintf(stderr, "not found in hash talbe\n");
}

/* delete a sta_info_t from list head: hostapd_data_t->sta_list */
void ap_sta_list_del(hostapd_data_t *hapd, sta_info_t *sta)
{
	sta_info_t *s;
	/* sta is the list head */
	if (hapd->sta_list == sta)
	{
		hapd->sta_list = sta->next;
		return;
	}
	/* not list head */
	s = hapd->sta_list;
	while (s != NULL && s->next != sta)
		s = s->next;

	if (s == NULL)
	{
		fprintf(stderr, "not found in the list\n");
		return;
	}
	else
		s->next = s->next->next;
	
}

/* add a new sta_info with specified sta mac address to hostapd_data_t */
sta_info_t ap_sta_add(hostapd_data_t *hapd, const u8 *sta_addr)
{
	sta_info_t *sta;

	sta = (sta_info_t) malloc(sizeof(*sta));
	if (sta == NULL)
	{
		fprintf(stderr, "%s_%d:malloc error\n", __FUNCTION__, __LINE__);
		return NULL
	}

	/* initialize sta info struct */
	memcpy(sta_addr, sta->addr, sizeof(*sta));
	/* add the new sta to list head */
	sta->next = hapd->sta_info;
	hapd->sta_info = sta;

	ap_sta_hash_add(hapd, sta);
	return sta;
}


/* free a sta_info_t */
vod ap_free_sta(hostapd_data_t *hapd, sta_info_t *sta)
{
	assert(sta == NULL);

	ap_sta_list_del(hapd, sta);
	ap_sta_hash_del(hapd, sta);
	/* free more malloc data in sta if has */
	free(sta);
}
