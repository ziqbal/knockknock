/*

KnockKnock with sequence memory

This experiment uses a piezo buzzer as a vibration sensor wired to an analog input.
Don't get confused with the piezo sensor as input and piezo buzzer as output!
Code must be changed if you use a standard button on a regular digital pin.
Also, be sure what kind of sound buzzer you have, is it an active or passive sound buzzer?
This experiment uses a passive sound buzzer.

Zafar Iqbal, Vagelis Chantzis
Arduino Day 2017
Athens, Greece

*/

// Setup input and output pins
int buzzerrPin = 9 ;
int sensorPin = 0 ;
int ledPin = 13 ;

// Threshold for knock sensor
int threshold = 1 ;

// Vars to keep track of current and previous state of sensor
int lastIsTrigger = 0 ;
int isTrigger = 0 ;

// Array to hold sequence of tap delays
#define MAXTAPS 100
int sequence[ MAXTAPS ] ;

// Vars used to handle tap times and sequence position
unsigned long taptime ; // !!!
unsigned long delta ; // !!!
int tappos;

// Var to control state
// 0 = Wait 1 = Record 2 = Play
int mode ;

void setup( ) {

    // Configure input/output pins
    pinMode( buzzerrPin , OUTPUT ) ;
    pinMode( ledPin , OUTPUT ) ;

    // Switch to Wait state
    mode = 0 ; 

}

void loop( ) {

    // Wait State
    if( mode == 0 ) {

        // If triggered then initialise variables and switch to record state
        if( isTriggered( ) ) {

            for( int i = 0 ; i < MAXTAPS ; i++ ) {

                sequence[ i ] = 0 ;

            }

            tappos = 0 ;
            taptime = millis( ) ;

            digitalWrite( ledPin , HIGH ) ;

            mode = 1 ;

        }

    }

    // Record State
    if( mode == 1 ) {

        // Calulate the time since last tap
        delta = millis( ) - taptime ;

        // If more than Xms of no trigger then switch to play state
        if( delta  > 1000 ) {

            mode = 2 ;

        }

        // If triggered then record delta in sequence
        if( isTriggered( ) ) {

            sequence[ tappos++ ] = delta ;
            taptime += delta ;

            // If we reach max sequence position then change to play state
            if( tappos == MAXTAPS ) {

                mode = 2 ;

            }

        }

    }


    // Play State
    if( mode == 2 ) {

        digitalWrite( ledPin , LOW ) ;

        // Important first beep!
        beep( ) ;

        // Loop through sequence and beep with correct delay
        for( int j = 0 ; j < tappos ; j++ ) {

            delay( sequence[ j ] ) ;

            beep( ) ;

        }

        // Back to wait state
        mode = 0 ;

    }

}

void beep( ) {

    switchon( );
    delay( 25 ) ;
    switchoff( ) ;

}

// Check for knock trigger and handle debounce as best as one can
bool isTriggered( ) {

    bool res = false ;

    if( analogRead( sensorPin ) > threshold ) {

        isTrigger = 1 ;

    } else {

        isTrigger = 0 ;

    }

    if( isTrigger == 0 && lastIsTrigger == 1 ) {

        res = true ;

    }

    lastIsTrigger = isTrigger ;

    delay( 25 ) ;

    return( res ) ;

}

void switchon( ) {

    tone( buzzerrPin , 1500 ) ;

    digitalWrite( ledPin , HIGH ) ;

}

void switchoff( ) {

    noTone( buzzerrPin ) ;

    digitalWrite( ledPin , LOW ) ;

}
