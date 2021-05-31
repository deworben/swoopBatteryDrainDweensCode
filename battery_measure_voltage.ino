
#define NUM_SAMPLES 10 // Used to find average reading
#define CAL1 5.015 // V dropped over r2 when measured with multimeter 
#define CAL2 11.132 // V dropped over full circuit when measured with multimeter. Approx (r2+r1)/r2. Calculate with v_total/v_r2

#define LEDPIN1 31
#define LEDPIN2 30
#define LEDPIN3 32


int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float r2_voltage = 0.0;         // calculated voltage over r2
float tot_voltage = 0.0;        // calculated voltage over whole circuit






void light_up(int num_active_leds){
  switch (num_active_leds) {
    
  case 0:
    digitalWrite(LEDPIN1, HIGH);
    digitalWrite(LEDPIN2, LOW);
    digitalWrite(LEDPIN3, LOW);
    Serial.println("we're low");
    break;
  case 1:
    digitalWrite(LEDPIN1, HIGH);
    digitalWrite(LEDPIN2, HIGH);
    digitalWrite(LEDPIN3, LOW);
    Serial.println("we're hiiiigh");
    break;
    
  default:
    digitalWrite(LEDPIN1, LOW);
    digitalWrite(LEDPIN2, LOW);
    digitalWrite(LEDPIN3, LOW);
    break;
  }
}

void display_leds(float tot_voltage){
  float num_leds = 2;
  float max_voltage = 38;
  float min_voltage = 30;
  float voltage_division_levels = (max_voltage-min_voltage)/num_leds;

  if( tot_voltage > min_voltage){
    int num_leds_active = floor((tot_voltage-min_voltage)/voltage_division_levels);
    Serial.println((String)"Num leds active = "+ num_leds_active);
    light_up(num_leds_active);
  }else{
    Serial.println((String)"Battery too low");
    light_up(0);
  }
    
}






void setup()
{
    Serial.begin(9600); 
    delay(2000);
    

    pinMode(LEDPIN1, OUTPUT);
    pinMode(LEDPIN2, OUTPUT); 
    pinMode(LEDPIN3, OUTPUT); 
}





void loop()
{

    // Sample a few voltage samples and take the average
    while (sample_count < NUM_SAMPLES) {
        sum += analogRead(A2);
        sample_count++;
        delay(10);
    }

    // CALIBRATION 1: Just calibrating the voltage over r2
    r2_voltage = ((float)sum / (float)NUM_SAMPLES * CAL1) / 1024.0;

    
    // CALIBRATION 2: divides by 11.132 is the calibrated voltage divide value bcs of voltage dividing circuit
    tot_voltage = r2_voltage * CAL2;
    Serial.println((String)""+tot_voltage+" V");

    display_leds(tot_voltage);

    // Reset variables
    sample_count = 0;
    sum = 0;
    
    delay(1000);
}
