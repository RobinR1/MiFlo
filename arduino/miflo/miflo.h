#define SECONDS_PER_MINUTE 60
// THE LINE BELOW SHOULD OVERRIDE PUBSUBCLIENT'S BUT DOESN'T REALLY DO IT ...
#define MQTT_MAX_PACKET_SIZE 1024

#define COLOR_BEIGE 0xffe9ac
#define COLOR_GREEN 0x588751
#define COLOR_RED 0xd41942
#define COLOR_ORANGE 0xff6f25
#define COLOR_YELLOW 0xffc700
#define COLOR_BLUE 0x2b19d4
#define COLOR_BLACK 0x030302
#define COLOR_WHITE 0xffe9e6

#define GOED_AANTAL 16

void show_log();
void add_log( String message );
void sample();
void jingle( int n );
void parse_command( char* json );
void mqttCallback(char* topic, byte* payload, unsigned int length);
void mqttOnlineCheck();
void load_jpgs();
void setup();
String format_time( int hour, int minute, int second );
String format_time_space( int hour, int minute, int second );
String current_time();
void show_clock();
static void clock_ray(int centerx, int centery, int &x, int &y, int r, double i);
void show_time_timer( double minutes, int centerx, int centery, int scale );
int job2jpg( String job );
void display_time_timer( double minutes );
void run_time_timer();
void show_timer_finished();
void statusbar( int hour, int minute, int second );
void show_reminder(String job);
void show_alarm(String job);
void show_upcoming_events();
void show_goed_gedaan();
void show_todo_icon( int x, int y, int job, int tag );
void show_todo_icons();
void show_todo();
void loop();
