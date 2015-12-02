

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


#include <stdio.h>
#include <pgen/pgen-types.h>
#include <pgen/module/debug.h>
#include <pgen/core/packet/packet.h>


int pgen_checkpack(pgen_packet* pack, const void* buf, int len){
	pack->cast(buf, len);
	pack->compile();

	if(pack->length() != len){
		return 1;
	}else if(memcmp(buf, pack->byte(), len) != 0){
		return 2;
	}else{
		return 0;	
	}
}


int pgen_check(pgen_packet* pack, struct pgen_checkopt* str){
	str->all_count=0;
	str->len_failed_count=0;
	str->bin_failed_count=0;
	memset(str->result, 0, sizeof str->result);
	int len;
	u_char buf[10000];

	pgen_t* read_handle  = pgen_open_offline(str->readfile, 0);
	pgen_t* write_handle = pgen_open_offline(str->writefile, 1);
	if(read_handle == NULL || write_handle == NULL){
		pgen_perror("pgen_open_offline");
		return -1;
	}
	
	for(int i=0; (len = pgen_recv_from_pcap(read_handle->offline.fd, buf, sizeof(buf))) >= 0; i++){
		pack->cast(buf, len);
		pack->compile();

		if(len != pack->length()){
			str->result[i] = 1;
			pgen_send_to_pcap(write_handle->offline.fd, buf, len);
			str->len_failed_count++;
		}else if((memcmp(pack->byte(), buf, len)!=0)){
			str->result[i] = 2;
			pgen_send_to_pcap(write_handle->offline.fd, buf, len);
			str->bin_failed_count++;
		}else{
			str->result[i] = 0;	
		}
		str->all_count++;
	}

	pgen_close(read_handle);
	pgen_close(write_handle);

	if(str->len_failed_count == str->bin_failed_count == 0)	return 1;
	else 						return -1;
}




void debugprint(int flag, const char* str){
	if(flag)
		printf("%s", str);
}








