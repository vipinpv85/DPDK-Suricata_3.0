#define _GNU_SOURCE

#include "util-dpdk-setup.h"
#include "util-dpdk-config.h"
#include "util-dpdk-common.h"
#include "util-error.h"
#include "util-debug.h"
#include "dpdk-include-common.h"
#include "source-dpdkintel.h"

/* D E F I N E S*/
#define SC_DPDK_MAJOR    1
#define SC_DPDK_MINOR    8
#define EAL_ARGS         32


/* E X T E R N */
extern stats_matchPattern_t stats_matchPattern;
extern uint64_t coreSet;

/* G L O B A L S */
uint8_t  portSpeed[16];
uint8_t  portSpeed10;
uint8_t  portSpeed100;
uint8_t  portSpeed1000;
uint8_t  portSpeed10000;
uint8_t  portSpeedUnknown;
uint8_t  dpdkIntelCoreCount = 0;

DpdkIntelPortMap portMap[16];
launchPtr launchFunc[5];

file_config_t file_config;

struct rte_mempool *dp_pktmbuf_pool = NULL;
struct rte_ring    *srb[16];

struct acl4_rule testv4;
struct acl6_rule testv6;

uint16_t argument_count = 1;
char argument[EAL_ARGS][EAL_ARGS * 3] = {{"suricata"}, {""}};

/* STATIC */
static const struct rte_eth_conf portConfDefault = {
    .rxmode = {
        .split_hdr_size = 0,
    },
    .txmode = {
        .mq_mode = ETH_MQ_TX_NONE,
    },
};

static struct rte_eth_txconf tx_conf = {
};

static struct rte_eth_rxconf rx_conf = {
    .rx_drop_en = 1,
};

static struct   ether_addr dp_ports_eth_addr [S_DPDK_MAX_ETHPORTS];

void initLaunchFunc(void);

int ringBuffSetup(void)
{
    char srbName [25];
    uint8_t index = 0, maxRing = 16;
    //(DPDKINTEL_GENCFG.Port > SC_RINGBUF)?SC_RINGBUF:DPDKINTEL_GENCFG.Port;

    for (index = 0; index < maxRing; index++)
    {
        sprintf(srbName, "%s%d", "RINGBUFF", index);

        srb [index] = rte_ring_create(srbName, RTE_RING_SIZE, 
                             SOCKET_ID_ANY, RING_F_SP_ENQ);

        if (NULL == srb [index])
        {
            SCLogError(SC_ERR_DPDKINTEL_MEM_FAILED, " Cannot create Ring buff %s", srbName);
            return -1;
        }
        SCLogDebug("Suricata Ring Buffer %s created", srbName);
    }

    return 0;
}

int dpdkPortUnSet(uint8_t portId)
{
    rte_eth_dev_stop(portId);

    SCLogDebug("dev stop done for Port : %u",portId);

    rte_eth_promiscuous_disable(portId);

    return 0;
}

int32_t dpdkIntelDevSetup(void)
{
    uint8_t portIndex = 0, portTotal = rte_eth_dev_count_avail();
    uint8_t inport = 0;
    int32_t ret = 0;
    char portName[RTE_ETH_NAME_MAX_LEN] = {0};

    struct rte_eth_link link;
    struct rte_eth_dev_info dev_info;

    if (unlikely((DPDKINTEL_GENCFG.Port <= 0) || (DPDKINTEL_GENCFG.Port > portTotal))){
        SCLogError(SC_ERR_DPDKINTEL_CONFIG_FAILED, " Ports in DPDK %d Config-file %d", 
                   portTotal, DPDKINTEL_GENCFG.Port);
        return -1;
    }
    SCLogDebug(" - DPDK ports %d config-file ports %d", portTotal, DPDKINTEL_GENCFG.Port);

    dp_pktmbuf_pool =
             rte_mempool_create("mbuf_pool", NB_MBUF,
                        MBUF_SIZE, 32,
                        sizeof(struct rte_pktmbuf_pool_private),
                        rte_pktmbuf_pool_init, NULL,
                        rte_pktmbuf_init, NULL,
                        rte_socket_id()/*SOCKET_ID_ANY*/,
                        0/*MEMPOOL_F_SP_PUT*/);
    if (unlikely(NULL == dp_pktmbuf_pool))
    {
        SCLogError(SC_ERR_DPDKINTEL_MEM_FAILED," mbuf_pool alloc failed");
        return -1;
    }
    SCLogDebug(" - pkt MBUFF setup %p", dp_pktmbuf_pool);

    ret = ringBuffSetup();
    if (ret < 0)
    {
        SCLogError(SC_ERR_DPDKINTEL_MEM_FAILED, " DPDK Ring Buffer setup failed");
        return -11;
    }

    /* check interface PCI information
       ToDo: support for non INTEL PCI interfaces also - phase 2
     */
    for (portIndex = 0; portIndex < DPDKINTEL_GENCFG.Port; portIndex++)
    {
        struct rte_eth_conf portConf = portConfDefault;

        inport = portMap [portIndex].inport;
        if (!rte_eth_dev_is_valid_port(inport)) {
            SCLogError(SC_ERR_DPDKINTEL_CONFIG_FAILED," invalid: err=%d, port=%u\n",
                  ret, (unsigned) inport);
            return -7;
        }

        rte_eth_dev_info_get (inport, &dev_info);
        if (rte_eth_dev_get_name_by_port(inport, portName) == 0)
            SCLogDebug(" - port (%u) Name (%s)", inport, portName);
        fflush(stdout);

        /* ToDo - change default configuration to systune configuration */
        if (dev_info.tx_offload_capa & DEV_TX_OFFLOAD_MBUF_FAST_FREE)
                portConf.txmode.offloads |=
                        DEV_TX_OFFLOAD_MBUF_FAST_FREE;

        ret = rte_eth_dev_configure(inport, 1, 1, &portConf);
        if (ret < 0)
        {
            /* TODO: free mempool */
            SCLogError(SC_ERR_DPDKINTEL_CONFIG_FAILED," configure device: err=%d, port=%u\n",
                  ret, (unsigned) inport);
            return -7;
        }
        SCLogDebug(" - Configured Port %d", inport);

        rte_eth_macaddr_get(inport, 
                           &dp_ports_eth_addr[inport]);

        /* init one RX queue */
        fflush(stdout);
        ret = rte_eth_rx_queue_setup(inport, 0, RTE_TEST_RX_DESC_DEFAULT,
                                     0/*SOCKET_ID_ANY*/,
                                     NULL,
                                     dp_pktmbuf_pool);
        if (ret < 0)
        {
            /* TODO: free mempool */
            SCLogError(SC_ERR_DPDKINTEL_CONFIG_FAILED," rte_eth_rx_queue_setup: err=%d, port=%u\n",
                  ret, (unsigned) inport);
            return -8;
        }
        SCLogDebug(" - RX Queue setup Port %d", inport);

        /* init one TX queue on each port */
        fflush(stdout);
        ret = rte_eth_tx_queue_setup(inport, 0, RTE_TEST_TX_DESC_DEFAULT,
                                     0/*SOCKET_ID_ANY*/,
                                     NULL);
        if (ret < 0)
        {
            SCLogError(SC_ERR_DPDKINTEL_CONFIG_FAILED, " rte_eth_tx_queue_setup:err=%d, port=%u",
                ret, (unsigned) inport);
            return -9;
        }
        SCLogDebug(" - TX Queue setup Port %d", inport);

        /* ToDo: check this from YAML conf file - pahse 2 */
        rte_eth_promiscuous_enable(inport);

        /* check interface link, speed, duplex */
        rte_eth_link_get(inport, &link);
        if (link.link_duplex != ETH_LINK_FULL_DUPLEX) {
            SCLogError(SC_ERR_MISSING_CONFIG_PARAM,
                       " port:%u; duplex:%s, status:%s",
                       (unsigned) inport,
                       (link.link_duplex == ETH_LINK_FULL_DUPLEX)?"Full":"half",
                       (link.link_status == 1)?"up":"down");
            //return -10;
        }
        portSpeed[inport] =    (link.link_speed == ETH_SPEED_NUM_10M)?1:
                               (link.link_speed == ETH_SPEED_NUM_100M)?2:
                               (link.link_speed == ETH_SPEED_NUM_1G)?3:
                               (link.link_speed == ETH_SPEED_NUM_10G)?4:
                               (link.link_speed == ETH_SPEED_NUM_20G)?5:
                               (link.link_speed == ETH_SPEED_NUM_40G)?6:
                               0;

        /* ToDo: add support for 20G and 40G */
        if ((link.link_speed == ETH_SPEED_NUM_20G) || 
            (link.link_speed == ETH_SPEED_NUM_40G))
        {
            SCLogError(SC_ERR_DPDKINTEL_CONFIG_FAILED, " Port %u unspported speed %u",
                       inport, portSpeed[inport]);
            return -11;
        }

        {
            (link.link_speed == ETH_SPEED_NUM_10M)?portSpeed10++:
            (link.link_speed == ETH_SPEED_NUM_100M)?portSpeed100++:
            (link.link_speed == ETH_SPEED_NUM_1G)?portSpeed1000++:
            (link.link_speed == ETH_SPEED_NUM_10G)?portSpeed10000++:
            portSpeedUnknown++;
        }

    }

    SCLogDebug("DPDK port setup over!!");
    return 0;
}


void dpdkConfSetup(void)
{
    int32_t ret = 0;
    uint8_t inport = 0, outport = 0, portIndex = 0, portBit = 0;

    SCLogNotice("DPDK Version: %s", rte_version());

    ret = rte_eal_has_hugepages();
    if (unlikely(ret < 0))
    {
        SCLogError(SC_ERR_MISSING_CONFIG_PARAM, "No hugepage configured; %d ", ret);
        rte_panic("ERROR: No Huge Page\n");
        exit(EXIT_FAILURE);
    }

    ret = rte_eal_iopl_init();
    if (ret < 0)
    {
        SCLogError(SC_ERR_MISSING_CONFIG_PARAM, "DPDK IOPL init %d ", ret);
        //rte_panic("ERROR: Cannot init IOPL\n");
        //exit(EXIT_FAILURE);
    }

    /* display default configuration */
    dumpGlobalConfig();

    /* check gloabl configuration meets the requirements */
    if (validateGlobalConfig() != 0) {
        SCLogError(SC_ERR_MISSING_CONFIG_PARAM, "DPDK config validate!!!");
        exit(EXIT_FAILURE);
    }

    /* DPDK Interface setup*/
    if (dpdkIntelDevSetup() != 0) {
        SCLogError(SC_ERR_MISSING_CONFIG_PARAM, "DPDK dev setup!!!");
        exit(EXIT_FAILURE);
    }

    for (portIndex = 0; portIndex < DPDKINTEL_GENCFG.Port; portIndex++) {
        inport  = portMap [portIndex].inport;
        outport = portMap [portIndex].outport;

        if (((portBit >> inport) & 1)  && ((portBit >> outport) & 1 ))
            continue;

        /* check for 10G or smaller */
        if (portSpeed[inport] <= 4) {
           SCLogDebug(" Config core for %d <--> %d", inport, outport);
           dpdkIntelCoreCount++;
        }
        else {
            SCLogError(SC_ERR_DPDKINTEL_CONFIG_FAILED,
                       " Unsupported speed ");
            exit(EXIT_FAILURE);
        }

        if (portSpeed [inport] != portSpeed [outport]) {
            SCLogError(SC_ERR_DPDKINTEL_CONFIG_FAILED,
                      "Mapped ports %d <--> %d Speed Mismatch",
                      inport, outport);
            exit(EXIT_FAILURE);
        }

        portBit |= 1 << inport;
        portBit |= 1 << outport;
    }

    file_config.isDpdk = 1;
    file_config.dpdkCpuCount = rte_eth_dev_count_avail();
    //file_config.dpdkCpuOffset = rte_lcore_count() - DPDKINTEL_GENCFG.Port;
    file_config.dpdkCpuOffset = rte_lcore_count() - dpdkIntelCoreCount;
    file_config.suricataCpuOffset = 0;

    initLaunchFunc();
}

void *ParseDpdkConf(void)
{
	SCEnter();
	struct rte_cfgfile *file = NULL;

	file = rte_cfgfile_load("dpdk-suricata.ini", 0);

	/* get section name EAL */
	if (rte_cfgfile_has_section(file, "EAL")) {
		SCLogDebug(" section (EAL); count %d", rte_cfgfile_num_sections(file, "EAL", sizeof("EAL") - 1));
		SCLogNotice(" section (EAL) has entries %d", rte_cfgfile_section_num_entries(file, "EAL"));

		int n_entries = rte_cfgfile_section_num_entries(file, "EAL");
		struct rte_cfgfile_entry entries[n_entries];

		if (rte_cfgfile_section_entries(file, "EAL", entries, n_entries) != -1) {

			for (int i = 0; i < n_entries; i++) {
				SCLogNotice(" - name: (%s) value: (%s)", entries[i].name, entries[i].value);
				snprintf(argument[i * 2 + 1], EAL_ARGS, "%s", entries[i].name);
				snprintf(argument[i * 2 + 2], EAL_ARGS * 3, "%s", entries[i].value);
				SCLogDebug(" - argument: (%s) (%s)", argument[i * 2 + 1], argument[i * 2 + 2]);
			        argument_count += (((entries[i].name) ? 1 : 0) + ((entries[i].value) ? 1 : 0));
			}
		}
	}

#if 0
	/* get section name PORT-X */
	for (int i = 0; i < RTE_MAX_ETHPORTS; i++) {
		char port_section_name[15] = {""};

		sprintf(port_section_name, "%s%d", "PORT-", i);
		if (rte_cfgfile_has_section(file, port_section_name)) {
			int n_port_entries = rte_cfgfile_section_num_entries(file, port_section_name);

			SCLogDebug(" %s", port_section_name);
			SCLogDebug(" section (PORT) has %d entries", n_port_entries);

			struct rte_cfgfile_entry entries[n_port_entries];
			if (rte_cfgfile_section_entries(file, port_section_name, entries, n_port_entries) != -1) {

				for (int j = 0; j < n_port_entries; j++) {
					SCLogDebug(" %s name: (%s) value: (%s)", port_section_name, entries[j].name, entries[j].value);

					if (strcasecmp("rx-queues", entries[j].name) == 0)
						dpdk_ports[i].rxq_count = atoi(entries[j].value);
					else if (strcasecmp("tx-queues", entries[j].name) == 0)
						dpdk_ports[i].txq_count = atoi(entries[j].value);
					else if (strcasecmp("mtu", entries[j].name) == 0)
						dpdk_ports[i].mtu = atoi(entries[j].value);
					else if (strcasecmp("rss-tuple", entries[j].name) == 0)
						dpdk_ports[i].rss_tuple = atoi(entries[j].value);
					else if (strcasecmp("jumbo", entries[j].name) == 0)
						dpdk_ports[i].jumbo = (strcasecmp(entries[j].value, "yes") == 0) ? 1 : 0;
					else if (strcasecmp("core", entries[j].name) == 0)
						dpdk_ports[i].lcore_index = atoi(entries[j].value);
				}
			}
		}
	}

	/* get section name MEMPOOL-PORT */
	if (rte_cfgfile_has_section(file, "MEMPOOL-PORT")) {
		SCLogDebug(" section (MEMPOOL-PORT); count %d", rte_cfgfile_num_sections(file, "MEMPOOL-PORT", sizeof("MEMPOOL-PORT") - 1));
		SCLogDebug(" section (MEMPOOL-PORT) has entries %d", rte_cfgfile_section_num_entries(file, "MEMPOOL-PORT"));

		int n_entries = rte_cfgfile_section_num_entries(file, "MEMPOOL-PORT");
		struct rte_cfgfile_entry entries[n_entries];

		if (rte_cfgfile_section_entries(file, "MEMPOOL-PORT", entries, n_entries) != -1) {
			for (int j = 0; j < n_entries; j++) {
				SCLogDebug(" - entries[i] name: (%s) value: (%s)", entries[j].name, entries[j].value);

				if (strcasecmp("name", entries[j].name) == 0)
					rte_memcpy(dpdk_mempool_config.name, entries[j].value, sizeof(entries[j].value));
				if (strcasecmp("n", entries[j].name) == 0)
					dpdk_mempool_config.n = atoi(entries[j].value);
				if (strcasecmp("elt_size", entries[j].name) == 0)
					dpdk_mempool_config.elt_size = atoi(entries[j].value);
				if (strcasecmp("private_data_size", entries[j].name) == 0)
					dpdk_mempool_config.private_data_size = atoi(entries[j].value);
				if (strcasecmp("socket_id", entries[j].name) == 0)
					dpdk_mempool_config.private_data_size = atoi(entries[j].value);
			}
		}
	}
#endif

	rte_cfgfile_close(file);

	SCReturnPtr(file, "void *");
}


void dpdkAclConfSetup(void)
{
    struct rte_acl_param acl_param;
    struct rte_acl_ctx *ctx;
    
    SCLogNotice("DPDK ACL setup\n");

    acl_param.socket_id = 0;
    acl_param.max_rule_num = 1024 * 1024 * 8;

    /* setup acl - IPv4 */
    acl_param.rule_size = RTE_ACL_RULE_SZ(RTE_DIM(ip4_defs));
    acl_param.name = "suricata-ipv4";
    ctx = rte_acl_create(&acl_param);
    if ((ctx == NULL) || (rte_acl_set_ctx_classify(ctx, RTE_ACL_CLASSIFY_SSE))) {
        SCLogError(SC_ERR_MISSING_CONFIG_PARAM, "acl ipv4 fail!!!");
        exit(EXIT_FAILURE);
    }
    SCLogNotice("DPDK ipv4AclCtx: %p done!", ctx);
    file_config.acl.ipv4AclCtx = (void *)ctx;

    /* setup acl - IPv6 */
    acl_param.rule_size = RTE_ACL_RULE_SZ(RTE_DIM(ip6_defs));
    acl_param.name = "suricata-ipv6";
    ctx = rte_acl_create(&acl_param);
    if ((ctx == NULL) || (rte_acl_set_ctx_classify(ctx, RTE_ACL_CLASSIFY_SSE))){
        SCLogError(SC_ERR_MISSING_CONFIG_PARAM, "acl ipv6 fail!!!");
        exit(EXIT_FAILURE);
    }
    SCLogNotice("DPDK ipv6AclCtx: %p done!", ctx);
    file_config.acl.ipv6AclCtx = (void *)ctx;

}

int32_t addDpdkAcl4Rule(uint32_t srcIp, uint32_t srcIpMask, uint32_t dstIp, uint32_t dstIpMask, uint8_t proto)
{
    int ret = 0;

    memset(&testv4, 0, sizeof(testv4));

    testv4.data.category_mask = -1;
    testv4.data.priority = file_config.acl.ipv4AclCount + 1;
    testv4.data.userdata = 0x1;

    testv4.field[PROTO_FIELD_IPV4].value.u8 = proto;
    testv4.field[PROTO_FIELD_IPV4].mask_range.u8 = 0xff;
    testv4.field[DST_FIELD_IPV4].value.u32 = (dstIp);
    testv4.field[DST_FIELD_IPV4].mask_range.u32 = (dstIpMask);
    testv4.field[SRC_FIELD_IPV4].value.u32 = (srcIp);
    testv4.field[SRC_FIELD_IPV4].mask_range.u32 = (srcIpMask);

    SCLogDebug(" - Proto 0x%x Mask 0xFF, SRC IP %x Mask %x, DST IP %x Mask %x",
        proto, srcIp, srcIpMask, dstIp, dstIpMask);

    //rte_hexdump(stdout, "ipv4 acl: ", (const void *) &testv4, sizeof(testv4));
    ret = rte_acl_add_rules(file_config.acl.ipv4AclCtx, (const struct rte_acl_rule *) &testv4, 1);
    if (unlikely(ret != 0)) {
       SCLogNotice(" ACL ipv4 add fail %d", ret);
       SCLogNotice(" - Proto 0x%x Mask 0xFF", proto);
       SCLogNotice(" - SRC IP %x Mask %x", srcIp, srcIpMask);
       SCLogNotice(" - DST IP %x Mask %x", dstIp, dstIpMask);
    } else
        file_config.acl.ipv4AclCount += 1;

    return ret;
}

int32_t addDpdkAcl6Rule(uint32_t srcIp[4], uint32_t srcIpMask[4], uint32_t dstIp[4], uint32_t dstIpMask[4], uint8_t proto)
{
    int ret = 0;

    memset(&testv6, 0, sizeof(testv6));

    testv6.data.category_mask = -1;
    testv6.data.priority = file_config.acl.ipv6AclCount + 1;
    testv6.data.userdata = 0x1;

    testv6.field[IP6_PROTO].value.u8 = proto;
    testv6.field[IP6_PROTO].mask_range.u8 = 0xff;

    testv6.field[IP6_DST0].value.u32 = dstIp[0];
    testv6.field[IP6_DST1].value.u32 = dstIp[1];
    testv6.field[IP6_DST2].value.u32 = dstIp[2];
    testv6.field[IP6_DST3].value.u32 = dstIp[3];
    testv6.field[IP6_DST0].mask_range.u32 = dstIpMask[0];
    testv6.field[IP6_DST1].mask_range.u32 = dstIpMask[1];
    testv6.field[IP6_DST2].mask_range.u32 = dstIpMask[2];
    testv6.field[IP6_DST3].mask_range.u32 = dstIpMask[3];

    testv6.field[IP6_SRC0].value.u32 = srcIp[0];
    testv6.field[IP6_SRC1].value.u32 = srcIp[1];
    testv6.field[IP6_SRC2].value.u32 = srcIp[2];
    testv6.field[IP6_SRC3].value.u32 = srcIp[3];
    testv6.field[IP6_SRC0].mask_range.u32 = srcIpMask[0];
    testv6.field[IP6_SRC1].mask_range.u32 = srcIpMask[1];
    testv6.field[IP6_SRC2].mask_range.u32 = srcIpMask[2];
    testv6.field[IP6_SRC3].mask_range.u32 = srcIpMask[3];

    //rte_hexdump(stdout, "ipv6 acl: ", (const void *) &testv6, sizeof(testv6));
    ret = rte_acl_add_rules(file_config.acl.ipv6AclCtx, (const struct rte_acl_rule *) &testv6, 1);
    if (unlikely(ret != 0)) {
       rte_acl_dump(file_config.acl.ipv6AclCtx);
       SCLogNotice(" ACL ipv6 add fail %d", ret);
       SCLogNotice(" - Proto 0x%x Mask 0xFF", proto);
       SCLogNotice(" - SRC IP %x:%x:%x:%x Mask %x:%x:%x:%x",
           srcIp[0], srcIp[1], srcIp[2], srcIp[3],
           srcIpMask[0], srcIpMask[1], srcIpMask[2], srcIpMask[3]);
       SCLogNotice(" - DST IP %x:%x:%x:%x Mask %x:%x:%x:%x",
           dstIp[0], dstIp[1], dstIp[2], dstIp[3],
           dstIpMask[0], dstIpMask[1], dstIpMask[2], dstIpMask[3]);
    } else
        file_config.acl.ipv6AclCount += 1;

    return ret;
}

int32_t addDpdkAcl4Build(void)
{
    int ret = 0;
    struct rte_acl_config acl_build_param = {0};

    acl_build_param.num_categories = 1;
    acl_build_param.num_fields = RTE_DIM(ip4_defs);
    memcpy(&acl_build_param.defs, ip4_defs, sizeof(ip4_defs));

    ret = rte_acl_build(file_config.acl.ipv4AclCtx, &acl_build_param);
    if (ret) {
        SCLogNotice("ACL ipv4 build failed %d", ret);
    }
    rte_acl_dump(file_config.acl.ipv4AclCtx);

    return ret;
}

int32_t addDpdkAcl6Build(void)
{
    int ret = 0;
    struct rte_acl_config acl_build_param = {0};

    acl_build_param.num_categories = 1;
    acl_build_param.num_fields = RTE_DIM(ip6_defs);
    memcpy(&acl_build_param.defs, ip6_defs, sizeof(ip6_defs));

    ret = rte_acl_build(file_config.acl.ipv6AclCtx, &acl_build_param);
    if (ret) {
        SCLogNotice("ACL ipv6 build failed %d", ret);
    }
    rte_acl_dump(file_config.acl.ipv6AclCtx);

    return ret;
}



int32_t dpdkEalInit()
{
    char *args[EAL_ARGS];

    for (int j = 0; j < argument_count; j++)
        args[j] = argument[j];

    int ret = rte_eal_init(argument_count, (char **)args);
    if (ret < 0)
    {
        SCLogError(SC_ERR_MISSING_CONFIG_PARAM, "DPDK EAL init %d ", ret);
        rte_panic("ERROR: Cannot init EAL\n");
        return -1;
    }
    return 0;
}

void dumpMatchPattern(void)
{
    SCLogNotice("----- Match Pattern ----");
    SCLogNotice(" * http:  %"PRId64" ",stats_matchPattern.http);
    SCLogNotice(" * ftp:   %"PRId64" ",stats_matchPattern.ftp);
    SCLogNotice(" * tls:   %"PRId64" ",stats_matchPattern.tls);
    SCLogNotice(" * dns:   %"PRId64" ",stats_matchPattern.dns);
    SCLogNotice(" * smtp:  %"PRId64" ",stats_matchPattern.smtp);
    SCLogNotice(" * ssh:   %"PRId64" ",stats_matchPattern.ssh);
    SCLogNotice(" * smb:   %"PRId64" ",stats_matchPattern.smb);
    SCLogNotice(" * smb2:  %"PRId64" ",stats_matchPattern.smb2);
    SCLogNotice(" * dcerpc:%"PRId64" ",stats_matchPattern.dcerpc);
    SCLogNotice(" * tcp:   %"PRId64" ",stats_matchPattern.tcp);
    SCLogNotice(" * udp:   %"PRId64" ",stats_matchPattern.udp);
    SCLogNotice(" * sctp:  %"PRId64" ",stats_matchPattern.sctp);
    SCLogNotice(" * icmpv4:%"PRId64" ",stats_matchPattern.icmpv4);
    SCLogNotice(" * icmpv6:%"PRId64" ",stats_matchPattern.icmpv6);
    SCLogNotice(" * gre:   %"PRId64" ",stats_matchPattern.gre);
    SCLogNotice(" * raw:   %"PRId64" ",stats_matchPattern.raw);
    SCLogNotice(" * ipv4:  %"PRId64" ",stats_matchPattern.ipv4);
    SCLogNotice(" * ipv6:  %"PRId64" ",stats_matchPattern.ipv6);
    SCLogNotice("-----------------------");

    if (rte_acl_find_existing("suricata-ipv4")) {
        SCLogNotice("----- ACL IPV4 DUMP (%u) ----", file_config.acl.ipv4AclCount);
        //rte_acl_dump(file_config.acl.ipv4AclCtx);
    }

    if (rte_acl_find_existing("suricata-ipv6")) {
        SCLogNotice("----- ACL IPV6 DUMP (%u) ----", file_config.acl.ipv6AclCount);
        //rte_acl_dump(file_config.acl.ipv6AclCtx);
    }

    return;
}

void dumpGlobalConfig(void)
{
    uint8_t index;

    SCLogNotice("----- Global DPDK-INTEL Config -----");
    SCLogNotice(" Number Of Ports  : %d", DPDKINTEL_GENCFG.Port);
    SCLogNotice(" Operation Mode   : %s", ((DPDKINTEL_GENCFG.OpMode == 1) ?"IDS":
                                           (DPDKINTEL_GENCFG.OpMode == 2) ?"IPS":"BYPASS"));
    for (index = 0; index < DPDKINTEL_GENCFG.Port; index++)
    {
        SCLogNotice(" Port:%d, Map:%d", portMap [index].inport, 
                                        portMap [index].outport);
    }
    SCLogNotice("------------------------------------");

    return;
}

uint32_t getCpuCOunt(uint32_t CpuBmp)
{
    uint32_t coreCounts = 0x00;

    do {
        if (CpuBmp)
        {
            coreCounts++;
            CpuBmp = CpuBmp & (CpuBmp - 1);
        }
    } while (CpuBmp);
    
    return coreCounts; 
}

/*  To find the core index from number*/
uint32_t getCpuIndex(void)
{
    static uint32_t cpuIndex = 0;
    unsigned lcore_id = 0;

    struct rte_config *ptr = rte_eal_get_configuration();
    if (ptr == NULL) {
        SCLogError(SC_ERR_DPDKINTEL_CONFIG_FAILED, " Cannot fetch rte_eal_get_configuration");
        return (-1);
    }

    SCLogNotice(" master_lcore %x lcore_count %u", ptr->master_lcore, ptr->lcore_count);
    RTE_LCORE_FOREACH_SLAVE(lcore_id) {
        if ((cpuIndex & (1 << lcore_id)) == 0) {
            cpuIndex |= (1 << lcore_id);
            SCLogNotice(" cpuIndex %x lcore_id %x", cpuIndex, lcore_id);
            return lcore_id;
        }
    }

    SCLogError(SC_ERR_DPDKINTEL_CONFIG_FAILED, " Cannot get right lcore!");
    return (-1);
}

void initLaunchFunc(void)
{
    launchFunc[IDS] = ReceiveDpdkPkts_IDS;
    launchFunc[IPS] = ReceiveDpdkPkts_IPS;
    launchFunc[BYPASS] = ReceiveDpdkPkts_BYPASS;

    return;
}
