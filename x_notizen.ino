/*  
Arduino 1.8.18 version
Board information:
ESP32 WROOM

Library neoPixel 1,2,0

https://dl.espressif.com/dl/package_esp32_index.json

ESP32 DEV MODUL
 
 *   
 ****** WIND m/s
0  Windstille        0 
1  leiser Zug        1>
2  leichte Brise     3>
3  schwacher Wind    5>
4  mäßiger Wind      7>
5  frischer Wind    10>
6  starker Wind     13>
7  steifer Wind     17>
8  stürmischer Wind 20>
9  Sturm            24>
 * 
 * 
 ******WOLKEN
0/8 = wolkenlos

1/8 = fast wolkenlos 12 %

2/8= heiter          25%

3/8 = leicht bewölkt 37%

4/8 = wolkig         50%

5/8 = bewölkt        62%

6/8 = stark bewölkt  75%

7/8 = fast bedeckt   87%

8/8 = bedeckt        100%

 ******* REGEN Rain volume for the last 1 hour, mm
leichter Sprühregen: < 0,1 mm 
mäßiger Sprühregen: ≥ 0,1 mm
starker Sprühregen: ≥ 0,5 mm
leichter Regen: < 2,5 mm 
mäßiger Regen: ≥ 2,5 mm bis < 10,0 mm
starker Regen: ≥ 10,0 mm
sehr starker Regen: ≥ 50,0 mm

 ******* SCHNEE Snow volume for the last 1 hour, mm
leichter Schneeschauer: < 0,2 mm (jeweils innerhalb von 10 Minuten)
mäßiger Schneeschauer: ≥ 0,2 mm bis < 0,9 mm
starker Schneeschauer: ≥ 0,9 mm


CO2 Gehalt im ppm  CO2 in %  Qualitätsstufe
Unter 800 <0,08 % Hohe Raumluftqualität
800 - 1.000 0,08 - 0,1 %  Mittlere Raumluftqualität
1.000 - 1.400

0,1 - 0,14 %  Mäßige Raumluftqualität
Über 1.400

>0,14 % Niedrige Raumluftqualität
*/
