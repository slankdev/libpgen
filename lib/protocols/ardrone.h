

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

#ifndef ARDRONE_H
#define ARDRONE_H



#include "udp.h"


typedef enum{
	ARDRONE_CMD_PCMD,		// 0
	ARDRONE_CMD_REF,		// 1
	ARDRONE_CMD_CONFIG_IDS, // 2
	ARDRONE_CMD_ANIM, 		// 3
	ARDRONE_CMD_FTRIM,		// 4
	ARDRONE_CMD_CONFIG, 	// 5
	ARDRONE_CMD_LED,		// 6
	ARDRONE_CMD_COMWDG,		// 7
	ARDRONE_CMD_CTRL		// 8
} ar_drone_cmdtype;

class pgen_ardrone : public pgen_udp {
	protected:
		char  pcmd_data[256];
		bit32 pcmd_data_len;
		char  ref_data[256];
		bit32 ref_data_len;
		char  configids_data[256];
		bit32 configids_data_len;
		char  anim_data[256];
		bit32 anim_data_len;
		char  ftrim_data[256];
		bit32 ftrim_data_len;
		char  config_data[256];
		bit32 config_data_len;
		char  led_data[256];
		bit32 led_data_len;
		char  comwdg_data[256];
		bit32 comwdg_data_len;
		char  ctrl_data[256];
		bit32 ctrl_data_len;
	public:
		static const int minLength = pgen_udp::minLen+39; // minimum ardrone packet
		static const int macLength = PGEN_MAX_PACKET_LEN;
		struct{
			struct{
				long seq;
				long flag;
				long roll;
				long pitch;
				long gaz;
				struct{
					long x;
					long y;
					long z;
				}yaw;
			}pcmd;
			struct{
				long seq;
				long command;
			}ref;
			struct{
				long seq;
				char session[256];
				char user[256];
				char app[256];
			}configids;
			struct{}anim;
			struct{}ftrim;
			struct{
				long seq;
				char name[256];
				char parameter[256];
			}config;
			struct{}led;
			struct{}comwdg;
			struct{
				long seq;
				long ctrlmode;
				long fw_update_filesize;
			}ctrl;
			
			int cmd_count;
			int cmd_type[10];
		}ARDRONE;

		pgen_ardrone();
		pgen_ardrone(const void*, int);
		void clear();
		void compile();
		void cast(const void*, const int);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();
		void help();
	
		int compile_pcmd();
		int compile_ref();
		int compile_configids();
		int compile_anim();
		int compile_ftrim();
		int compile_config();
		int compile_led();
		int compile_comwdg();
		int compile_ctrl();
		int cast_pcmd(const char*);
		int cast_ref(const  char*);
		int cast_configids(const char*);
		int cast_anim(const char*);
		int cast_ftrim(const char*);
		int cast_config(const char*);
		int cast_led(const char*);
		int cast_comwdg(const char*);
		int cast_ctrl(const char*);
		void clear_pcmd();
		void clear_ref();
		void clear_configids();
		void clear_anim();
		void clear_ftrim();
		void clear_config();
		void clear_led();
		void clear_comwdg();
		void clear_ctrl();

		void DSUMMARY();
};




#endif
