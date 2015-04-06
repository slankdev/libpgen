// many packet's option code macros
// format: PGEN_XXXOP_YYY 



/* option  of ARP */
#include <net/if_arp.h>
#define PGEN_ARPOP_REQEST     ARPOP_REQUEST				/* ARP request.  */
#define PGEN_ARPOP_REPLY      ARPOP_REPLY          		/* ARP reply.    */
#define PGEN_ARPOP_RREQEST    ARPOP_RREQUEST    		/* RARP request. */
#define PGEN_ARPOP_RREPLY     ARPOP_RREPLY      		/* RARP reply.   */
#define PGEN_ARPOP_InREQUEST  ARPOP_InREQUEST   		/* InARP request.*/
#define PGEN_ARPOP_InREPLY    ARPOP_InREPLY     		/* InARP reply.  */
#define PGEN_ARPOP_NAK        ARPOP_NAK            		/* (ATM)ARP NAK. */



/* option  of ICMP */
#include <netinet/ip_icmp.h>
#define PGEN_ICMPOP_ECHOREPLY		ICMP_ECHOREPLY		 	/* Echo Reply				*/
#define PGEN_ICMPOP_DEST_UNREACH	ICMP_DEST_UNREACH	  	/* Destination Unreachable	*/
#define PGEN_ICMPOP_SOURCE_QUENCH	ICMP_SOURCE_QUENCH	 	/* Source Quench			*/
#define PGEN_ICMPOP_REDIRECT		ICMP_REDIRECT		 	/* Redirect (change route)	*/
#define PGEN_ICMPOP_ECHO			ICMP_ECHO			  	/* Echo Request				*/
#define PGEN_ICMPOP_TIME_EXCEEDED	ICMP_TIME_EXCEEDED		/* Time Exceeded			*/
#define PGEN_ICMPOP_PARAMETERPROB	ICMP_PARAMETERPROB		/* Parameter Problem		*/
#define PGEN_ICMPOP_TIMESTAMP		ICMP_TIMESTAMP			/* Timestamp Request		*/
#define PGEN_ICMPOP_TIMESTAMPREPLY	ICMP_TIMESTAMPREPLY		/* Timestamp Reply			*/
#define PGEN_ICMPOP_INFO_REQUEST	ICMP_INFO_REQUEST		/* Information Request		*/
#define PGEN_ICMPOP_INFO_REPLY		ICMP_INFO_REPLY			/* Information Reply		*/
#define PGEN_ICMPOP_ADDRESS			ICMP_ADDRESS			/* Address Mask Request		*/
#define PGEN_ICMPOP_ADDRESSREPLY	ICMP_ADDRESSREPLY		/* Address Mask Reply		*/
#define PGEN_ICMOOP_NR_ICMP_TYPES	NR_ICMP_TYPES			/* Nop						*/



/* code of ICMP */
#include <netinet/ip_icmp.h>
#define PGEN_ICMPCODE_NET_UNREACH		ICMP_NET_UNREACH	/* Network Unreachable			*/
#define PGEN_ICMPCODE_HOST_UNREACH		ICMP_HOST_UNREACH	/* Host Unreachable				*/
#define PGEN_ICMPCODE_PROT_UNREACH		ICMP_PROT_UNREACH	/* Protocol Unreachable			*/
#define PGEN_ICMPCODE_PORT_UNREACH		ICMP_PORT_UNREACH	/* Port Unreachable				*/
#define PGEN_ICMPCODE_FRAG_NEEDED		ICMP_FRAG_NEEDED	/* Fragmentation Needed/DF set	*/
#define PGEN_ICMPCODE_SR_FAILED			ICMP_SR_FAILED		/* Source Route failed			*/
#define PGEN_ICMPCODE_NET_UNKNOWN		ICMP_NET_UNKNOWN	
#define PGEN_ICMPCODE_HOST_UNKNOWN		ICMP_HOST_UNKNOWN	
#define PGEN_ICMPCODE_HOST_ISOLATED		ICMP_HOST_ISOLATED	
#define PGEN_ICMPCODE_NET_ANO			ICMP_NET_ANO		
#define PGEN_ICMPCODE_HOST_ANO			ICMP_HOST_ANO		
#define PGEN_ICMPCODE_NET_UNR_TOS		ICMP_NET_UNR_TOS	
#define PGEN_ICMPCODE_HOST_UNR_TOS		ICMP_HOST_UNR_TOS	
#define PGEN_ICMPCODE_PKT_FILTERED		ICMP_PKT_FILTERED	/* Packet filtered				 */
#define PGEN_ICMPCODE_PREC_VIOLATION	ICMP_PREC_VIOLATION	/* Precedence violation			 */
#define PGEN_ICMPCODE_PREC_CUTOFF		ICMP_PREC_CUTOFF	/* Precedence cut off			 */
#define PGEN_ICMPCODE_NR_ICMP_UNREACH	NR_ICMP_UNREACH		/* instead of hardcoding 
															   			immediate value	   	 */




