
#include "gyro_mov.hpp"


void turn(sensor_gyro_t & Gyro4){
	int control;
	while(true){
		usleep(BASE_SLEEP);
		control = PIDcontrol(Gyro4);
		if(control == 0) return;
		BP.set_motor_power(PORT_B, control/* + MOTORSPEED*/);
		BP.set_motor_power(PORT_C, (-1 * control)/* - MOTORSPEED*/);
	}
}

void goright(sensor_gyro_t & Gyro4){
	right_sound();
	baseline += 90;
	turn(Gyro4);
}

void goleft(sensor_gyro_t & Gyro4){
	left_sound();
	baseline -= 90;
	turn(Gyro4);
}

void turnaround(sensor_gyro_t & Gyro4){
	baseline -= 180;
	turn(Gyro4);
}

void brake(){
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
}
void object(sensor_gyro_t & Gyro4, sensor_ultrasonic_t & Ultrasonic3, const int32_t& encoderVerschil1, const int32_t& encoderVerschil2) {
	BP.get_sensor(PORT_3, &Ultrasonic3);
	int32_t count250s = 0;
	while (Ultrasonic3.cm < 10) {
		// gaat naar links
		goleft(Gyro4);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		usleep(BASE_SLEEP);
		
		// rijd 250
		int32_t EncoderC = BP.get_motor_encoder(PORT_C);
		int32_t EncoderB = BP.get_motor_encoder(PORT_B);
		int32_t encoder1 = EncoderC + 250;
		int32_t encoder2 = EncoderB + 250;

		while (EncoderC <= encoder1 && EncoderB <= encoder2) {
			EncoderC = BP.get_motor_encoder(PORT_C);
			EncoderB = BP.get_motor_encoder(PORT_B);
			int controlValue = PIDcontrol(Gyro4);
			BP.get_sensor(PORT_4, &Gyro4);
			BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
			BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
			usleep(BASE_SLEEP);
		}
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);

		usleep(BASE_SLEEP);

		//gaat naar rechts en checkt of er nog een object is
		goright(Gyro4);
		count250s++;
		BP.get_sensor(PORT_3, &Ultrasonic3);
	}

	//draait naar links en rijd nog 500 zodat de wielen het object niet raken
	goleft(Gyro4);
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	usleep(BASE_SLEEP);

	int32_t EncoderC = BP.get_motor_encoder(PORT_C);
	int32_t EncoderB = BP.get_motor_encoder(PORT_B);
	int32_t encoder1 = EncoderC + 500;
	int32_t encoder2 = EncoderB + 500;

	while (EncoderC <= encoder1 && EncoderB <= encoder2) {
		int controlValue = PIDcontrol(Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(BASE_SLEEP);
	}
	//draait naar rechts
	goright(Gyro4);
	count250s += 2;

	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	usleep(BASE_SLEEP);

	//rijd 500 naar voren en draait naar rechts
	EncoderC = BP.get_motor_encoder(PORT_C);
	EncoderB = BP.get_motor_encoder(PORT_B);
	encoder1 = EncoderC + 500;
	encoder2 = EncoderB + 500;

	while (EncoderC <= encoder1 && EncoderB <= encoder2) {
		int controlValue = PIDcontrol(Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(BASE_SLEEP);
	}
	goright(Gyro4);
	int count250sforward = 0;
	count250sforward += 2;

	//checkt sensor rijd door en checkt de sensor weer
	BP.get_sensor(PORT_3, &Ultrasonic3);
	while (Ultrasonic3.cm < 15) {
		//draait naar links
		goleft(Gyro4);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		usleep(BASE_SLEEP);
		// rijd 250
		int32_t EncoderC = BP.get_motor_encoder(PORT_C);
		int32_t EncoderB = BP.get_motor_encoder(PORT_B);
		int32_t encoder1 = EncoderC + 250;
		int32_t encoder2 = EncoderB + 250;

		while (EncoderC <= encoder1 && EncoderB <= encoder2) {
			EncoderC = BP.get_motor_encoder(PORT_C);
			EncoderB = BP.get_motor_encoder(PORT_B);
			int controlValue = PIDcontrol(Gyro4);
			BP.get_sensor(PORT_4, &Gyro4);
			BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
			BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
			usleep(BASE_SLEEP);
		}
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		usleep(BASE_SLEEP);

		//gaat naar rechts en kijkt of er nog een object is
		goright(Gyro4);
		count250sforward++;
		BP.get_sensor(PORT_3, &Ultrasonic3);
	}

	//draait naar links en rijd nog 500
	goleft(Gyro4);
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	usleep(BASE_SLEEP);

	EncoderC = BP.get_motor_encoder(PORT_C);
	EncoderB = BP.get_motor_encoder(PORT_B);
	encoder1 = EncoderC + 500;
	encoder2 = EncoderB + 500;

	while (EncoderC <= encoder1 && EncoderB <= encoder2) {
		int controlValue = PIDcontrol(Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(BASE_SLEEP);
	}
	count250sforward += 2;

	//doet de 250*aantal en rijd dit terug naar de lijn
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	usleep(BASE_SLEEP);
	count250s *= 250;

	goright(Gyro4);
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);

	EncoderC = BP.get_motor_encoder(PORT_C);
	EncoderB = BP.get_motor_encoder(PORT_B);
	encoder1 = EncoderC + count250s;
	encoder2 = EncoderB + count250s;

	while (EncoderC <= encoder1 && EncoderB <= encoder2) {
		int controlValue = PIDcontrol(Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(BASE_SLEEP);
	}

	//draait terug op de lijn de goeie kant op en rijd de overgebleven afstand af
	goleft(Gyro4);
	usleep(BASE_SLEEP);

	count250sforward *= 250;

	EncoderC = BP.get_motor_encoder(PORT_C);
	EncoderB = BP.get_motor_encoder(PORT_B);
	encoder1 = EncoderC + encoderVerschil1 - count250sforward;
	encoder2 = EncoderB + encoderVerschil2 - count250sforward;

	while (EncoderC <= encoder1 && EncoderB <= encoder2) {
		int controlValue = PIDcontrol(Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(BASE_SLEEP);
	}
}

void goforward_man(double distance, sensor_gyro_t & Gyro4){
	int controlValue;
	int32_t encoder1 = distance + BP.get_motor_encoder(PORT_C);
	int32_t encoder2 = distance + BP.get_motor_encoder(PORT_B);

	while(BP.get_motor_encoder(PORT_C) <= encoder1 && BP.get_motor_encoder(PORT_B) <= encoder2){
		BP.get_sensor(PORT_4, &Gyro4);
		controlValue = PIDcontrol(Gyro4);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(BASE_SLEEP);
	}
}

void goforward_auto(const int32_t & distance, sensor_gyro_t & Gyro4, sensor_ultrasonic_t & Ultrasonic3){
	int controlValue;
	int32_t encoder1 = distance * 250 + BP.get_motor_encoder(PORT_C);
	int32_t encoder2 = distance * 250 + BP.get_motor_encoder(PORT_B);

	while(BP.get_motor_encoder(PORT_C) <= encoder1 && BP.get_motor_encoder(PORT_B) <= encoder2){
		BP.get_sensor(PORT_4, &Gyro4);
		controlValue = PIDcontrol(Gyro4);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);

		BP.get_sensor(PORT_3, &Ultrasonic3);
		if(Ultrasonic3.cm < 10){
			int32_t encoderVerschil1 = encoder1 - BP.get_motor_encoder(PORT_C);
			int32_t encoderVerschil2 = encoder2 - BP.get_motor_encoder(PORT_B);
			object(Gyro4,Ultrasonic3, encoderVerschil1, encoderVerschil2);
		}
		usleep(BASE_SLEEP);
	}
}
