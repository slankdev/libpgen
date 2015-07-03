#include <pgen.h>
#include <stdio.h>

const char* dev = "wlan0";

int main(){
	pgen_ardrone pack;

	pack.IP.src = "192.168.1.3";
	pack.IP.dst = "192.168.1.1";
	pack.UDP.srcPort = 5556;
	pack.UDP.dstPort = 5556;
	
	pack.ARDRONE.pcmd_mag.seq   = 1;
	pack.ARDRONE.pcmd_mag.flag  = 0;
	pack.ARDRONE.pcmd_mag.roll  = 0;
	pack.ARDRONE.pcmd_mag.pitch = 0;
	pack.ARDRONE.pcmd_mag.gaz   = 0;
	pack.ARDRONE.pcmd_mag.yaw.x = 0;
	pack.ARDRONE.pcmd_mag.yaw.y = 0;
	pack.ARDRONE.pcmd_mag.yaw.z = 0;
	pack.ARDRONE.ref.seq        = 0;
	pack.ARDRONE.ref.command    = 0;

	/*
	pack.ARDRONE.pcmd_mag.seq = 1000000110;
	pack.ARDRONE.pcmd_mag.flag = 220;
	pack.ARDRONE.pcmd_mag.roll = 330;
	pack.ARDRONE.pcmd_mag.pitch = 440;
	pack.ARDRONE.pcmd_mag.gaz = 550;
	pack.ARDRONE.pcmd_mag.yaw.x = 660;
	pack.ARDRONE.pcmd_mag.yaw.y = 770;
	pack.ARDRONE.pcmd_mag.yaw.z = 880;
	pack.ARDRONE.ref.seq = 111;
	pack.ARDRONE.ref.command = 222;
	*/

	//pack.INFO();
	for(;;){
		pack.SEND(dev);
		pack.SUMMARY();
		sleep(1);
	}
}
