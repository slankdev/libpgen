

#include <pgen.h>

pgen_t* h;
pgen_t* w;



bool func(const u_char* p, int l){
	pgen_ip pack;
	int res = pgen_checkpack(&pack, p, l);

	pack.cast(p,l);
	pack.compile();

	switch(res){
		case 1:
			printf(" - length error %d, %d \n", l, pack.len);
			pgen_hex(p, l);
			pgen_hex(pack.data, pack.len);
			break;
		case 2:
			printf(" - binary error \n");
			pgen_hex(p, l);
			pgen_hex(pack.data, l);
			break;
		default:
			printf(" - success \n");
			break;
	}
	pack.send_handle(w);

	return true;	
}



int main(int argc, char** argv){
	if(argc != 2){
		printf("Usage: %s filename \n", argv[0]);
		return -1;
	}

	h = pgen_open_offline(argv[1], 0);
	w = pgen_open_offline("out.pcap", 1);
	if(h == NULL || w == NULL){
		pgen_perror("oopts");
		return -1;
	}

	sniff(h, func);
	pgen_close(h);
	pgen_close(w);
}





//int main(int argc, char** argv){
//	pgen_ip pack;
//
//
//	if(argc != 3){
//		printf("Usage: %s filename missfile\n", argv[0]);
//		return -1;
//	}
//	struct pgen_checkopt pc;
//
//	strncpy(pc.readfile, argv[1], sizeof(pc.readfile));
//	strncpy(pc.writefile, argv[2], sizeof(pc.writefile));
//	pgen_check(&pack, &pc);
//
//	printf("read : %s \n", pc.readfile);
//	printf("write: %s \n", pc.writefile);
//	for(int i=0; i<pc.all_count; i++){
//		if(pc.result[i] == 1)
//			printf("[%02d] length not same \n", i);
//		else if(pc.result[i] == 2)
//			printf("[%02d] binary not same \n", i);
//	}
//	printf("%d/%d failed \n",
//			pc.len_failed_count+pc.bin_failed_count, pc.all_count);
//	
//}


