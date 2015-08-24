#include <pgen.h>
#include <stdio.h>

const char* dev = "wlan0";


int main(){
	pgen_ardrone pack;
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = "192.168.179.1";
	pack.UDP.src = 5556;
	pack.UDP.dst = 5556;

	pack.ARDRONE.pcmd.seq = 0;
	pack.ARDRONE.pcmd.flag = 0;
	pack.ARDRONE.pcmd.roll = 0;
	pack.ARDRONE.pcmd.pitch = 0;
	pack.ARDRONE.pcmd.gaz = 0;
	pack.ARDRONE.pcmd.yaw.x = 0;
	pack.ARDRONE.pcmd.yaw.y = 0;
	pack.ARDRONE.pcmd.yaw.z = 0;
	pack.ARDRONE.ref.seq = 0;
	pack.ARDRONE.ref.command = 0;

	pack.compile();
	pack.summary();
	pack.send(dev);
}
