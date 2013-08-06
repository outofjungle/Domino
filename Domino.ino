//
//  Domino
//  Turns on an LED by reading the output of a photocell
//

const int sensor_pin = A0;
const int led_pin = 11;
const int wait = 10;
const size_t history_size = 30;

int counter = 0;
int sensor_value = 0;
int sensor_history[ history_size ];

int duration = 2000;
int threshold = 20;

void setup() {
  for( int i = 0; i < history_size; i++ ) {
    sensor_history[ i ] = 0;
  }

  pinMode( sensor_pin, INPUT );
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
  sensor_value = analogRead( sensor_pin );
  int avg = get_average( sensor_history, history_size );
  int delta = get_delta( avg, sensor_value );
  
  statergy02( led_pin, delta, threshold, duration );
  
  counter = counter % history_size;
  sensor_history[ counter ] = sensor_value;
  counter++;

  print_history( sensor_history, history_size );
  
  delay( wait );
}

void statergy01( int led_pin, int delta, int threshold ) {
  if ( delta > threshold ) {
    digitalWrite( led_pin, HIGH ); 
  }
  
  if ( delta < ( -1 * threshold ) ) {
    digitalWrite( led_pin, LOW ); 
  }
}

void statergy02( int led_pin, int delta, int threshold, int duration ) {
  static long on_till = 0;
  if ( millis() > on_till ) {
    if ( delta > threshold ) {
      on_till = millis() + duration;
      digitalWrite( led_pin, HIGH ); 
    } else {
      digitalWrite( led_pin, LOW );
    }
  }
}

int get_average( int * history, size_t len ){
  int sum = 0;
  for( int i = 0; i < len; i++ ) {
    sum += history[ i ];
  }
  return sum/len;
}

int get_delta( int avg, int value ) {
  int diff = avg - value;
  float result = ( (float) diff / (float)avg ) * 100;
  return (int)result;
}

void print_history( int * history, size_t len ) {
  for( int i = 0; i < len; i++ ) {
    Serial.print( history[ i ] );
    Serial.print( "," );
  }
  Serial.println();
}
