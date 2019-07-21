/*! \ingroup wl_wifi 
 * Authentication modes */
enum wl_auth_mode {
    AUTH_MODE_INVALID,
    AUTH_MODE_AUTO,
    AUTH_MODE_OPEN_SYSTEM,
    AUTH_MODE_SHARED_KEY,
    AUTH_MODE_WPA,
    AUTH_MODE_WPA2,
    AUTH_MODE_WPA_PSK,
    AUTH_MODE_WPA2_PSK
};

/*! \ingroup wl_wifi 
 * Encryption modes */
enum wl_enc_type {  /* Values map to 802.11 encryption suites... */
    ENC_TYPE_WEP  = 5,
    ENC_TYPE_TKIP = 2,
    ENC_TYPE_CCMP = 4,
    /* ... except these two, 7 and 8 are reserved in 802.11-2007 */
    ENC_TYPE_NONE = 7,
    ENC_TYPE_AUTO = 8
};


/*! \ingroup wl_wifi 
 *
 * Network representation 
 *
 */
struct wl_network_t
{
    u8 bssid[ETH_ALEN];
    u8 ssid[32];
    size_t ssid_len;
    u8  auth;
    u8  enc_type;
    int freq;
    int rssi;
};

/*! \ingroup wl_wifi 
 * Network list representation. Array of pointers to wl_network_t entries.
 *
 */
struct wl_network_list_t
{
    struct wl_network_t **net; /**< The list of pointers to networks */
    size_t cnt;                /**< Number of networks */
};

int supplicant_run_init(void *ctx);
void supplicant_run(void);

