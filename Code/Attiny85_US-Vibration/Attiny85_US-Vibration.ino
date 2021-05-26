
#include <Ultrasonic.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

/*****************************************************************************/

#define pinEcho 0
#define pinTrigger 1
#define pinPot 2
#define pinMotor 3

/*****************************************************************************/

float d;
float rango;
int motor_on;
volatile int wd_counter = 0;
Ultrasonic s_ultrasonic(pinTrigger,pinEcho);

/*****************************************************************************/

ISR(WDT_vect)
{
	wd_counter++;
}

/*****************************************************************************/

void setup()
{
	pinMode(pinMotor, OUTPUT);
	analogWrite(pinMotor,0);
	motor_on = 0;
}

void loop()
{
	d = s_ultrasonic.Ranging(CM);
	rango = analogRead(pinPot);
	rango = map(rango,0,1023,0,155);

	if(d <= rango) // Objeto dentro del rango
	{
		if(motor_on == 0)
		{
			motor_on = 1;
			analogWrite(pinMotor,1023);
		}
	}

	if(d > rango) // Objeto fuera del rango
	{
		if(motor_on == 1)
		{
			motor_on = 0;
			analogWrite(pinMotor,0);
		}
	}

	wd_delay(250);
}

/*****************************************************************************/

// OJO: LA SENSIBILIDAD SERA DE 15MS (UN CAMBIO MENOR A 15MS NO SE DETECTA Y,
// EL VALOR MINIMO SERA DE 15MS)
void wd_delay(unsigned long t_ms)
{
	int num_veces = 0;

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	if(t_ms == 15)
	{
		wd_setup(WDTO_15MS);
		sleep_mode();
	}
	else if(t_ms == 30)
	{
		wd_setup(WDTO_30MS);
		sleep_mode();
	}
	else if(t_ms == 60)
	{
		wd_setup(WDTO_60MS);
		sleep_mode();
	}
	else if(t_ms == 120)
	{
		wd_setup(WDTO_120MS);
		sleep_mode();
	}
	else if(t_ms == 250)
	{
		wd_setup(WDTO_250MS);
		sleep_mode();
	}
	else if(t_ms == 500)
	{
		wd_setup(WDTO_500MS);
		sleep_mode();
	}
	else if(t_ms == 1000)
	{
		wd_setup(WDTO_1S);
		sleep_mode();
	}
	else if(t_ms == 2000)
	{
		wd_setup(WDTO_2S);
		sleep_mode();
	}
	else if(t_ms == 4000)
	{
		wd_setup(WDTO_4S);
		sleep_mode();
	}
	else if(t_ms == 8000)
	{
		wd_setup(WDTO_8S);
		sleep_mode();
	}
	else
	{
		num_veces = t_ms/15;
		while(wd_counter < num_veces)
		{
			wd_setup(WDTO_15MS);
			sleep_mode();
		}
	}

	wdt_disable();
	wd_counter = 0;
}

void wd_setup(int timerPrescaler)
{
	if ((timerPrescaler < 0) || (timerPrescaler > 10))
		timerPrescaler = WDTO_15MS;

	byte bb = timerPrescaler & 7;
	if(timerPrescaler > 7)
		bb |= (1<<5); // Establece los bits necesarios

	MCUSR &= ~(1<<WDRF); // Limpia el flag de reset del watchdog
	WDTCR |= (1<<WDCE) | (1<<WDE); // Activa el watchdog
	WDTCR = bb; // Establece el Timeout del watchdog
	WDTCR |= _BV(WDIE); // Activa la interrupcion del watchdog
}
