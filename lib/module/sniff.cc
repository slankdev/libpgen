

/*
 * copyright (C) <2015>  <Slank Hiroki Shirokura>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */


#include <pgen/pgen-types.h>
#include <pgen/io/pgen-io.h>
#include <pgen/io/pgen-error.h>
#include <pgen/module/sniff.h>



void sniff(pgen_t* handle, bool (*callback)(const u_char*, int)){
	if(pgen_descriptor_is_readable(handle) == false){
		fprintf(stderr, "sniff: handle is not read mode \n");
		return;
	}

	
	u_char  packet[4096];
	bool result = true;
	int len;
	

	for(;result;){
		len = pgen_recv(handle, packet, sizeof(packet));
		if(len < 0){
			pgen_perror("sniff");
			return;
		}else if(len == 0){
			printf("fin pack\n");	
			return;
		}


		// if(pgen_descriptor_is_offline(handle)){ // offline sniff
		// 	len = pgen_recv_from_pcap(handle->offline.fd, packet, sizeof(packet));
		// 	if(len < 0){
		// 		pgen_perror("sniff");
		// 		return;
		// 	}
        //
		// }else{ // online sniff	
		// 	len = pgen_recv_from_netif(handle->online.fd, packet, sizeof(packet));
		// 	if(len < 0){
		// 		pgen_perror("sniff");
		// 		return;
		// 	}
        //
		// }
		
		result = (*callback)(packet, len);
	}
}

