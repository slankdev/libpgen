#include <pgen.h>
#include <stdio.h>

const char* dev = "wlan0";

int main(){
	pgen_ardrone pack;

	pack.IP.src = "192.168.1.2";
	pack.IP.dst = "192.168.1.1";
	pack.UDP.srcPort = 5556;
	pack.UDP.dstPort = 5556;
	
	pack.ARDRONE.pcmd_mag.seq = 111;
	pack.ARDRONE.pcmd_mag.flag = 222;
	pack.ARDRONE.pcmd_mag.roll = 333;
	pack.ARDRONE.pcmd_mag.pitch = 444;
	pack.ARDRONE.pcmd_mag.gaz = 555;
	pack.ARDRONE.pcmd_mag.yaw1 = 666;
	pack.ARDRONE.pcmd_mag.yaw2 = 777;
	pack.ARDRONE.pcmd_mag.yaw3 = 888;
	pack.ARDRONE.ref.seq = 111;
	pack.ARDRONE.ref.command = 222;


	pack.SEND(dev);
}
