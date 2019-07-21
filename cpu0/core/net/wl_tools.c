/* ------------------------ OR32 includes -------------------------------- */
#include <stdlib.h>

#include "wl_tools.h"

#include <malloc.h>

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

#ifdef __WIFI__

struct wpa_ssid * 
ewl_get_network_lists(void);

struct wpa_ssid *
ewl_get_current_network_list(void);


static inline void 
wl_print_cipher_txt(int cipher)
{
	int first = 1;
	printf("-");

	if (cipher & WPA_CIPHER_NONE) {
		printf("%sNONE", first ? "" : "+");
		first = 0;
	}

	if (cipher & WPA_CIPHER_WEP40) {
		printf("%sWEP40", first ? "" : "+");
	}

	if (cipher & WPA_CIPHER_WEP104) {
		printf("%sWEP104",first ? "" : "+");
		first = 0;
	}

	if (cipher & WPA_CIPHER_TKIP) {
		printf("%sTKIP", first ? "" : "+");
		first = 0;
	}

	if (cipher & WPA_CIPHER_CCMP) {
		printf("%sCCMP", first ? "" : "+");
		first = 0;
	}

}


static inline void 
wl_print_ie_txt(const char *proto, const u8 *ie, size_t ie_len)
{
	struct wpa_ie_data data;
	int first;

	printf("[%s-", proto);

	if (ewl_parse_wpa_ie(ie, ie_len, &data) < 0) {
		printf("?]");
		return;
	}

	first = 1;
	if (data.key_mgmt & WPA_KEY_MGMT_IEEE8021X) {
		printf("%sEAP", first ? "" : "+");
		first = 0;
	}
	
	if (data.key_mgmt & WPA_KEY_MGMT_PSK) {
		printf("%sPSK", first ? "" : "+");
		first = 0;
	}

	if (data.key_mgmt & WPA_KEY_MGMT_WPA_NONE) {
		printf("%sNone", first ? "" : "+");
		first = 0;
	}

	wl_print_cipher_txt(data.pairwise_cipher);

	printf("]");

}


void 
wl_sprintf_ie_txt(char *sbuff, const char *proto, const u8 *ie, size_t ie_len)
{
	struct wpa_ie_data data;
	int first;
    int cipher;
    char *s=sbuff;
	sprintf(s,"[%s-", proto);
    s += strlen(s);
    
	if (ewl_parse_wpa_ie(ie, ie_len, &data) < 0) {
		sprintf(s,"?]");
        s += strlen(s);
		return;
	}

	first = 1;
	if (data.key_mgmt & WPA_KEY_MGMT_IEEE8021X) {
		sprintf(s,"%sEAP", first ? "" : "+");
        s += strlen(s);
		first = 0;
	}
	
	if (data.key_mgmt & WPA_KEY_MGMT_PSK) {
		sprintf(s,"%sPSK", first ? "" : "+");
		s += strlen(s);
		first = 0;
	}

	if (data.key_mgmt & WPA_KEY_MGMT_WPA_NONE) {
		sprintf(s,"%sNone", first ? "" : "+");
		s += strlen(s);
		first = 0;
	}

    sprintf(s,"-"); s += strlen(s);
    cipher = data.pairwise_cipher;
    first = 1;

	if (cipher & WPA_CIPHER_NONE) {
		sprintf(s,"%sNONE", first ? "" : "+"); s += strlen(s);
		first = 0;
	}

	if (cipher & WPA_CIPHER_WEP40) {
		sprintf(s, "%sWEP40", first ? "" : "+"); s += strlen(s);
	}

	if (cipher & WPA_CIPHER_WEP104) {
		sprintf(s,"%sWEP104",first ? "" : "+"); s += strlen(s);
		first = 0;
	}

	if (cipher & WPA_CIPHER_TKIP) {
		sprintf(s,"%sTKIP", first ? "" : "+"); s += strlen(s);
		first = 0;
	}

	if (cipher & WPA_CIPHER_CCMP) {
		sprintf(s, "%sCCMP", first ? "" : "+"); s += strlen(s);
		first = 0;
	}

	sprintf(s,"]"); s += strlen(s);

}


/**
    print result of scan .
*/
void
wl_scan_results(int num_scan, struct wpa_scan_result *results_scan)
{
	struct wpa_scan_result *res;
	int i, ret;

	printf("bssid / frequency / signal level / flags / ssid\r\n");

	for (i = 0; i < num_scan; i++) {
		res = &results_scan[i];
		printf(MACSTR "\t%d\t%d\t",
				  MAC2STR(res->bssid), res->freq, (char)res->level);		  

		if (res->wpa_ie_len) {
			wl_print_ie_txt("WPA",
						    res->wpa_ie,
						    res->wpa_ie_len);
		}
		if (res->rsn_ie_len) {
			wl_print_ie_txt("WPA2",
						    res->rsn_ie,
						    res->rsn_ie_len);
		}
		if (!res->wpa_ie_len && !res->rsn_ie_len &&
		    res->caps & IEEE80211_CAP_PRIVACY) {
			ret = printf("[WEP]");
		}
		if (res->caps & IEEE80211_CAP_IBSS) {
			ret = printf("[IBSS]");
		}
        res->ssid[res->ssid_len] = 0;
		printf("\t%s",  res->ssid);

		printf("\r\n");
	}

}

/**
    print list of network that registed supplicant.
*/
void
wl_print_list_networks(void)
{
    struct wpa_ssid *ssid;

    ssid = ewl_get_network_lists();
    printf("network id / ssid / bssid \r\n");

    while (ssid) {
        ssid->ssid[ssid->ssid_len] = 0;
		printf("%d\t%s", ssid->id, ssid->ssid);

		if (ssid->bssid_set)
            printf("\t" MACSTR, MAC2STR(ssid->bssid));
		else 
		    printf("\tany");
		
        printf("\t%s%s", 
                ssid == ewl_get_current_network_list() ? "[CURRENT]" : "",
				ssid->disabled ? "[DISABLED]" : "");
        printf("\r\n");
		ssid = ssid->next;
	}
}

void
wl_print_apcfg(void)
{
    struct ewlap_cfg ap_cfg;
 
    if(!ewlap_get_apcfg(&ap_cfg))
    {
        printf("\r\ns/w AP cfg -------------->\r\n");
        printf("ssid    : %s \r\n", ap_cfg.ssid);
        printf("channel : %d \r\n", ap_cfg.channel);
        printf("max sta : %d \r\n", ap_cfg.max_sta_count);

        if(ap_cfg.enc_proto == 1)
            printf("security: wpa2-pask, passphrase %s \r\n", ap_cfg.passphrase);
        else
            printf("security : open \r\n");
#if 1
		printf("================================================================================\r\n");
		int i;
		printf("bcast_ssid_ctl          : 0x%02X(%d)\r\n", ap_cfg.bcast_ssid_ctl, ap_cfg.bcast_ssid_ctl);	/** Broadcast ssid control */
		printf("radio_ctl               : 0x%02X(%d)\r\n", ap_cfg.radio_ctl, ap_cfg.radio_ctl);				/** Radio control: on/off */
		printf("dtim_period             : 0x%02X(%d)\r\n", ap_cfg.dtim_period, ap_cfg.dtim_period);			/** dtim period */
		printf("beacon_period           : 0x%04X(%d)\r\n", ap_cfg.beacon_period, ap_cfg.beacon_period);		/** beacon period */

		for(i=0;i<14;i++)
		printf("rates[%02d]               : 0x%02X(%d)\r\n", i, ap_cfg.rates[i], ap_cfg.rates[i]);			/** rates */

		printf("tx_data_rate            : 0x%04X(%d)\r\n", ap_cfg.tx_data_rate, ap_cfg.tx_data_rate);		/** Tx data rate */
		printf("mcbc_data_rate          : 0x%04X(%d)\r\n", ap_cfg.mcbc_data_rate, ap_cfg.mcbc_data_rate);	/** multicast/broadcast data rate */
		printf("tx_power_level          : 0x%02X(%d)\r\n", ap_cfg.tx_power_level, ap_cfg.tx_power_level);	/** Tx power level in dBm */
		printf("tx_antenna              : 0x%02X(%d)\r\n", ap_cfg.tx_antenna, ap_cfg.tx_antenna);			/** Tx antenna */
		printf("rx_antenna              : 0x%02X(%d)\r\n", ap_cfg.rx_antenna, ap_cfg.rx_antenna);			/** Rx antenna */
		printf("pkt_forward_ctl         : 0x%02X(%d)\r\n", ap_cfg.pkt_forward_ctl, ap_cfg.pkt_forward_ctl);	/** packet forward control */
	
		/** mac filter */
		printf("filter_mode             : 0x%04X(%d)\r\n", ap_cfg.filter_mode, ap_cfg.filter_mode);			/** mac filter mode */
		printf("mac_count               : 0x%04X(%d)\r\n", ap_cfg.mac_count, ap_cfg.mac_count);				/** mac adress count */
		for(i=0;i<16;i++) {
		printf("mac_list[%02d]            : %02X-%02X-%02X-%02X-%02X-%02X\r\n", i, 
			ap_cfg.mac_list[i][0], ap_cfg.mac_list[i][1], ap_cfg.mac_list[i][2], 
			ap_cfg.mac_list[i][3], ap_cfg.mac_list[i][4], ap_cfg.mac_list[i][5]);	/** mac address list */
		}

		printf("sta_ageout_timer        : 0x%08X(%d)\r\n", ap_cfg.sta_ageout_timer, ap_cfg.sta_ageout_timer);/** station ageout timer in unit of 100ms  */
		printf("ps_sta_ageout_timer     : 0x%08X(%d)\r\n", ap_cfg.ps_sta_ageout_timer, ap_cfg.ps_sta_ageout_timer);	/** PS station ageout timer in unit of 100ms	*/
		printf("rts_threshold           : 0x%04X(%d)\r\n", ap_cfg.rts_threshold, ap_cfg.rts_threshold);		/** RTS threshold */
		printf("frag_threshold          : 0x%04X(%d)\r\n", ap_cfg.frag_threshold, ap_cfg.frag_threshold);	/** fragmentation threshold */
		printf("retry_limit             : 0x%04X(%d)\r\n", ap_cfg.retry_limit, ap_cfg.retry_limit);			/**  retry_limit */
		printf("pairwise_update_timeout : 0x%08X(%d)\r\n", ap_cfg.pairwise_update_timeout, ap_cfg.pairwise_update_timeout);/**  pairwise update timeout in milliseconds */
		printf("pwk_retries             : 0x%08X(%d)\r\n", ap_cfg.pwk_retries, ap_cfg.pwk_retries);			/** pairwise handshake retries */
		printf("groupwise_update_timeout: 0x%08X(%d)\r\n", ap_cfg.groupwise_update_timeout, ap_cfg.groupwise_update_timeout);/**  groupwise update timeout in milliseconds */
		printf("gwk_retries             : 0x%08X(%d)\r\n", ap_cfg.gwk_retries, ap_cfg.gwk_retries);			/** groupwise handshake retries */
		printf("preamble_type           : 0x%02X(%d)\r\n", ap_cfg.preamble_type, ap_cfg.preamble_type);		/** preamble type */
		printf("band_cfg                : 0x%02X(%d)\r\n", ap_cfg.band_cfg, ap_cfg.band_cfg);				/** band cfg */
		printf("auth_mode               : 0x%04X(%d)\r\n", ap_cfg.auth_mode, ap_cfg.auth_mode);				/** auth mode */
		printf("protocol                : 0x%04X(%d)\r\n", ap_cfg.protocol, ap_cfg.protocol);				/** encryption protocol */
		printf("key_mgmt                : 0x%04X(%d)\r\n", ap_cfg.key_mgmt, ap_cfg.key_mgmt);				/** key managment type */
	//	ap_cfg.wep_cfg;						/** wep param */
	//	ap_cfg.wpa_cfg;						/** wpa param */
		printf("mgmt_ie_passthru_mask   : 0x%08X(%d)\r\n", ap_cfg.mgmt_ie_passthru_mask, ap_cfg.mgmt_ie_passthru_mask);/** Mgmt IE passthru mask */
	
		/** 11n HT Cap	HTCap_t ht_cap */
		printf("ht_cap_info             : 0x%04X(%d)\r\n", ap_cfg.ht_cap_info, ap_cfg.ht_cap_info);			/** HT Capabilities Info field */
		printf("ampdu_param             : 0x%02X(%d)\r\n", ap_cfg.ampdu_param, ap_cfg.ampdu_param);			/** A-MPDU Parameters field */

		for(i=0;i<16;i++)
		printf("supported_mcs_set[%02d] : 0x%02X(%d)\r\n", i, ap_cfg.supported_mcs_set[i], ap_cfg.supported_mcs_set[i]);/** Supported MCS Set field */

		printf("ht_ext_cap              : 0x%04X(%d)\r\n", ap_cfg.ht_ext_cap, ap_cfg.ht_ext_cap);			/** HT Extended Capabilities field */
		printf("tx_bf_cap               : 0x%08X(%d)\r\n", ap_cfg.tx_bf_cap, ap_cfg.tx_bf_cap);				/** Transmit Beamforming Capabilities field */
		printf("asel                    : 0x%02X(%d)\r\n", ap_cfg.asel, ap_cfg.asel);						/** Antenna Selection Capability field */
		printf("enable_2040coex         : 0x%02X(%d)\r\n", ap_cfg.enable_2040coex, ap_cfg.enable_2040coex);	/** Enable 2040 Coex */
		printf("key_mgmt_operation      : 0x%04X(%d)\r\n", ap_cfg.key_mgmt_operation, ap_cfg.key_mgmt_operation);/** key management operation */
		printf("bss_status              : 0x%04X(%d)\r\n", ap_cfg.bss_status, ap_cfg.bss_status);			/** BSS status */

		printf("num_of_chan             : 0x%08X(%d)\r\n", ap_cfg.num_of_chan, ap_cfg.num_of_chan);			/** Number of channels in scan_channel_list */
		for(i=0;i<ap_cfg.num_of_chan;i++)					/** scan channel list in ACS mode */
		printf("chan_list[%02d]           : number(%d) band_config_type(%d)\r\n", i, ap_cfg.chan_number[i], ap_cfg.band_config_type[i]);

	//	cfg->wmm_para;										/** Wmm parameters */
#endif

		printf("================================================================================\r\n");
    }
}

void
wl_printf_sta_list(void)
{
    int i=0;
    int rssi = 0;
    ewl_sta_list *plist=NULL;

    if(!(plist = malloc(sizeof(ewl_sta_list))))
    {
        printf("%s, failed to alloc plist \r\n",__func__);
        return;
    }

    if(ewlap_get_sta_list(plist))
    {
        printf("%s, failed to get sta list \r\n",__func__);
        return;
    }

    printf("Number of STA = %d\r\n\r\n", plist->sta_count);

    for (i = 0; i < plist->sta_count; i++) {
        printf("STA %d information:\r\n", i + 1);
        printf("=====================\r\n");
        printf("MAC Address: ");
        printf("\t" MACSTR, MAC2STR(plist->info[i].mac_address));
        printf("\r\nPower mfg status: %s\r\n",
               (plist->info[i].power_mfg_status == 0) ? "active" : "power save");

        /** On some platform, s8 is same as unsigned char*/
        rssi = (int) plist->info[i].rssi;
        if (rssi > 0x7f)
            rssi = -(256 - rssi);
        printf("Rssi : %d dBm\r\n\r\n", rssi);
    }
    free(plist);
}

#endif
