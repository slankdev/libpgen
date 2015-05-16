// many packet's option code macros
// format: PGEN_XXXOP_YYY 

#include "mytypes.h"


/* option  of ARP */
#include <net/if_arp.h>
#define PGEN_ARPOP_REQEST     MT_ARPOP_REQUEST				/* ARP request.  */
#define PGEN_ARPOP_REPLY      MT_ARPOP_REPLY          		/* ARP reply.    */
#define PGEN_ARPOP_RREQEST    MT_ARPOP_RREQUEST    		/* RARP request. */
#define PGEN_ARPOP_RREPLY     MT_ARPOP_RREPLY      		/* RARP reply.   */
#define PGEN_ARPOP_InREQUEST  MT_ARPOP_InREQUEST   		/* InARP request.*/
#define PGEN_ARPOP_InREPLY    MT_ARPOP_InREPLY     		/* InARP reply.  */
#define PGEN_ARPOP_NAK        MT_ARPOP_NAK            		/* (ATM)ARP NAK. */
                             


/* option  of ICMP */
#include <netinet/ip_icmp.h>
#define PGEN_ICMPOP_ECHOREPLY		MT_ICMP_ECHOREPLY		 	/* Echo Reply				*/
#define PGEN_ICMPOP_DEST_UNREACH	MT_ICMP_DEST_UNREACH	  	/* Destination Unreachable	*/
#define PGEN_ICMPOP_SOURCE_QUENCH	MT_ICMP_SOURCE_QUENCH	 	/* Source Quench			*/
#define PGEN_ICMPOP_REDIRECT		MT_ICMP_REDIRECT		 	/* Redirect (change route)	*/
#define PGEN_ICMPOP_ECHO			MT_ICMP_ECHO			  	/* Echo Request				*/
#define PGEN_ICMPOP_TIME_EXCEEDED	MT_ICMP_TIME_EXCEEDED		/* Time Exceeded			*/
#define PGEN_ICMPOP_PARAMETERPROB	MT_ICMP_PARAMETERPROB		/* Parameter Problem		*/
#define PGEN_ICMPOP_TIMESTAMP		MT_ICMP_TIMESTAMP			/* Timestamp Request		*/
#define PGEN_ICMPOP_TIMESTAMPREPLY	MT_ICMP_TIMESTAMPREPLY		/* Timestamp Reply			*/
#define PGEN_ICMPOP_INFO_REQUEST	MT_ICMP_INFO_REQUEST		/* Information Request		*/
#define PGEN_ICMPOP_INFO_REPLY		MT_ICMP_INFO_REPLY			/* Information Reply		*/
#define PGEN_ICMPOP_ADDRESS			MT_ICMP_ADDRESS			/* Address Mask Request		*/
#define PGEN_ICMPOP_ADDRESSREPLY	MT_ICMP_ADDRESSREPLY		/* Address Mask Reply		*/
#define PGEN_ICMOOP_NR_ICMP_TYPES	MT_NR_ICMP_TYPES			/* Nop						*/
        
                                   

/* code of ICMP */
#include <netinet/ip_icmp.h>
#define PGEN_ICMPCODE_NET_UNREACH		MT_ICMP_NET_UNREACH	/* Network Unreachable			*/
#define PGEN_ICMPCODE_HOST_UNREACH		MT_ICMP_HOST_UNREACH	/* Host Unreachable				*/
#define PGEN_ICMPCODE_PROT_UNREACH		MT_ICMP_PROT_UNREACH	/* Protocol Unreachable			*/
#define PGEN_ICMPCODE_PORT_UNREACH		MT_ICMP_PORT_UNREACH	/* Port Unreachable				*/
#define PGEN_ICMPCODE_FRAG_NEEDED		MT_ICMP_FRAG_NEEDED	/* Fragmentation Needed/DF set	*/
#define PGEN_ICMPCODE_SR_FAILED			MT_ICMP_SR_FAILED		/* Source Route failed			*/
#define PGEN_ICMPCODE_NET_UNKNOWN		MT_ICMP_NET_UNKNOWN	
#define PGEN_ICMPCODE_HOST_UNKNOWN		MT_ICMP_HOST_UNKNOWN	
#define PGEN_ICMPCODE_HOST_ISOLATED		MT_ICMP_HOST_ISOLATED	
#define PGEN_ICMPCODE_NET_ANO			MT_ICMP_NET_ANO		
#define PGEN_ICMPCODE_HOST_ANO			MT_ICMP_HOST_ANO		
#define PGEN_ICMPCODE_NET_UNR_TOS		MT_ICMP_NET_UNR_TOS	
#define PGEN_ICMPCODE_HOST_UNR_TOS		MT_ICMP_HOST_UNR_TOS	
#define PGEN_ICMPCODE_PKT_FILTERED		MT_ICMP_PKT_FILTERED	/* Packet filtered				 */
#define PGEN_ICMPCODE_PREC_VIOLATION	MT_ICMP_PREC_VIOLATION	/* Precedence violation			 */
#define PGEN_ICMPCODE_PREC_CUTOFF		MT_ICMP_PREC_CUTOFF	/* Precedence cut off			 */
#define PGEN_ICMPCODE_NR_ICMP_UNREACH	MT_NR_ICMP_UNREACH		/* instead of hardcoding 
															   			immediate value	   	 */




