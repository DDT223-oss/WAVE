/*
  Waveshare Ecosystem Integration - Round LCD Industrial Telemetry Gauge
  File: Waveshare_Round_LCD_Gauge.ino
  Description: Renders high-speed antialiased UI data gauges for the Waveshare GC9A01 circular screen.
*/

#include <SPI.h>
#include <TFT_eSPI.h> // Ensure you optimize User_Setup.h configuration parameters for GC9A01

TFT_eSPI tft = TFT_eSPI();       
TFT_eSprite img = TFT_eSprite(&tft); // Double-buffer sprite array to completely eliminate flickering screens

int gaugeAngle = 0;
bool animationDirection = true;

void drawIndustrialGauge(int angle) {
  img.fillSprite(TFT_BLACK); // Clear internal rendering buffer frame
  
  // Center alignments for standard 240x240 Waveshare circular panels
  int centerX = 120;
  int centerY = 120;
  int radius = 110;

  // Draw hardware dashboard boundaries
  img.drawCircle(centerX, centerY, radius, TFT_SILVER);
  img.drawCircle(centerX, centerY, radius - 3, TFT_DARKGREY);
  
  // Render tick marks along safe parameter zones
  for (int i = 0; i < 360; i += 30) {
    float rad = (i - 90) * 0.0174532925;
    int xStart = centerX + (radius - 12) * cos(rad);
    int yStart = centerY + (radius - 12) * sin(rad);
    int xEnd = centerX + (radius - 4) * cos(rad);
    int yEnd = centerY + (radius - 4) * sin(rad);
    img.drawLine(xStart, yStart, xEnd, yEnd, TFT_BLUE);
  }

  // Calculate dynamic coordinates for UI pointer needle positions
  float needleRad = (angle - 210) * 0.0174532925; 
  int needleX = centerX + (radius - 20) * cos(needleRad);
  int needleY = centerY + (radius - 20) * sin(needleRad);

  // Draw tracking dial needle line vector
  img.drawLine(centerX, centerY, needleX, needleY, TFT_RED);
  img.fillCircle(centerX, centerY, 6, TFT_WHITE);
  
  // Text tracking labels info
  img.setTextDatum(MC_DATUM);
  img.setTextColor(TFT_GREEN, TFT_BLACK);
  img.drawString("SYS LOAD", centerX, centerY + 40, 2);
  
  // Push full buffered canvas output trace safely out to SPI hardware array
  img.pushSprite(0, 0);
}

void setup() {
  tft.init();
  tft.setRotation(0);
  
  // Initialize virtual rendering engine space matching screen scale
  img.createSprite(240, 240);
  img.setTextWrap(false);
  
  drawIndustrialGauge(0);
}

void loop() {
  // Animate dial needles back and forth dynamically
  if (animationDirection) {
    gaugeAngle += 2;
    if (gaugeAngle >= 240) animationDirection = false;
  } else {
    gaugeAngle -= 2;
    if (gaugeAngle <= 0) animationDirection = true;
  }
  
  drawIndustrialGauge(gaugeAngle);
  delay(15); // Frame execution smoothing limit lock
}
