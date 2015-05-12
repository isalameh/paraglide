#ifndef LOG_H
#define LOG_H
#include <robotics_cape.h>
#define LOG_TABLE \
    X(unsigned long, "%lu", time)\
    X(float,  "%f", roll	) \
    X(float,  "%f", pitch	) \
    X(float,  "%f", yaw		) \
	X(float,  "%f", xa      ) \
	X(float,  "%f", ya      ) \
	X(float,  "%f", za      ) \
	X(float,  "%f", xg      ) \
	X(float,  "%f", yg      ) \
	X(float,  "%f", zg      ) \
	X(long, "%li", temperature_raw)\
	X(long, "%li", pressure_raw)\
	X(long, "%li", temperature)\
	X(long, "%li", pressure)

#define LOG_TABLE_RADIO \
    X(unsigned long, "%lu", time)\
    X(int,  "%d", Thr	) \
	X(int,  "%d", Roll	) \
	X(int,  "%d", Pitch	) \
	X(int,  "%d", Yaw	) \
	X(int,  "%d", Kill	) \
	X(int,  "%d", Mode	) \
	X(int,  "%d", Aux1	) \
	X(int,  "%d", Aux2	) \
	X(int,  "%d", Aux3	) 
	
	
#define X(type, fmt, name) type name ;
typedef struct log_entry_t { LOG_TABLE } log_entry_t;
typedef struct log_entry_r { LOG_TABLE_RADIO} log_entry_r;
#undef X
	
FILE* log_file;
FILE* log_file_r;
volatile log_entry_t log_buffer;
volatile log_entry_r log_buffer_r;
volatile int log_flag;

int log_write_entry(volatile log_entry_t* entry);
int log_write_entry_r(volatile log_entry_r* entry);
//void*  log_writer();
int log_start();
int log_stop();

#endif /*LOG_H*/