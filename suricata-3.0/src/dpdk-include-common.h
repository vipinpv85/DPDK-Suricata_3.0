#ifndef __DPDK_INCLUDE_COMMON__
#define __DPDK_INCLUDE_COMMON__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>

#include <rte_config.h>
#include <rte_eal.h>
#include <rte_common.h>
#include <rte_byteorder.h>
#include <rte_log.h>
#include <rte_memory.h>
#include <rte_memcpy.h>
#include <rte_memzone.h>
#include <rte_per_lcore.h>
#include <rte_launch.h>
#include <rte_atomic.h>
#include <rte_cycles.h>
#include <rte_prefetch.h>
#include <rte_lcore.h>
#include <rte_per_lcore.h>
#include <rte_branch_prediction.h>
#include <rte_interrupts.h>
#include <rte_pci.h>
#include <rte_random.h>
#include <rte_debug.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_ether.h>
#include <rte_ethdev.h>
#include <rte_ip.h>
#include <rte_tcp.h>
#include <rte_udp.h>
#include <rte_string_fns.h>
#include <rte_acl.h>
#include <rte_version.h>
#include <rte_tailq.h>
#include <rte_cfgfile.h>
#include <rte_hexdump.h>

//#include "util-dpdk-config.h"
#include "util-dpdk-common.h"
#include "util-dpdk-setup.h"
#include "util-dpdk-validate.h"

#include "src/util-error.h"
#include "src/util-debug.h"

/*
 * Rule and trace formats definitions.
 */
enum {
    PROTO_FIELD_IPV4,
    SRC_FIELD_IPV4,
    DST_FIELD_IPV4,
#if 0
    SRCP_FIELD_IPV4,
    DSTP_FIELD_IPV4,
#endif
    NUM_FIELDS_IPV4
};

/*
 * That effectively defines order of IPV4 classifications:
 *  - PROTO
 *  - SRC IP ADDRESS
 *  - DST IP ADDRESS
 *  - PORTS (SRC and DST)
 */
enum {
    RTE_ACL_IPV4_PROTO,
    RTE_ACL_IPV4_SRC,
    RTE_ACL_IPV4_DST,
    RTE_ACL_IPV4_PORTS,
    RTE_ACL_IPV4_NUM
};

/*
 --- ipv4 ---
src ip 3
dst ip 7
sport 11
dport 13
 --- ipv6 ---
 src ip 2
 dst ip 18
 sport ip 34
 dport ip 36
 */

static struct rte_acl_field_def ip4_defs[NUM_FIELDS_IPV4] = {
    [0] = {
    .type = RTE_ACL_FIELD_TYPE_BITMASK,
    .size = sizeof(uint8_t),
    .field_index = PROTO_FIELD_IPV4,
    .input_index = RTE_ACL_IPV4_PROTO,
    .offset = 0,
    },
    [1] = {
    .type = RTE_ACL_FIELD_TYPE_RANGE/*RTE_ACL_FIELD_TYPE_MASK*/,
    .size = sizeof(uint32_t),
    .field_index = SRC_FIELD_IPV4,
    .input_index = RTE_ACL_IPV4_SRC,
    //.offset = offsetof(struct ipv4_hdr, src_addr) - offsetof(struct ipv4_hdr, next_proto_id),
    .offset = 3,
    },
    [2] = {
    .type = RTE_ACL_FIELD_TYPE_RANGE/*RTE_ACL_FIELD_TYPE_MASK*/,
    .size = sizeof(uint32_t),
    .field_index = DST_FIELD_IPV4,
    .input_index = RTE_ACL_IPV4_DST,
    //.offset = offsetof(struct ipv4_hdr, dst_addr) - offsetof(struct ipv4_hdr, next_proto_id),
    .offset = 8,
    },
#if 0
    [3] = {
    .type = RTE_ACL_FIELD_TYPE_RANGE,
    .size = sizeof(uint16_t),
    .field_index = SRCP_FIELD_IPV4,
    .input_index = RTE_ACL_IPV4_PORTS,
    .offset = 12,
    },
    [4] ={
    .type = RTE_ACL_FIELD_TYPE_RANGE,
    .size = sizeof(uint16_t),
    .field_index = DSTP_FIELD_IPV4,
    .input_index = RTE_ACL_IPV4_PORTS,
    .offset =  14,
    },
#endif
};

enum {
    IP6_PROTO,
    IP6_SRC0,
    IP6_SRC1,
    IP6_SRC2,
    IP6_SRC3,
    IP6_DST0,
    IP6_DST1,
    IP6_DST2,
    IP6_DST3,
#if 0
    IP6_SRCP,
    IP6_DSTP,
#endif
    IP6_NUM
};
#define IP6_ADDR_SIZE 16
static struct rte_acl_field_def ip6_defs[IP6_NUM] = {
    {
    .type = RTE_ACL_FIELD_TYPE_BITMASK,
    .size = sizeof(uint8_t),
    .field_index = IP6_PROTO,
    .input_index = IP6_PROTO,
    .offset = 0,
    },
    {
    .type = RTE_ACL_FIELD_TYPE_RANGE/*RTE_ACL_FIELD_TYPE_MASK*/,
    .size = 4,
    .field_index = IP6_SRC0,
    .input_index = IP6_SRC0,
    .offset = 2,
    },
    {
    .type = RTE_ACL_FIELD_TYPE_RANGE/*RTE_ACL_FIELD_TYPE_MASK*/,
    .size = 4,
    .field_index = IP6_SRC1,
    .input_index = IP6_SRC1,
    .offset = 6,
    },
    {
    .type = RTE_ACL_FIELD_TYPE_RANGE/*RTE_ACL_FIELD_TYPE_MASK*/,
    .size = 4,
    .field_index = IP6_SRC2,
    .input_index = IP6_SRC2,
    .offset = 10,
    },
    {
    .type = RTE_ACL_FIELD_TYPE_RANGE/*RTE_ACL_FIELD_TYPE_MASK*/,
    .size = 4,
    .field_index = IP6_SRC3,
    .input_index = IP6_SRC3,
    .offset = 14,
    },
    {
    .type = RTE_ACL_FIELD_TYPE_RANGE/*RTE_ACL_FIELD_TYPE_MASK*/,
    .size = 4,
    .field_index = IP6_DST0,
    .input_index = IP6_DST0,
    .offset = 18,
    },
    {
    .type = RTE_ACL_FIELD_TYPE_RANGE/*RTE_ACL_FIELD_TYPE_MASK*/,
    .size = 4,
    .field_index = IP6_DST1,
    .input_index = IP6_DST1,
    .offset = 22,
    },
    {
    .type = RTE_ACL_FIELD_TYPE_RANGE/*RTE_ACL_FIELD_TYPE_MASK*/,
    .size = 4,
    .field_index = IP6_DST2,
    .input_index = IP6_DST2,
    .offset = 26,
    },
    {
    .type = RTE_ACL_FIELD_TYPE_RANGE/*RTE_ACL_FIELD_TYPE_MASK*/,
    .size = 4,
    .field_index = IP6_DST3,
    .input_index = IP6_DST3,
    .offset = 30,
    },
#if 0
    {
    .type = RTE_ACL_FIELD_TYPE_RANGE,
    .size = sizeof(uint16_t),
    .field_index = IP6_SRCP,
    .input_index = IP6_SRCP,
    .offset = 34,
    },
    {
    .type = RTE_ACL_FIELD_TYPE_RANGE,
    .size = sizeof(uint16_t),
    .field_index = IP6_DSTP,
    .input_index = IP6_SRCP,
    .offset = 36,
    }
#endif
};

RTE_ACL_RULE_DEF(acl4_rule, RTE_DIM(ip4_defs));
RTE_ACL_RULE_DEF(acl6_rule, RTE_DIM(ip6_defs));

#endif /*__DPDK_INCLUDE_COMMON__*/
