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

//
//#include <stdio.h>
//#include <stdint.h>
//#include <stdlib.h>
//#include <inttypes.h>
//#include <sys/types.h>
//#include <string.h>
//#include <sys/queue.h>
//#include <stdarg.h>
//#include <errno.h>
//#include <getopt.h>
//#include <fcntl.h>
//#include <ctype.h>
//#include <sys/stat.h>
//#include <sys/types.h>

#include <rte_config.h>
#include <rte_common.h>
#include <rte_byteorder.h>
#include <rte_log.h>
#include <rte_memory.h>
#include <rte_memcpy.h>
#include <rte_memzone.h>
#include <rte_eal.h>
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

//#include "util-dpdk-config.h"
#include "util-dpdk-common.h"
#include "util-dpdk-setup.h"
#include "util-dpdk-validate.h"

#include "src/util-error.h"
#include "src/util-debug.h"
#endif /*__DPDK_INCLUDE_COMMON__*/

