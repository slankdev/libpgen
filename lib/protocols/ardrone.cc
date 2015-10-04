

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
#include "pgen.h"
#include "packet.h"
#include "address.h"
#include "pgen-types.h"
#include "pgen-io.h"
#include "ardrone.h"

static int get_original_string(const char* p, char buf[]);
static int get_command_len(const void*);


pgen_ardrone::pgen_ardrone(){
	clear();
}




pgen_ardrone::pgen_ardrone(const void* packet, int len){
	clear();
	cast(packet, len);
}



void pgen_ardrone::clear_pcmd(){
	this->ARDRONE.pcmd.seq   = 0;
	this->ARDRONE.pcmd.flag  = 0;
	this->ARDRONE.pcmd.roll  = 0;
	this->ARDRONE.pcmd.pitch = 0;
	this->ARDRONE.pcmd.gaz   = 0;
	this->ARDRONE.pcmd.yaw.x = 0;
	this->ARDRONE.pcmd.yaw.y = 0;
	this->ARDRONE.pcmd.yaw.z = 0;
}




void pgen_ardrone::clear_ref(){
	this->ARDRONE.ref.seq        = 0;
	this->ARDRONE.ref.command    = 0;
}





void pgen_ardrone::clear_configids(){
	this->ARDRONE.configids.seq     = 0;
	memset(this->ARDRONE.configids.session, 0, sizeof(this->ARDRONE.configids.session));
	memset(this->ARDRONE.configids.user, 0, sizeof(this->ARDRONE.configids.user));
	memset(this->ARDRONE.configids.app , 0, sizeof(this->ARDRONE.configids.app ));
}




void pgen_ardrone::clear_config(){
	this->ARDRONE.config.seq     = 0;
	memset(this->ARDRONE.config.name, 0, sizeof(this->ARDRONE.config.name));
	memset(this->ARDRONE.config.parameter, 0, sizeof(this->ARDRONE.config.parameter));
}




void pgen_ardrone::clear_anim(){}
void pgen_ardrone::clear_ftrim(){}
void pgen_ardrone::clear_led(){}
void pgen_ardrone::clear_comwdg(){}




void pgen_ardrone::clear_ctrl(){
	this->ARDRONE.ctrl.seq                = 0;
	this->ARDRONE.ctrl.ctrlmode           = 0;
	this->ARDRONE.ctrl.fw_update_filesize = 0;
}




void pgen_ardrone::clear(){
	pgen_udp::clear();
	
	clear_pcmd();
	clear_ref();
	clear_configids();
	clear_anim();
	clear_ftrim();
	clear_config();
	clear_led();
	clear_comwdg();
	clear_ctrl();

	memset(pcmd_data, 0, sizeof(pcmd_data));
	memset(ref_data , 0, sizeof(ref_data));
	memset(configids_data, 0, sizeof(configids_data));
	memset(anim_data, 0, sizeof(anim_data));
	memset(ftrim_data, 0, sizeof(ftrim_data));
	memset(config_data, 0, sizeof(config_data));
	memset(led_data, 0, sizeof(led_data));
	memset(comwdg_data, 0, sizeof(comwdg_data));
	memset(ctrl_data, 0, sizeof(ctrl_data));

	pcmd_data_len      = 0;
	ref_data_len       = 0;
	configids_data_len = 0;
	anim_data_len      = 0;
	ftrim_data_len     = 0;
	config_data_len    = 0;
	led_data_len       = 0;
	comwdg_data_len    = 0;
	ctrl_data_len      = 0;
}





int pgen_ardrone::compile_pcmd(){
	snprintf(pcmd_data, sizeof(pcmd_data), "AT*PCMD_MAG=%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld", 
			this->ARDRONE.pcmd.seq, this->ARDRONE.pcmd.flag, 
			this->ARDRONE.pcmd.roll, this->ARDRONE.pcmd.pitch, 
			this->ARDRONE.pcmd.gaz, this->ARDRONE.pcmd.yaw.x, 
			this->ARDRONE.pcmd.yaw.y, this->ARDRONE.pcmd.yaw.z);
	return strlen(pcmd_data);
}




int pgen_ardrone::compile_ref(){
	snprintf(ref_data, sizeof(ref_data),"AT*REF=%ld,%ld", 
			this->ARDRONE.ref.seq, this->ARDRONE.ref.command);
	return strlen(ref_data);	
}




int pgen_ardrone::compile_configids(){
	snprintf(configids_data,sizeof(configids_data), "AT*CONFIG_IDS=%ld,\"%s\",\"%s\",\"%s\"", 
			this->ARDRONE.configids.seq, this->ARDRONE.configids.session,
			this->ARDRONE.configids.user, this->ARDRONE.configids.app);
	return strlen(configids_data);
}




int pgen_ardrone::compile_config(){
	snprintf(config_data, sizeof(configids_data), "AT*CONFIG=%ld,\"%s\",\"%s\"", 
			this->ARDRONE.config.seq, this->ARDRONE.config.name,
			this->ARDRONE.config.parameter);
	return strlen(config_data);
}




int pgen_ardrone::compile_anim(){
	return 0;
}
int pgen_ardrone::compile_ftrim(){
	return 0;
}
int pgen_ardrone::compile_led(){
	return 0;
}
int pgen_ardrone::compile_comwdg(){
	return 0;
}





int pgen_ardrone::compile_ctrl(){
	snprintf(ctrl_data, sizeof(ctrl_data), "AT*CTRL=%ld,%ld,%ld", this->ARDRONE.ctrl.seq, 
			this->ARDRONE.ctrl.ctrlmode, this->ARDRONE.ctrl.fw_update_filesize);
	return strlen(ctrl_data);
}





void pgen_ardrone::compile(){
	pcmd_data_len      = compile_pcmd();
	ref_data_len       = compile_ref();
	configids_data_len = compile_configids();
	anim_data_len      = compile_anim();
	ftrim_data_len     = compile_ftrim();
	config_data_len    = compile_config();
	led_data_len       = compile_led();
	comwdg_data_len    = compile_comwdg();
	ctrl_data_len      = compile_ctrl();

	char command[256];
	int command_len = 0;
	char spliter = 0x0d;

	memset(command, 0, sizeof(command));
	char* p1 = command;
	for(int i=0; i<this->ARDRONE.cmd_count; i++){
		if(this->ARDRONE.cmd_type[i] == ARDRONE_CMD_PCMD){
			memcpy(p1, pcmd_data, pcmd_data_len);
			p1 += pcmd_data_len;
		}else if(this->ARDRONE.cmd_type[i] == ARDRONE_CMD_REF){
			memcpy(p1, ref_data, ref_data_len);
			p1 += ref_data_len;
		}else if(this->ARDRONE.cmd_type[i] == ARDRONE_CMD_CONFIG_IDS){
			memcpy(p1, configids_data, configids_data_len);
			p1 += configids_data_len;
		}else if(this->ARDRONE.cmd_type[i] == ARDRONE_CMD_ANIM){
			memcpy(p1, anim_data, anim_data_len);
			p1 += anim_data_len;
		}else if(this->ARDRONE.cmd_type[i] == ARDRONE_CMD_FTRIM){
			memcpy(p1, ftrim_data, ftrim_data_len);
			p1 += ftrim_data_len;
		}else if(this->ARDRONE.cmd_type[i] == ARDRONE_CMD_CONFIG){
			memcpy(p1, config_data, config_data_len);
			p1 += config_data_len;
		}else if(this->ARDRONE.cmd_type[i] == ARDRONE_CMD_LED){
			memcpy(p1, led_data, led_data_len);
			p1 += led_data_len;
		}else if(this->ARDRONE.cmd_type[i] == ARDRONE_CMD_COMWDG){
			memcpy(p1, comwdg_data, comwdg_data_len);
			p1 += comwdg_data_len;
		}else if(this->ARDRONE.cmd_type[i] == ARDRONE_CMD_CTRL){
			memcpy(p1, ctrl_data, ctrl_data_len);
			p1 += ctrl_data_len;
		}else{
			fprintf(stderr, "pgen_ardrone::compile: command type is not found \n");	
			return;
		}
		memcpy(p1, &spliter, 1);
		p1 += 1;
	}
	command_len = p1 - command;

	this->UDP.len = UDP_HDR_LEN + command_len;
	pgen_udp::compile();

	memset(this->data, 0, PGEN_MAX_PACKET_LEN);
	u_char* p = this->data;
	memcpy(p, &eth, sizeof eth);
	p += sizeof(eth);
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct ip_header);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct udp_header);
	memcpy(p, command, command_len);
	p += command_len;
	len = p- this->data;
}







int pgen_ardrone::cast_pcmd(const char* buf){
	int ret = sscanf(buf, "AT*PCMD_MAG=%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld",
			&ARDRONE.pcmd.seq, &ARDRONE.pcmd.flag, &ARDRONE.pcmd.roll,
			&ARDRONE.pcmd.pitch, &ARDRONE.pcmd.gaz, &ARDRONE.pcmd.yaw.x,
			&ARDRONE.pcmd.yaw.y, &ARDRONE.pcmd.yaw.z);
	if(ret != 8){
		fprintf(stderr, "pgen_ardrone::cast_pcmd: scan miss\n");
	}
	return get_command_len(buf);
}






int pgen_ardrone::cast_ref(const char* buf){
	int ret = sscanf(buf, "AT*REF=%ld,%ld",
			&this->ARDRONE.ref.seq, &this->ARDRONE.ref.command);
	
	if(ret != 2){
		fprintf(stderr, "pgen_ardrone::cast_ref: scan miss\n");
	}
	return get_command_len(buf);
}





int pgen_ardrone::cast_configids(const char* buf){
	const char* p = buf;
	int len;

	char seq[256];
	char session[256];
	char user[256];
	char app[256];
	memset(seq, 0, sizeof(seq));
	memset(session, 0, sizeof(session));
	memset(user, 0, sizeof(user));
	memset(app, 0, sizeof(app));

	p += strlen("AT*CONFIG_IDS=");
	for(int i=0; *p!=','; i++, p++)
		seq[i] = *p;	

	p++;
	len = get_original_string(p, session);
	p += len + 1;
	len = get_original_string(p, user);
	p += len + 1;
	len = get_original_string(p, app);	
	p += len;

	this->ARDRONE.configids.seq = atoi(seq);
	strcpy(this->ARDRONE.configids.session, session);
	strcpy(this->ARDRONE.configids.user, user);
	strcpy(this->ARDRONE.configids.app, app);

	return get_command_len(buf);	
}




int pgen_ardrone::cast_anim(const char* buf){
	return 0;
}





int pgen_ardrone::cast_ftrim(const char* buf){
	return 0;
}





int pgen_ardrone::cast_config(const char* buf){
	const char* p = buf;
	int len;
	
	char seq[256];
	char name[256];
	char parameter[256];
	memset(seq, 0, sizeof(seq));
	memset(name, 0, sizeof(name));
	memset(parameter, 0, sizeof(parameter));

	p += strlen("AT*CONFIG=");
	for(int i=0; *p!=','; i++, p++)
		seq[i] = *p;	

	p++;
	len = get_original_string(p, name);
	p += len + 1;
	len = get_original_string(p, parameter);	
	p += len;

	this->ARDRONE.config.seq = atoi(seq);
	strcpy(this->ARDRONE.config.name, name);
	strcpy(this->ARDRONE.config.parameter, parameter);

	return get_command_len(buf);	
}




int pgen_ardrone::cast_led(const char* buf){
	return 0;
}
int pgen_ardrone::cast_comwdg(const char* buf){
	return 0;
}




int pgen_ardrone::cast_ctrl(const char* buf){
	int ret = sscanf(buf, "AT*CTRL=%ld,%ld,%ld", &this->ARDRONE.ctrl.seq, 
			&this->ARDRONE.ctrl.ctrlmode, &this->ARDRONE.ctrl.fw_update_filesize);
	if(ret != 3){
		fprintf(stderr, "pgen_ardrone::cast_ctrl: scan miss (%d)\n", ret);
	}
	return get_command_len(buf);
}





void pgen_ardrone::cast(const void* packet, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_tcp::cast(): packet len isn`t support (%d)\n", len);
		return;
	}
	pgen_udp::cast(packet, len);
	
	int cmdlen;
	const char* p = (const char*)packet;
	p += ETH_HDR_LEN;
	p += IP_HDR_LEN;
	p += UDP_HDR_LEN;

	for(this->ARDRONE.cmd_count=0; (const u_char*)p-(const u_char*)packet < len; this->ARDRONE.cmd_count++){
		if(strncmp(p, "AT*PCMD", 7) == 0){
			this->ARDRONE.cmd_type[this->ARDRONE.cmd_count] = ARDRONE_CMD_PCMD;
			cmdlen = cast_pcmd(p);	
			p += cmdlen + 1;
		}else if(strncmp(p, "AT*REF", 6) == 0){
			this->ARDRONE.cmd_type[this->ARDRONE.cmd_count] = ARDRONE_CMD_REF;
			cmdlen = cast_ref(p);
			p += cmdlen + 1;
		}else if(strncmp(p, "AT*CONFIG_IDS", 13) == 0){
			this->ARDRONE.cmd_type[this->ARDRONE.cmd_count] = ARDRONE_CMD_CONFIG_IDS;
			cmdlen = cast_configids(p);
			p += cmdlen + 1;
		}else if(strncmp(p, "AT*ANIM", 7) == 0){
			this->ARDRONE.cmd_type[this->ARDRONE.cmd_count] = ARDRONE_CMD_ANIM;
			cmdlen = cast_anim(p);
			p += cmdlen + 1;
		}else if(strncmp(p, "AT*FTRIM", 8) == 0){
			this->ARDRONE.cmd_type[this->ARDRONE.cmd_count] = ARDRONE_CMD_FTRIM;
			cmdlen = cast_ftrim(p);
			p += cmdlen + 1;
		}else if(strncmp(p, "AT*CONFIG=", 10) == 0){
			this->ARDRONE.cmd_type[this->ARDRONE.cmd_count] = ARDRONE_CMD_CONFIG;
			cmdlen = cast_config(p);
			p += cmdlen + 1;
		}else if(strncmp(p, "AT*LED", 6) == 0){
			this->ARDRONE.cmd_type[this->ARDRONE.cmd_count] = ARDRONE_CMD_LED;
			cmdlen = cast_led(p);
			p += cmdlen + 1;
		}else if(strncmp(p, "AT*COMWDG", 9) == 0){
			this->ARDRONE.cmd_type[this->ARDRONE.cmd_count] = ARDRONE_CMD_COMWDG;
			cmdlen = cast_comwdg(p);
			p += cmdlen + 1;
		}else if(strncmp(p, "AT*CTRL", 7) == 0){
			this->ARDRONE.cmd_type[this->ARDRONE.cmd_count] = ARDRONE_CMD_CTRL;
			cmdlen = cast_ctrl(p);
			p += cmdlen + 1;
		}else{
			fprintf(stderr, "pgen_ardrone::cast: command type not found\n");
			return ;
		}
	}
	
}





void pgen_ardrone::summary(){
	compile();
	printf("AR Drone { ");
	
	for(int i=0; i<this->ARDRONE.cmd_count; i++){
		switch(this->ARDRONE.cmd_type[i]){
			case ARDRONE_CMD_PCMD:
				printf("PCMD(seq=%ld flag=%ld roll=%ld pitch=%ld gaz=%ld yaw=%ld,%ld,%ld) ", 
						ARDRONE.pcmd.seq, ARDRONE.pcmd.flag, ARDRONE.pcmd.roll, 
						ARDRONE.pcmd.pitch, ARDRONE.pcmd.gaz,
						ARDRONE.pcmd.yaw.x, ARDRONE.pcmd.yaw.y,
						ARDRONE.pcmd.yaw.z);
				break;
			case ARDRONE_CMD_REF:
				printf("REF(seq=%ld cmd=%ld) ", ARDRONE.ref.seq, ARDRONE.ref.command);
				break;
			case ARDRONE_CMD_CONFIG_IDS:
				printf("CONFIG_IDS(seq=%ld session=%s user=%s app=%s) ",
						ARDRONE.configids.seq, ARDRONE.configids.session, 
						ARDRONE.configids.user, ARDRONE.configids.app);
				break;
			case ARDRONE_CMD_ANIM:
				printf("ANIM(not implementation)\n"); exit(-1);
				break;
			case ARDRONE_CMD_FTRIM:
				printf("FTRIM(not implementation) \n"); exit(-1);
				break;
			case ARDRONE_CMD_CONFIG:
				printf("CONFIG(seq=%ld name=%s parameter=%s) ",
						ARDRONE.config.seq, ARDRONE.config.name, ARDRONE.config.parameter);
				break;
			case ARDRONE_CMD_LED:
				printf("LED(not implementation)\n"); exit(-1);
				break;
			case ARDRONE_CMD_COMWDG:
				printf("COMWDG(not implementation)\n"); exit(-1);
				break;
			case ARDRONE_CMD_CTRL:
				printf("CTRL(seq=%ld mode=%ld fwupfilezie=%ld) ", ARDRONE.ctrl.seq, 
						ARDRONE.ctrl.ctrlmode, ARDRONE.ctrl.fw_update_filesize);
				break;
			default:
				fprintf(stderr, "pgen_ardrone::summary: command type not found \n");
				break;
		}
	}
	printf("}\n");
}




void pgen_ardrone::info(){
	compile();
	pgen_udp::info();
	
	printf(" * AR Drone packet\n");
	
	
	for(int i=0; i<this->ARDRONE.cmd_count; i++){
		switch(this->ARDRONE.cmd_type[i]){
			case ARDRONE_CMD_PCMD:
				printf("    - PCMD MAG\n");
				printf("         Sequence Num : %ld \n", ARDRONE.pcmd.seq);
				printf("         Flag         : %ld \n", ARDRONE.pcmd.flag);
				printf("         Roll         : %ld \n", ARDRONE.pcmd.roll);
				printf("         Pitch        : %ld \n", ARDRONE.pcmd.pitch);
				printf("         Gaz          : %ld \n", ARDRONE.pcmd.gaz);
				printf("         Yaw(x,y,z)   : (%ld,%ld,%ld)  \n", 
						ARDRONE.pcmd.yaw.x, ARDRONE.pcmd.yaw.y,
						ARDRONE.pcmd.yaw.z);
				break;
			case ARDRONE_CMD_REF:
				printf("    - REF\n");
				printf("         Sequence Num : %ld \n", ARDRONE.ref.seq);
				printf("         Command      : %ld \n", ARDRONE.ref.command);
				break;
			case ARDRONE_CMD_CONFIG_IDS:
				printf("    - CONFIG_IDS \n");
				printf("         Sequence Num : %ld \n", ARDRONE.configids.seq);
				printf("         Session      : %s \n", ARDRONE.configids.session);
				printf("         User         : %s \n", ARDRONE.configids.user);
				printf("         App          : %s \n", ARDRONE.configids.app);
				break;
			case ARDRONE_CMD_ANIM:
				printf("    - ANIM \n");
				printf("         Not implement yet \n");
				break;
			case ARDRONE_CMD_FTRIM:
				printf("    - FTRIM \n");
				printf("         Not implement yet \n");
				break;
			case ARDRONE_CMD_CONFIG:
				printf("    - CONFIG \n");
				printf("         Sequence Num : %ld \n", ARDRONE.config.seq);
				printf("         Name         : %s \n", ARDRONE.config.name);
				printf("         Parameter    : %s \n", ARDRONE.config.parameter);
				break;
			case ARDRONE_CMD_LED:
				printf("    - LED \n");
				printf("         Not implement yet \n");
				break;
			case ARDRONE_CMD_COMWDG:
				printf("    - COMWDG \n");
				printf("         Not implement yet \n");
				break;
			case ARDRONE_CMD_CTRL:
				printf("    - CTRL \n");
				printf("         Sequence Num : %ld \n", ARDRONE.ctrl.seq);
				printf("         Control Mode : %ld \n", ARDRONE.ctrl.ctrlmode);
				printf("         FW Update    : %ld \n", ARDRONE.ctrl.fw_update_filesize);
				break;
			default:
				fprintf(stderr, "pgen_ardrone::summary: command type not found \n");
				break;
		}
	}
}




void pgen_ardrone::DSUMMARY(){
	compile();

	printf("%s -> %s ", IP.src.c_str(), IP.dst.c_str());
	summary();
}




static int get_original_string(const char* p, char buf[]){
	const char* c = p;
	c++;
	int i;
	for(i=0; *c != '"'; i++,c++){
		buf[i] = *c;
	}
	c++;
	i++;
	buf[i] = '\0';
	return c - p;	
}





static int get_command_len(const void* p){
	const char* c = (const char*)p;
	const char spliter = 0x0d;
	int len;
	for(len=0; c[len]!=spliter; len++) continue;
	
	return len;
}


void pgen_ardrone::help(){
	printf("ar_drone Packet CLass-----------------------------------------------------------\n");
	printf("pcmd.seq                : pcmd sequence number             : long value \n");
	printf("pcmd.flag               : pcmd flags                       : long value \n");
	printf("pcmd.roll               : pcmd roll                        : long value \n");
	printf("pcmd.pitch              : pcmd pitch                       : long value \n");
	printf("pcmd.gaz                : pcmd gaz                         : long value \n");
	printf("pcmd.yaw.x              : pcmd yaw x                       : long value \n");
	printf("pcmd.yaw.y              : pcmd yaw y                       : long value \n");
	printf("pcmd.yaw.z              : pcmd yaw z                       : long value \n");
	printf("ref.seq                 : ref  sequence number             : long value \n");
	printf("ref.command             : ref  command                     : long value \n");
	printf("configids.seq;          : configids sequence number        : long value \n");
	printf("configids.session[256]  : configids current session id     : char array[256] \n");
	printf("configids.user[256]     : configids current user id        : char array[256] \n");
	printf("configids.app[256]      : configids current application id : char array[256] \n");
	printf("config.seq              : config sequence number           : long value \n");
	printf("config.name             : config option name               : char array[256] \n");
	printf("config.parameter        : config option parameter          : char array[256] \n");
	printf("ctrl.seq                : ctrl sequence number             : long value \n");
	printf("ctrl.ctrlmode           : ctrl control mode                : long value \n");
	printf("ctrl.fw_update_filesize : ctrl firmware update file size   : long value \n");
	printf("\n");
	printf("cmd_count               : command count                    : int value \n");
	printf("cmd_type[10]            : command type array               : int array[10] \n");
	printf("---------------------------------------------------------------------------------\n");
}
