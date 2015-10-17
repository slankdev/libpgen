

#include <pgen.h>


/*
struct pgen_checkopt{
	int count;
	int failed_count;
	char readfile[256];
	char writefile[256];
};
*/

//int pgen_check(pgen_packet* pack, struct pgen_checkopt* str);

int main(int argc, char** argv){
	if(argc != 3){
		printf("Usage: %s filename missfile\n", argv[0]);
		return -1;
	}
	struct pgen_checkopt pc;
	pgen_arp pack;

	strncpy(pc.readfile, argv[1], sizeof(pc.readfile));
	strncpy(pc.writefile, argv[2], sizeof(pc.writefile));

	pgen_check(&pack, &pc);
	printf("read : %s \n", pc.readfile);
	printf("write: %s \n", pc.writefile);
	printf("%d/%d failed \n", pc.failed_count, pc.count);
}



/*
int pgen_check(pgen_packet* pack, struct pgen_checkopt* str){
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

		if(len != pack->len){
			printf("%d: length isn't same %d->%d \n", i, len, pack->len);	
			pgen_send_to_pcap(write_handle->offline.fd, buf, len);
			str->failed_count++;
		}else if((memcmp(pack->data, buf, len)!=0)){
			printf("%d: binary isn't same \n", i);
			pack->hex();
			pgen_hex(buf, len);
			pgen_send_to_pcap(write_handle->offline.fd, buf, len);
			str->failed_count++;
		}
		str->count++;
	}

	pgen_close(read_handle);
	pgen_close(write_handle);

	if(str->failed_count == 0)	return 1;
	else 						return -1;
}



*/
