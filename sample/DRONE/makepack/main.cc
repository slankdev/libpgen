#include <pgen.h>
#include <stdio.h>

const char* dev = "lo";
const int count = 3;


int main(){
	pgen_ardrone pack;

	pack.IP.src = "192.168.1.3";
	pack.IP.dst = "192.168.1.1";
	pack.UDP.srcPort = 5556;
	pack.UDP.dstPort = 5556;
	
	pack.ARDRONE.pcmd.seq   = 1;
	pack.ARDRONE.pcmd.flag  = 0;
	pack.ARDRONE.pcmd.roll  = 0;
	pack.ARDRONE.pcmd.pitch = 0;
	pack.ARDRONE.pcmd.gaz   = 0;
	pack.ARDRONE.pcmd.yaw.x = 0;
	pack.ARDRONE.pcmd.yaw.y = 0;
	pack.ARDRONE.pcmd.yaw.z = 0;
	pack.ARDRONE.ref.seq        = 0;
	pack.ARDRONE.ref.command    = 0;

	/*
	pack.ARDRONE.pcmd.seq = 1000000110;
	pack.ARDRONE.pcmd.flag = 220;
	pack.ARDRONE.pcmd.roll = 330;
	pack.ARDRONE.pcmd.pitch = 440;
	pack.ARDRONE.pcmd.gaz = 550;
	pack.ARDRONE.pcmd.yaw.x = 660;
	pack.ARDRONE.pcmd.yaw.y = 770;
	pack.ARDRONE.pcmd.yaw.z = 880;
	pack.ARDRONE.ref.seq = 111;
	pack.ARDRONE.ref.command = 222;
	*/

	//pack.INFO();
	for(int i=0; ; i++){
		pack.SEND(dev);
		pack.SUMMARY();
		printf("%05d: ", i);
		pack.ARDRONE.pcmd.seq++;
		//sleep(count);
	}
}
