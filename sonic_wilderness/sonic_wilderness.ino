// sonic wilderness mushroom synth
// ver. 1.0
// inge vanden kroonenberg and jacob remin
// haliuoto 2016

int speaker1 = 3, speaker2 = 4;
int pot1 = A0, pot2 = A1, mushroomProbe = A5;
int led = 13;
int phaseCounter = 0, phase1 = 32, phase2 = 71;

int modes = 4;

float nature = 0, natureFlux = 0;
float easing = 0.1;

void setup() {
  pinMode(speaker1, OUTPUT);
  pinMode(speaker2, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);

  nature = analogRead(mushroomProbe);
}

void loop() {

  int p1 = analogRead(pot1);
  int p2 = analogRead(pot2);
  int rawNature = analogRead(mushroomProbe);
  
  natureFlux = float(rawNature) - nature;
  nature = nature + natureFlux * easing;
  
  Serial.print("raw nature "); Serial.println(rawNature);
  Serial.print("nature flux "); Serial.println(natureFlux);
  Serial.print("nature "); Serial.println(nature);
  
  // select active mode on pot 1
  int mode = map(p1, 0, 1023, 0, modes);
  Serial.print("mode "); Serial.println(mode);
  int modeMin = mode * 1023 / modes;
  int modeMax = (mode + 1) * 1023 / modes;
  
  //Serial.print("modeMin ");Serial.println(modeMin);
  //Serial.print("modeMax ");Serial.println(modeMax);

  switch (mode) {
    case 0:
      {
        digitalWrite(led, HIGH);

        int fluxPhase = int(natureFlux * 100) % phase2;
        if (phaseCounter % phase1 == 0) digitalWrite(speaker1, HIGH);
        if (phaseCounter % fluxPhase == 0) digitalWrite(speaker2, HIGH);

        int randomSpeed = random(15, 500);

        int del = map (p2, 0, 1023, 0, randomSpeed);
        del = del * abs(natureFlux);
        Serial.print("del "); Serial.println(del);
        delay(del);
        digitalWrite(speaker1, LOW);
        digitalWrite(speaker2, LOW);
        digitalWrite(led, LOW);
        delayMicroseconds(abs(natureFlux) * 100);

        int randomPause = random(1000);
        if (randomPause < 150) delay(random(100, 300));
        if (randomPause < 3) delay(random(1000, 10000));

        if (randomPause > 995) {
          int ranLen = random(10)*random(3);
          for (int i = 0; i < ranLen; i++) {
            delay(p2);
            digitalWrite(speaker1, LOW);
            digitalWrite(speaker2, LOW);
            delay(p2);
            digitalWrite(speaker1, HIGH);
            digitalWrite(speaker2, HIGH);
            delayMicroseconds(natureFlux * 400);
          }
        }

        phaseCounter++;
        break;
      }
    case 1:
      delay(p2);
      digitalWrite(speaker1, LOW);
      digitalWrite(speaker2, LOW);
      delay(p2);
      digitalWrite(speaker1, HIGH);
      digitalWrite(speaker2, HIGH);
      delayMicroseconds(natureFlux * 400);
      break;
    case 2:
      {
        int ran = random (100);
        if ((ran + int(natureFlux * 10)) % phase1 == 0) digitalWrite(speaker1, HIGH);
        if ((ran + int(natureFlux * 10)) % phase2 == 0) digitalWrite(speaker2, HIGH);
        int del = map (p1, modeMin, modeMax, 0, 200);
        delay(del);
        phaseCounter++;
        if (ran % phase1 == 0) digitalWrite(speaker1, LOW);
        if (ran % phase2 == 0) digitalWrite(speaker2, LOW);
        break;
      }
    case 3:
      {
        digitalWrite(led, HIGH);
        int pRan = random(0, 3);
        if (phaseCounter % (phase1 + pRan) == 0) digitalWrite(speaker1, HIGH);
        if (phaseCounter % phase2 == 0) digitalWrite(speaker2, HIGH);

        int del = map (p1, modeMin, modeMax, 0, 200);
        Serial.print("del "); Serial.println(del);
        delay(del);
        digitalWrite(speaker1, LOW);
        digitalWrite(speaker2, LOW);
        digitalWrite(led, LOW);
        delayMicroseconds(abs(natureFlux) * 100);
        //delay(p2);

        phaseCounter++;
        break;
      }
  }
}
