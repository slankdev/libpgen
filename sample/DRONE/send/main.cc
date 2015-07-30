#include <pgen.h>
#include <stdio.h>

const char* dev = "wlan0";
const int count = 1;


int main(){
	pgen_ardrone pack;

	pack.IP.src.setipbydev(dev);
	pack.IP.src = "1.1.1.1";
	pack.IP.dst = "192.168.0.0";
	pack.UDP.src = 5556;
	pack.UDP.dst = 5556;

	pack.ARDRONE.pcmd.seq = 0;
	pack.ARDRONE.pcmd.flag = 0;
	pack.ARDRONE.pcmd.roll = 0;
	pack.ARDRONE.pcmd.pitch = 0;
	pack.ARDRONE.pcmd.gaz = -10000000;
	pack.ARDRONE.pcmd.yaw.x = 0;
	pack.ARDRONE.pcmd.yaw.y = 0;
	pack.ARDRONE.pcmd.yaw.z = 0;
	pack.ARDRONE.ref.seq = 0;
	pack.ARDRONE.ref.command = 0;

	//pack.INFO();
	for(int i=0; ; i++){
		printf("%05d: ", i);
  		pack.SEND(dev);
 	 	pack.SUMMARY();
		pack.ARDRONE.pcmd.seq++;
		sleep(count);
	}
}
