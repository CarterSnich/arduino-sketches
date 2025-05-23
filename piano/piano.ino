const int buzzer = 13;
const int cKey = 2;
const int dKey = 3;
const int eKey = 4;
const int fKey = 5;
const int gKey = 6;
const int aKey = 7;
const int bKey = 8;
const int CKey = 9;

int lastButton = 0;

void setup()
{
    pinMode(buzzer, OUTPUT);
    pinMode(cKey, INPUT);
}

void loop()
{
    if (digitalRead(cKey) == HIGH)
    {
        lastButton = cKey;
        tone(buzzer, 523.251); // C5
    }
    else if (lastButton == cKey && digitalRead(dKey) == HIGH)
    {
        tone(buzzer, 554.365); // C#5
    }
    else if (digitalRead(dKey) == HIGH)
    {
        lastButton = dKey;
        tone(buzzer, 587.330); // D5
    }
    else if (lastButton == dKey && digitalRead(eKey) == HIGH)
    {
        tone(buzzer, 622.254); // D5#
    }
    else if (digitalRead(eKey) == HIGH)
    {
        lastButton = eKey;
        tone(buzzer, 659.255); // E5
    }
    else if (digitalRead(fKey) == HIGH)
    {
        lastButton = fKey;
        tone(buzzer, 698.456); // F5
    }
    else if (lastButton == fKey && digitalRead(gKey) == HIGH)
    {
        tone(buzzer, 739.989); // F5#
    }
    else if (digitalRead(gKey) == HIGH)
    {
        lastButton = gKey;
        tone(buzzer, 783.991); // G5
    }
    else if (lastButton == gKey && digitalRead(aKey) == HIGH)
    {
        tone(buzzer, 830.609); // G5#
    }
    else if (digitalRead(aKey) == HIGH)
    {
        lastButton = aKey;
        tone(buzzer, 880.000); // A5
    }
    else if (lastButton == aKey && digitalRead(bKey) == HIGH)
    {
        tone(buzzer, 932.328); // A5#
    }
    else if (digitalRead(bKey) == HIGH)
    {
        lastButton = bKey;
        tone(buzzer, 987.767); // B5
    }
    else if (digitalRead(CKey) == HIGH)
    {
        lastButton = CKey;
        tone(buzzer, 1046.502); // C6
    }
    else
    {
        lastButton = 0;
        noTone(buzzer);
    }
}
