#include "global.h"
#include <robotics_cape.h>
#include "log.h"
#include "task.h"
#include "bmp180.h"
#include "ubloxlea6.h"

int main()
{
	/////////// Initialization////////////////////
	if(initialize_cape()<0){
		return -1;
	}
	
	if(log_start()<0){
		return -1;
	}
	log_flag = 0;
	set_pause_unpressed_func(&task_pause_unpress);
	
	printf("\nInitializing BAROMETER\n");
	if(bmp_init()<0)
	{
		printf("Barometer initialization failed!\n");
		cleanup_cape();
		return -1;
	}
	
	
	// printf("\nInitializing GPS\n");
	// if(ubloxlea6_init(1)<0)
	// {
		// printf("GPS initialization failed!\n");
		// cleanup_cape();
		// return -1;
	// }

	if(initialize_dsm2()){
		// if init returns -1 if there was a problem 
		// most likely no calibration file found
		printf("run calibrate_dsm2 first\n");
		return -1;
	}
	
	
	/////////// StartThreads///////////////////////
	signed char orientation[9] = ORIENTATION_FLAT;
	if(initialize_imu(SAMPLE_HZ, orientation)){
		printf("IMU initialization failed, please reboot\n");
		cleanup_cape();
		return -1;
	}
	set_imu_interrupt_func(&task_sample_imu);

		

	
	/////////// Main loop//////////////////////////
	while(get_state()!=EXITING)
	{
		usleep(100000);
		
	}
	
	/////////// Exit//////////////////////////////
	setGRN(0);
	setRED(0);
	cleanup_cape();
	log_stop();
	return 0;
}