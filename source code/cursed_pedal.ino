

/*A guitar "effects" pedal that pleads for mercy and then screams in pain when you play through it
 * 
 * TODO
 * 
 * change button to RMS guitar measurement and determine threshold (hopefully just a matter of plugging in)
 * maybe get some better samples for the pleading
 * 
 */

// screaming
#include "AudioSampleScreamattack.h"   
#include "AudioSampleScreamsustain.h" 
#include "AudioSampleScreamdecay.h" 


// pleading
#include "AudioSampleNo1.h"
#include "AudioSampleNo2.h"  
#include "AudioSampleNo3.h"  
#include "AudioSampleNo4.h"  
#include "AudioSampleNo5.h"  
#include "AudioSampleNo6.h"


#define delayFix 5 //number of milliseconds early next sample is played. accounts for delay in playing a sound. Should be small 
#define pleadRarity 200 // 6/ this number is the probability of saying one of the sad samples per tick

#include <elapsedMillis.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlayMemory          playMem2;       //xy=313,354
AudioInputI2S            i2s1;           //xy=322,251
AudioPlayMemory          playMem3;       //xy=351,396
AudioPlayMemory          playMem4;       //xy=371,442
AudioPlayMemory          playMem1;       //xy=386,312
AudioPlayMemory          playMem5;       //xy=400,527
AudioAnalyzeRMS          rms1;           //xy=519,262
AudioMixer4              mixer1;         //xy=581,364
AudioMixer4              mixer2;         //xy=699,454
AudioEffectFreeverb      freeverb1;      //xy=881,317
AudioMixer4              mixer3;         //xy=1039,417
AudioAmplifier           amp1;           //xy=1188,377
AudioOutputI2S           i2s2;           //xy=1359,356
AudioConnection          patchCord1(playMem2, 0, mixer1, 1);
AudioConnection          patchCord2(i2s1, 0, rms1, 0);
AudioConnection          patchCord3(playMem3, 0, mixer1, 2);
AudioConnection          patchCord4(playMem4, 0, mixer1, 3);
AudioConnection          patchCord5(playMem1, 0, mixer1, 0);
AudioConnection          patchCord6(playMem5, 0, mixer2, 1);
AudioConnection          patchCord7(mixer1, 0, mixer2, 0);
AudioConnection          patchCord8(mixer2, freeverb1);
AudioConnection          patchCord9(mixer2, 0, mixer3, 1);
AudioConnection          patchCord10(freeverb1, 0, mixer3, 0);
AudioConnection          patchCord11(mixer3, amp1);
AudioConnection          patchCord12(amp1, 0, i2s2, 0);
AudioConnection          patchCord13(amp1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1002,591
// GUItool: end automatically generated code



//loop for detection of guitar signal
elapsedMillis timeElapsed;
unsigned int looptime = 50;

int makeNoise = 0;
int silent = 1;
unsigned long pleadBlock = 0;

float rmsVal;

void setup() {
  Serial.begin(9600);
  
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(1); //for testing headphones - doesn't control line out

  sgtl5000_1.unmuteLineout(); //must use line out rather than headphone jack to amplifier!
  sgtl5000_1.muteHeadphone(); 
  
  mixer1.gain(0, 1); //different bits of the scream
  mixer1.gain(1, 1);
  mixer1.gain(2, 1);
  mixer1.gain(3, 1);

  mixer2.gain(0, 1); //screaming
  mixer2.gain(1, 0.5);  //pleading

  mixer3.gain(0, 0.3); //reverb channel
  mixer3.gain(1, 1);    //dry channel
  
  freeverb1.roomsize(0.4);
  freeverb1.damping(0.2);

  amp1.gain(0.1); 

  randomSeed(analogRead(16)); //seed random pleading
}



void loop() {
  
  if (timeElapsed > looptime) {
    
    int knob = analogRead(A1); //check volume knob
    float vol = ((float)knob / 1280.0);
    amp1.gain(vol);
    plead(); //a chance to cry for help :(
    
    rmsVal = rms1.read(); //rms value of input signal over last looptime. 
    Serial.print(vol);
    Serial.print(" ");
    Serial.println(rmsVal, 5);

    if(rmsVal > 0.002){ //pin for output testing. Will use for seeding random pleas later
      makeNoise = 1;  
    }
    else{
      makeNoise = 0;
    }
    
    timeElapsed = 0; //reset timer
    ++pleadBlock; // 
  }
    
  if (makeNoise == 1){ 
    startPlaying();
  }
  else{
    stopPlaying();
  }
    
}



void startPlaying() {
  //start SCREAMING BLOODY MURDER
  playMem5.stop();
  
  if (silent == 1){
    playMem1.play(AudioSampleScreamattack);
    silent = 0;
  }
  else{ // a way to get a relatively smooth loop
    if(!playMem2.isPlaying() && !playMem3.isPlaying() && playMem1.lengthMillis() - playMem1.positionMillis() < delayFix){
      playMem2.play(AudioSampleScreamsustain);
    }
    if(playMem2.isPlaying() && playMem2.lengthMillis() - playMem2.positionMillis() < delayFix && !playMem3.isPlaying()){
      playMem3.play(AudioSampleScreamsustain);
    }
    if(playMem3.isPlaying() && playMem3.lengthMillis() - playMem3.positionMillis() < delayFix && !playMem2.isPlaying()){
      playMem2.play(AudioSampleScreamsustain);
    }       
  } 
  pleadBlock = 0; //"timer" that stops pleading from happening again  
}





void stopPlaying() {
  // okay that's enough SCREAMING
  if (silent == 0){
    if(playMem1.isPlaying() && playMem1.lengthMillis() - playMem1.positionMillis() < delayFix){
      playMem4.play(AudioSampleScreamdecay);
      silent = 1; 
    }
    if(playMem2.isPlaying() && playMem2.lengthMillis() - playMem2.positionMillis() < delayFix){
      playMem4.play(AudioSampleScreamdecay);
      silent = 1; 
    }
    if(playMem3.isPlaying() && playMem3.lengthMillis() - playMem3.positionMillis() < delayFix){
      playMem4.play(AudioSampleScreamdecay);
      silent = 1; 
    }    
  }    
}




void plead() {
  long randPlea = random(pleadRarity); //chance of using one of the voice lines is 6 / pleadRarity per tick (if there's not a line already playing)
  if (!playMem5.isPlaying() && !playMem1.isPlaying() && !playMem2.isPlaying() && !playMem3.isPlaying() && !playMem4.isPlaying() && pleadBlock > 30){    
    if (randPlea == 1){
      playMem5.play(AudioSampleNo1);
    }
    if (randPlea == 2){
      playMem5.play(AudioSampleNo2);
    }
    if (randPlea == 3){
      playMem5.play(AudioSampleNo3);
    }
    if (randPlea == 4){
      playMem5.play(AudioSampleNo4);
    }
    if (randPlea == 5){
      playMem5.play(AudioSampleNo5);
    }
    if (randPlea == 6){
      playMem5.play(AudioSampleNo6);
    }
  }
}
