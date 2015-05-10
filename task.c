#include <robotics_cape.h>
#include "global.h"
#include "task.h"
#include "log.h"
#include "bmp180.h"

volatile int task_sample_counter = 0;
volatile int task_led_red = 0;
volatile int task_led_grn = 0;
volatile unsigned int bmp_temp = 0;
volatile unsigned long bmp_press = 0;
int task_sample_imu()
{
	mpudata_t mpu; //struct to read IMU data into
	memset(&mpu, 0, sizeof(mpudata_t)); //make sure it's clean before starting
	if (mpu9150_read(&mpu) == 0) {
		printf("\r");
		
		
		printf("X: %0.1f Y: %0.1f Z: %0.1f ",
		mpu.fusedEuler[VEC3_X] * RAD_TO_DEGREE, 
		mpu.fusedEuler[VEC3_Y] * RAD_TO_DEGREE, 
		mpu.fusedEuler[VEC3_Z] * RAD_TO_DEGREE);
		
		printf("Xg: %05d Yg: %05d Zg: %05d ",
		mpu.rawGyro[VEC3_X], 
		mpu.rawGyro[VEC3_Y], 
		mpu.rawGyro[VEC3_Z]);  
		
		printf("Xa: %05d Ya: %05d Za: %05d ",
		mpu.calibratedAccel[VEC3_X], 
		mpu.calibratedAccel[VEC3_Y], 
		mpu.calibratedAccel[VEC3_Z]);
		
		bmp_temp = bmp_read_temp();
		bmp_press = bmp_read_press();
		printf("TEMP: %u PRESS: %lu",bmp_temp,bmp_press);
		
		fflush(stdout);
		
		unsigned long *tmp;
		linux_get_ms(tmp);
		log_buffer.time = *tmp;
		log_buffer.pitch	= -mpu.fusedEuler[VEC3_X];
		log_buffer.roll =  mpu.fusedEuler[VEC3_Y];
		log_buffer.yaw 	=  mpu.fusedEuler[VEC3_Z];
		
		log_buffer.xa	= mpu.calibratedAccel[VEC3_X];
		log_buffer.ya =  mpu.calibratedAccel[VEC3_Y];
		log_buffer.za	=  mpu.calibratedAccel[VEC3_Z];
		
		log_buffer.xg	= mpu.rawGyro[VEC3_X];
		log_buffer.yg =  mpu.rawGyro[VEC3_Y];
		log_buffer.zg	=  mpu.rawGyro[VEC3_Z];
		
		log_buffer.temperature = bmp_temp;
		log_buffer.pressure = bmp_press;
		
		if(log_flag!=0)
			log_write_entry(&log_buffer);
		task_sample_counter++;
		
		if(task_sample_counter%SAMPLE_HZ==0)
		{
			task_led_red ^=1;
			task_led_grn ^=1;
			task_led_grn &=log_flag;
			setGRN(task_led_grn);
			setRED(task_led_red);
			task_sample_counter = 0;	

			if(is_new_dsm2_data()){	
				
				log_buffer_r.time = *tmp;
				log_buffer_r.Thr = get_dsm2_ch_raw(1);
				log_buffer_r.Roll = get_dsm2_ch_raw(2);
				log_buffer_r.Pitch = get_dsm2_ch_raw(3);
				log_buffer_r.Yaw = get_dsm2_ch_raw(4);
				log_buffer_r.Kill = get_dsm2_ch_raw(5);
				log_buffer_r.Mode = get_dsm2_ch_raw(6);
				log_buffer_r.Aux1 = get_dsm2_ch_raw(7);
				log_buffer_r.Aux2 = get_dsm2_ch_raw(8);
				log_buffer_r.Aux3 = get_dsm2_ch_raw(9);
				
				send_servo_pulse_us(1,log_buffer_r.Thr);
				send_servo_pulse_us(2,log_buffer_r.Roll);
				send_servo_pulse_us(3,log_buffer_r.Pitch);
				send_servo_pulse_us(4,log_buffer_r.Yaw);
				send_servo_pulse_us(5,log_buffer_r.Kill);
				send_servo_pulse_us(6,log_buffer_r.Mode);
				send_servo_pulse_us(7,log_buffer_r.Aux1);
				send_servo_pulse_us(8,log_buffer_r.Aux2);
				send_servo_pulse_us(9,log_buffer_r.Aux3);
				
				if(log_flag!=0)
					log_write_entry_r(&log_buffer);
			
				// printf("\r");
				// for(i=0;i<RC_CHANNELS;i++){
					// printf(" %d   ", get_dsm2_ch_raw(i+1));
				// }
				// fflush(stdout);
			}			
		}

		
	}
	return 0; 
}

int task_pause_unpress()
{
	log_flag^=1;
}