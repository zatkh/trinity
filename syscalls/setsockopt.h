/*
 * SYSCALL_DEFINE5(setsockopt, int, fd, int, level, int, optname, char __user *, optval, int, optlen)
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <linux/tcp.h>
#include <netinet/udp.h>
#include <netipx/ipx.h>
#include <netatalk/at.h>
#include <netax25/ax25.h>
#include <netrose/rose.h>
#include <netrom/netrom.h>
#include <linux/dn.h>
#include <linux/tipc.h>
#include <linux/filter.h>
#include <linux/icmpv6.h>
#include <linux/icmp.h>
#include <linux/if_packet.h>
#include <linux/atmdev.h>
#include <linux/atm.h>
#include <linux/irda.h>
#include <linux/if.h>
#include <linux/llc.h>
#include <linux/dccp.h>
#include <linux/netlink.h>
#include <linux/if_pppol2tp.h>

#include "compat.h"
#include "config.h"

#ifdef USE_CAIF
#include <linux/caif/caif_socket.h>
#endif

#ifdef USE_RDS
#include <linux/rds.h>
#endif

#define SOL_TCP		6
#define SOL_SCTP        132
#define SOL_UDPLITE     136
#define SOL_NETBEUI     267
#define SOL_LLC         268
#define SOL_DCCP        269
#define SOL_NETLINK     270
#define SOL_RXRPC       272
#define SOL_PPPOL2TP    273
#define SOL_BLUETOOTH   274
#define SOL_PNPIPE      275
#define SOL_RDS         276
#define SOL_IUCV        277
#define SOL_CAIF        278
#define SOL_ALG         279
#define SOL_NFC		280

#define NR_SOL_IP_OPTS 19
static int ip_opts[NR_SOL_IP_OPTS] = { IP_TOS, IP_TTL, IP_HDRINCL, IP_OPTIONS,
	IP_ROUTER_ALERT, IP_RECVOPTS, IP_RETOPTS, IP_PKTINFO,
	IP_PKTOPTIONS, IP_MTU_DISCOVER, IP_RECVERR, IP_RECVTTL,
	IP_RECVTOS, IP_MTU, IP_FREEBIND, IP_IPSEC_POLICY,
	IP_XFRM_POLICY, IP_PASSSEC, IP_TRANSPARENT };

#define NR_SOL_SOCKET_OPTS 49
static int socket_opts[NR_SOL_SOCKET_OPTS] = { SO_DEBUG, SO_REUSEADDR, SO_TYPE, SO_ERROR,
	SO_DONTROUTE, SO_BROADCAST, SO_SNDBUF, SO_RCVBUF,
	SO_SNDBUFFORCE, SO_RCVBUFFORCE, SO_KEEPALIVE, SO_OOBINLINE,
	SO_NO_CHECK, SO_PRIORITY, SO_LINGER, SO_BSDCOMPAT,
	SO_REUSEPORT, SO_PASSCRED, SO_PEERCRED, SO_RCVLOWAT, SO_SNDLOWAT,
	SO_RCVTIMEO, SO_SNDTIMEO, SO_SECURITY_AUTHENTICATION, SO_SECURITY_ENCRYPTION_TRANSPORT,
	SO_SECURITY_ENCRYPTION_NETWORK, SO_BINDTODEVICE, SO_ATTACH_FILTER, SO_DETACH_FILTER,
	SO_PEERNAME, SO_TIMESTAMP, SO_ACCEPTCONN, SO_PEERSEC,
	SO_PASSSEC, SO_TIMESTAMPNS, SO_MARK, SO_TIMESTAMPING,
	SO_PROTOCOL, SO_DOMAIN, SO_RXQ_OVFL, SO_WIFI_STATUS,
	SO_PEEK_OFF, SO_NOFCS, SO_LOCK_FILTER, SO_SELECT_ERR_QUEUE };

#define NR_SOL_TCP_OPTS 23
static int tcp_opts[NR_SOL_TCP_OPTS] = { TCP_NODELAY, TCP_MAXSEG, TCP_CORK, TCP_KEEPIDLE,
	TCP_KEEPINTVL, TCP_KEEPCNT, TCP_SYNCNT, TCP_LINGER2,
	TCP_DEFER_ACCEPT, TCP_WINDOW_CLAMP, TCP_INFO, TCP_QUICKACK,
	TCP_CONGESTION, TCP_MD5SIG, TCP_COOKIE_TRANSACTIONS, TCP_THIN_LINEAR_TIMEOUTS,
	TCP_THIN_DUPACK, TCP_USER_TIMEOUT, TCP_REPAIR, TCP_REPAIR_QUEUE,
	TCP_QUEUE_SEQ, TCP_REPAIR_OPTIONS, TCP_FASTOPEN};

#define NR_SOL_UDP_OPTS 2
static int udp_opts[NR_SOL_UDP_OPTS] = { UDP_CORK, UDP_ENCAP };

#define NR_SOL_UDPLITE_OPTS 4
static int udplite_opts[NR_SOL_UDPLITE_OPTS] = { UDP_CORK, UDP_ENCAP, UDPLITE_SEND_CSCOV, UDPLITE_RECV_CSCOV };

#define NR_SOL_IPV6_OPTS 24
static int ipv6_opts[NR_SOL_IPV6_OPTS] = {
	IPV6_ADDRFORM, IPV6_2292PKTINFO, IPV6_2292HOPOPTS, IPV6_2292DSTOPTS,
	IPV6_2292RTHDR, IPV6_2292PKTOPTIONS, IPV6_CHECKSUM, IPV6_2292HOPLIMIT,
	IPV6_NEXTHOP, IPV6_AUTHHDR, IPV6_FLOWINFO, IPV6_UNICAST_HOPS,
	IPV6_MULTICAST_IF, IPV6_MULTICAST_HOPS, IPV6_MULTICAST_LOOP, IPV6_ADD_MEMBERSHIP,
	IPV6_DROP_MEMBERSHIP, IPV6_ROUTER_ALERT, IPV6_MTU_DISCOVER, IPV6_MTU,
	IPV6_RECVERR, IPV6_V6ONLY, IPV6_JOIN_ANYCAST, IPV6_LEAVE_ANYCAST };

#define NR_SOL_ICMPV6_OPTS 1
static int icmpv6_opts[NR_SOL_ICMPV6_OPTS] = { ICMPV6_FILTER };

#define NR_SOL_SCTP_OPTS 39
static int sctp_opts[NR_SOL_SCTP_OPTS] = {
	SCTP_RTOINFO, SCTP_ASSOCINFO, SCTP_INITMSG, SCTP_NODELAY,
	SCTP_AUTOCLOSE, SCTP_SET_PEER_PRIMARY_ADDR, SCTP_PRIMARY_ADDR, SCTP_ADAPTATION_LAYER,
	SCTP_DISABLE_FRAGMENTS, SCTP_PEER_ADDR_PARAMS, SCTP_DEFAULT_SEND_PARAM, SCTP_EVENTS,
	SCTP_I_WANT_MAPPED_V4_ADDR, SCTP_MAXSEG, SCTP_STATUS, SCTP_GET_PEER_ADDR_INFO,
	SCTP_DELAYED_ACK_TIME, SCTP_CONTEXT, SCTP_FRAGMENT_INTERLEAVE, SCTP_PARTIAL_DELIVERY_POINT,
	SCTP_MAX_BURST, SCTP_AUTH_CHUNK, SCTP_HMAC_IDENT, SCTP_AUTH_KEY,
	SCTP_AUTH_ACTIVE_KEY, SCTP_AUTH_DELETE_KEY, SCTP_PEER_AUTH_CHUNKS, SCTP_LOCAL_AUTH_CHUNKS,
	SCTP_GET_ASSOC_NUMBER, SCTP_GET_ASSOC_ID_LIST, SCTP_AUTO_ASCONF,

	SCTP_SOCKOPT_BINDX_ADD, SCTP_SOCKOPT_BINDX_REM, SCTP_SOCKOPT_PEELOFF, SCTP_SOCKOPT_CONNECTX_OLD,
	SCTP_GET_PEER_ADDRS, SCTP_GET_LOCAL_ADDRS, SCTP_SOCKOPT_CONNECTX, SCTP_SOCKOPT_CONNECTX3 };

#define NR_SOL_AX25_OPTS 12
static int ax25_opts[NR_SOL_AX25_OPTS] = {
	AX25_WINDOW, AX25_T1, AX25_N2, AX25_T3,
	AX25_T2, AX25_BACKOFF, AX25_EXTSEQ, AX25_PIDINCL,
	AX25_IDLE, AX25_PACLEN, AX25_IAMDIGI,
	SO_BINDTODEVICE };

#define NR_SOL_NETROM_OPTS 5
static int netrom_opts[NR_SOL_NETROM_OPTS] = {
	NETROM_T1, NETROM_T2, NETROM_N2, NETROM_T4, NETROM_IDLE };

#define NR_SOL_ROSE_OPTS 7
static int rose_opts[NR_SOL_ROSE_OPTS] = {
	ROSE_DEFER, ROSE_T1, ROSE_T2, ROSE_T3,
	ROSE_IDLE, ROSE_QBITINCL, ROSE_HOLDBACK };

#define NR_SOL_DECNET_OPTS 18
static int decnet_opts[NR_SOL_DECNET_OPTS] = {
	SO_CONDATA, SO_CONACCESS, SO_PROXYUSR, SO_LINKINFO,
	DSO_CONDATA, DSO_DISDATA, DSO_CONACCESS, DSO_ACCEPTMODE,
	DSO_CONACCEPT, DSO_CONREJECT, DSO_LINKINFO, DSO_STREAM,
	DSO_SEQPACKET, DSO_MAXWINDOW, DSO_NODELAY, DSO_CORK,
	DSO_SERVICES, DSO_INFO
};

#define NR_SOL_PACKET_OPTS 22
static int packet_opts[NR_SOL_PACKET_OPTS] = {
	PACKET_ADD_MEMBERSHIP, PACKET_DROP_MEMBERSHIP, PACKET_RECV_OUTPUT, 4,	/* Value 4 is still used by obsolete turbo-packet. */
	PACKET_RX_RING, PACKET_STATISTICS, PACKET_COPY_THRESH, PACKET_AUXDATA,
	PACKET_ORIGDEV, PACKET_VERSION, PACKET_HDRLEN, PACKET_RESERVE,
	PACKET_TX_RING, PACKET_LOSS, PACKET_VNET_HDR, PACKET_TX_TIMESTAMP,
	PACKET_TIMESTAMP, PACKET_FANOUT };

#define NR_SOL_ATM_OPTS 6
static int atm_opts[NR_SOL_ATM_OPTS] = {
	SO_SETCLP, SO_CIRANGE, SO_ATMQOS, SO_ATMSAP, SO_ATMPVC, SO_MULTIPOINT };

#define NR_SOL_IRDA_OPTS 11
static int irda_opts[NR_SOL_IRDA_OPTS] = {
	IRLMP_ENUMDEVICES, IRLMP_IAS_SET, IRLMP_IAS_QUERY, IRLMP_HINTS_SET,
	IRLMP_QOS_SET, IRLMP_QOS_GET, IRLMP_MAX_SDU_SIZE, IRLMP_IAS_GET,
	IRLMP_IAS_DEL, IRLMP_HINT_MASK_SET, IRLMP_WAITDEVICE };

#ifndef USE_LLC_OPT_PKTINFO
#define LLC_OPT_PKTINFO LLC_OPT_UNKNOWN
#endif

#define NR_SOL_LLC_OPTS 9
static int llc_opts[NR_SOL_LLC_OPTS] = {
	LLC_OPT_RETRY, LLC_OPT_SIZE, LLC_OPT_ACK_TMR_EXP, LLC_OPT_P_TMR_EXP,
	LLC_OPT_REJ_TMR_EXP, LLC_OPT_BUSY_TMR_EXP, LLC_OPT_TX_WIN, LLC_OPT_RX_WIN,
	LLC_OPT_PKTINFO };

#define NR_SOL_DCCP_OPTS 16
static int dccp_opts[NR_SOL_DCCP_OPTS] = {
	DCCP_SOCKOPT_PACKET_SIZE, DCCP_SOCKOPT_SERVICE, DCCP_SOCKOPT_CHANGE_L, DCCP_SOCKOPT_CHANGE_R,
	DCCP_SOCKOPT_GET_CUR_MPS, DCCP_SOCKOPT_SERVER_TIMEWAIT, DCCP_SOCKOPT_SEND_CSCOV, DCCP_SOCKOPT_RECV_CSCOV,
	DCCP_SOCKOPT_AVAILABLE_CCIDS, DCCP_SOCKOPT_CCID, DCCP_SOCKOPT_TX_CCID, DCCP_SOCKOPT_RX_CCID,
	DCCP_SOCKOPT_QPOLICY_ID, DCCP_SOCKOPT_QPOLICY_TXQLEN, DCCP_SOCKOPT_CCID_RX_INFO, DCCP_SOCKOPT_CCID_TX_INFO };

#define NR_SOL_NETLINK_OPTS 5
static int netlink_opts[NR_SOL_NETLINK_OPTS] = {
	NETLINK_ADD_MEMBERSHIP, NETLINK_DROP_MEMBERSHIP, NETLINK_PKTINFO, NETLINK_BROADCAST_ERROR,
	NETLINK_NO_ENOBUFS };

#define NR_SOL_TIPC_OPTS 6
static int tipc_opts[NR_SOL_TIPC_OPTS] = {
	TIPC_IMPORTANCE, TIPC_SRC_DROPPABLE, TIPC_DEST_DROPPABLE, TIPC_CONN_TIMEOUT,
	TIPC_NODE_RECVQ_DEPTH, TIPC_SOCK_RECVQ_DEPTH };

#define NR_SOL_RXRPC_OPTS 8
static int rxrpc_opts[NR_SOL_RXRPC_OPTS] = {
	RXRPC_USER_CALL_ID, RXRPC_ABORT, RXRPC_ACK, RXRPC_NET_ERROR,
	RXRPC_BUSY, RXRPC_LOCAL_ERROR, RXRPC_NEW_CALL, RXRPC_ACCEPT };

#define NR_SOL_PPPOL2TP_OPTS 5
static int pppol2tp_opts[NR_SOL_PPPOL2TP_OPTS] = {
	PPPOL2TP_SO_DEBUG, PPPOL2TP_SO_RECVSEQ, PPPOL2TP_SO_SENDSEQ, PPPOL2TP_SO_LNSMODE,
	PPPOL2TP_SO_REORDERTO };

#define NR_SOL_BLUETOOTH_OPTS 5
static int bluetooth_opts[NR_SOL_BLUETOOTH_OPTS] = {
	BT_SECURITY, BT_DEFER_SETUP, BT_FLUSHABLE, BT_POWER,
	BT_CHANNEL_POLICY };

#define NR_SOL_BLUETOOTH_HCI_OPTS 3
static int bluetooth_hci_opts[NR_SOL_BLUETOOTH_HCI_OPTS] = {
	HCI_DATA_DIR, HCI_FILTER, HCI_TIME_STAMP };

#define NR_SOL_BLUETOOTH_L2CAP_OPTS 2
static int bluetooth_l2cap_opts[NR_SOL_BLUETOOTH_L2CAP_OPTS] = {
	L2CAP_OPTIONS, L2CAP_LM };

#define NR_SOL_BLUETOOTH_RFCOMM_OPTS 2
static int bluetooth_rfcomm_opts[NR_SOL_BLUETOOTH_RFCOMM_OPTS] = { RFCOMM_LM };

#ifdef USE_RDS
#define NR_SOL_RDS_OPTS 7
static int rds_opts[NR_SOL_RDS_OPTS] = {
	RDS_CANCEL_SENT_TO, RDS_GET_MR, RDS_FREE_MR,
	4, /* deprecated RDS_BARRIER 4 */
	RDS_RECVERR, RDS_CONG_MONITOR, RDS_GET_MR_FOR_DEST };
#endif

#define NR_SOL_IUCV_OPTS 3
static int iucv_opts[NR_SOL_IUCV_OPTS] = {
	SO_IPRMDATA_MSG, SO_MSGLIMIT, SO_MSGSIZE };

#ifdef USE_CAIF
#define NR_SOL_CAIF_OPTS 2
static int caif_opts[NR_SOL_CAIF_OPTS] = {
	CAIFSO_LINK_SELECT, CAIFSO_REQ_PARAM };
#endif

